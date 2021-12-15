#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define SIZE 512

int main(int argc, char* argv[]){
	ssize_t nread;
	char buf[SIZE];
	int filedes;

	if(argc > 1){
		for(int i = 1; i < argc; i++){
			if((filedes = open(argv[i], O_RDWR)) == -1)
				return -1;
			while((nread = read(filedes, buf, SIZE)) > 0)
				write(1, buf, nread);
		}
	}
	else if(argc == 1){
		nread = read(0, buf, SIZE);
		write(1, buf, nread);
	}
	else
		return -1;
}
