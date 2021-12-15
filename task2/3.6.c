#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void whatable(char* filename){
	if(access(filename, R_OK) == 0)
		printf("can read file\n");
	else
		perror("have no read permission ");
	if(access(filename, W_OK) == 0)
		printf("can write file\n");
	else
		perror("have no write permission ");
	if(access(filename, X_OK) == 0)
		printf("can excute file\n");
	else
		perror("have no	excute permission ");
}

int main(int argc, char* argv[]){
	whatable(argv[1]);
}
