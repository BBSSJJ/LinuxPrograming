#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void slowwatch(const char*);
struct stat sb;
time_t last_time;

int main(int argc, char* argv[]){
	int j;
	int fd;
	if((fd = open(argv[1], O_RDWR | O_CREAT, 0755)) == -1){
		printf("file open failed");
		return 0;
	}
	close(fd);
	if(argc > 2)
		printf("input only one file");

	if(stat(argv[1], &sb) == -1){
		printf("lookout: couldn't stat %s\n", argv[1]);
		exit(1);
	}
	last_time = sb.st_mtime;

	for(;;){
		slowwatch(argv[1]);
		sleep(10);
	}
}

void slowwatch(const char* filename){
	if(stat(filename, &sb) == -1 || sb.st_mtime != last_time){
		printf("file changed\n");
		int fd;
		ssize_t nread;
		char buff[512];

		if((fd = open(filename, O_RDONLY)) == -1){
			printf("file open failed");
			return;
		}
		while((nread = read(fd, buff, 512)) > 0){
			printf("%s",buff);
		}
		memset(buff, 0, 512);
		close(fd);
		last_time = sb.st_mtime;
	}
}

