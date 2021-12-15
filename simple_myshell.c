#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#define MAX_CMD_ARG 20
#define BUFSIZ 256

#define PIPE_NO_USE -1
#define PIPE_RDWR 0
#define PIPE_RDONLY 1
#define PIPE_WRONLY 2

const char *prompt = "myshell> ";
char* cmdvector[MAX_CMD_ARG];
char  cmdline[BUFSIZ];
pid_t forpid = -1;
char* cmdpipe1[MAX_CMD_ARG];
char* cmdpipe2[MAX_CMD_ARG];
char* cmdpipe3[MAX_CMD_ARG];


typedef struct {
	int flag;
	int fd[4];
}Global_pipe;
Global_pipe global_pipe;

void fatal(char *str){
	perror(str);
	exit(1);
}

int makelist(char *s, const char *delimiters, char** list, int MAX_LIST){	
  int i = 0;
  int numtokens = 0;
  char *snew = NULL;

  if( (s==NULL) || (delimiters==NULL) ) return -1;

  snew = s + strspn(s, delimiters);	/* Skip delimiters */
  if( (list[numtokens]=strtok(snew, delimiters)) == NULL )
    return numtokens;
	
  numtokens = 1;
  
  while(1){
     if( (list[numtokens]=strtok(NULL, delimiters)) == NULL)
	break;
     if(numtokens == (MAX_LIST-1)) return -1;
     numtokens++;
  }
  return numtokens;
}

void signalhandler(int signo){
	switch(signo){
		case SIGCHLD:
			while((waitpid(-1, NULL, WNOHANG)) > 0){}
			break;
		case SIGINT:
			if(forpid != -1){
				kill(forpid, SIGINT);
				forpid = -1;
				printf("\n");
				fflush(stdout);
			}
			else{
				printf("\nmyshell> ");
				fflush(stdout);
			}
			break;
		case SIGQUIT:
			if(forpid != -1){
				kill(forpid, SIGQUIT);
				forpid = -1;
				printf("\n");
				fflush(stdout);
			}
			else{
				printf("\nmyshell> ");
				fflush(stdout);
			}
			break;
	}
}

int redirectRight(char** cmdv, int start, int end){
	int i;
	int fd;

	int stdout_backup = dup(STDOUT_FILENO);

	for(i = start; i <= end; i++){
		if(strcmp(cmdv[i], ">") == 0)
			break;
	}
	if(i == end + 1)
		return -1;

	fd = open(cmdv[i+1], O_RDWR | O_CREAT, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);

	for(i = i; i<=(end-2); i++) 
		cmdv[i] = cmdv[i+2];
	cmdv[end-1] = NULL;
	cmdv[end] = NULL;

	return stdout_backup;
}

int redirectLeft(char** cmdv, int start, int end){
	int i;
	int fd;

	int stdin_backup = dup(STDIN_FILENO);

	for(i = start; i<=end; i++){
		if(strcmp(cmdv[i], "<") == 0)
			break;
	}
	if(i == end + 1) return -1;

	fd = open(cmdv[i+1], O_RDONLY);
	dup2(fd, STDIN_FILENO);
	close(fd);

	for(; i<=(end-2); i++)
		cmdv[i] = cmdv[i+2];
	cmdv[end-1] = NULL;
	cmdv[end] = NULL;

	return stdin_backup;
}

int execute_command(char** cmdv, int start, int end, int pipe_usage) {
	int pid;
	int wait_flag=1;
	if(cmdv[end][0] == '&') { wait_flag=0; end--; }
	if(pipe_usage != -1 && !wait_flag) {
		printf("Syntax error near unexpected token '|'\n");
		return -1;
	}

	int stdin_backup = redirectLeft(cmdv, start, end);
	if(stdin_backup != -1) end -= 2;

	int stdout_backup = redirectRight(cmdv, start, end);
	if(stdout_backup != -1) end -= 2;

	char **new_cmdv = (char **)calloc((end-start+2), sizeof(char *));
	for(int i=0; i<end-start+1; i++) {
		if(cmdv[start+i] != NULL) {
			new_cmdv[i] = cmdv[start+i];
		}
		else new_cmdv[i] = NULL;
	}
	new_cmdv[end-start+1] = NULL;

	int write_backup_fd=-1, read_backup_fd=-1;
	int write_pipe_fd, read_pipe_fd;
	if(pipe_usage != PIPE_NO_USE) {
		switch(pipe_usage) {
		case PIPE_RDWR:
			read_backup_fd = dup(STDIN_FILENO);
			read_pipe_fd = global_pipe.flag ? global_pipe.fd[0] : global_pipe.fd[2];
			dup2(read_pipe_fd, STDIN_FILENO);
			close(read_pipe_fd);
			write_backup_fd = dup(STDOUT_FILENO);
			write_pipe_fd = global_pipe.flag ? global_pipe.fd[3] : global_pipe.fd[1];
			dup2(write_pipe_fd, STDOUT_FILENO);
			close(write_pipe_fd);
			break;
		case PIPE_RDONLY:
			read_backup_fd = dup(STDIN_FILENO);
			read_pipe_fd = global_pipe.flag ? global_pipe.fd[0] : global_pipe.fd[2];
			dup2(read_pipe_fd, STDIN_FILENO);
			close(read_pipe_fd);
			break;
		case PIPE_WRONLY:
			write_backup_fd = dup(STDOUT_FILENO);
			write_pipe_fd = global_pipe.flag ? global_pipe.fd[3] : global_pipe.fd[1];
			dup2(write_pipe_fd, STDOUT_FILENO);
			close(write_pipe_fd);
			break;
		}
	}

	// Execute
	switch(pid=fork()){
        case 0:
                signal(SIGINT, SIG_DFL);
                signal(SIGQUIT, SIG_DFL);
                signal(SIGCHLD, SIG_DFL);
                if(execvp(new_cmdv[0], new_cmdv) == -1)
			perror("execvp");
                fatal("main()");
        case -1:
                fatal("main()");
        default:
                if(wait_flag){
                        forpid = pid;
                        waitpid(pid, NULL, 0);
                        forpid = -1;
                }
        }

	// Restore STDIN/STDOUT
	if(stdin_backup != -1) {
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
	}
	if(stdout_backup != -1) {
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdout_backup);
	}
	if(read_backup_fd != -1) {
		dup2(read_backup_fd, STDIN_FILENO);
		close(read_backup_fd);
	}
	if(write_backup_fd != -1) {
		dup2(write_backup_fd, STDOUT_FILENO);
		close(write_backup_fd);
	}

	if(pipe_usage == PIPE_RDWR) pipe(global_pipe.flag ? global_pipe.fd : global_pipe.fd+2);

	free(new_cmdv);
	return 0;
}

int run_commands(char** cmdv, int numtokens){
	int pipe_count = 0;
	int this_start=0, this_end=0;
	int redirect_flag = 0; // 0: no redirect, 1: left 2: right
	int redirect_file = 0;

	// initialize pipes
	int pipe_usage = 0; // -1: no pipe r/w, 0: Read & write, 1: Only read, 2: Only write
	global_pipe.flag = 0; // 0: fd[0], fd[1] & 1: fd[2], fd[3]
	pipe(global_pipe.fd);
	pipe(global_pipe.fd+2);

	if(cmdv[numtokens-1][0] == '|' || cmdv[0][0] == '|') {
		printf("Invalid expression\n");
		return -1;
	}

	for(int i=0; i<numtokens; i++) {
		if(cmdv[i][0] == '|') {
			this_end = i - 1;
			if(this_start == 0) pipe_usage = PIPE_WRONLY;
			else pipe_usage = PIPE_RDWR;
			if(execute_command(cmdv, this_start, this_end, pipe_usage) < 0) return -1;
			// Execution error
			global_pipe.flag = global_pipe.flag ? 0 : 1;
			this_start = i + 1;
		}
	}
	this_end = numtokens - 1;
	if(this_start == 0) pipe_usage = PIPE_NO_USE;
	else pipe_usage = PIPE_RDONLY;
	if(execute_command(cmdv, this_start, this_end, pipe_usage) < 0) return -1;

	for(int i=0; i<4; i++) close(global_pipe.fd[i]);
	return 0;
}

int main(int argc, char**argv){
  pid_t pid;
  int wait_flag;
  signal(SIGINT, signalhandler);
  signal(SIGQUIT, signalhandler);
  signal(SIGCHLD, signalhandler);


  int start_num, end_num;

  while (1) {
  	fputs(prompt, stdout);
	fgets(cmdline, BUFSIZ, stdin);
	if(cmdline[0] == '\n') continue;

	cmdline[strlen(cmdline) -1] = '\0';
	int numtokens = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);

 	if(strcmp(cmdvector[0], "cd") == 0){
		chdir(cmdvector[1]);
		continue;
	}
	else if(strcmp(cmdvector[0], "exit") == 0)
		return 0;

	run_commands(cmdvector, numtokens);
  }
  return 0;
}
