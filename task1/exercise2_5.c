#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFSIZE 512

int main(){
	char buffer[BUFFSIZE];
	int filedes;
	ssize_t nread;
	long word_count = 0;
	long line_count = 0;
	int word_end = 1;

	if((filedes = open("anotherfile", O_RDONLY)) == -1){
			printf("error in opening anotherfile\n");
			exit(1);
	}

	while((nread = read(filedes, buffer, BUFFSIZE)) > 0){
		for(int i = 0; i < nread; i++){
			if(buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\n'){
				if(word_end == 0)
					word_count += 1;
				word_end = 1;
			}
			else word_end = 0;
			if(buffer[i] == '\n')
				line_count += 1;
		}
	}
	

	printf("total words in anotherfile: %ld\n", word_count);
	printf("total lines in anotherfile: %ld\n", line_count);
	exit(0);

	

}
