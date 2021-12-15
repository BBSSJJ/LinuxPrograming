#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[] ){
	char* filename = argv[1];
	char* opentype = argv[2];
	int filedes;
	
	if(!strcmp(opentype, "r"))
		filedes = open(filename, O_RDONLY);
	else if(!strcmp(opentype, "w"))
		filedes = open(filename, O_WRONLY);
	else if(!strcmp(opentype, "rw"))
		filedes = open(filename, O_RDWR);
	else if(!strcmp(opentype, "a"))
		filedes = open(filename, O_WRONLY | O_APPEND);
	else
		filedes = -1;
}
