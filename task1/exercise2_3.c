#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(){
	int filedes;
	if((filedes = creat("./newfile", 0644)) == -1){
		printf("creat failed\n");
		exit(1);
	}
	else printf("creat success\n");

	if((filedes = open("./newfile", O_RDWR)) == -1){
		printf("open failed\n");
		exit(1);
	}
	else printf("open success\n");
	return 0;
}
