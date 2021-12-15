#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	if(access(argv[1], R_OK) == -1){
		char yn;
		printf("have no permission to write\ndo you want to delete file?\npress y to delete, any key to stop : ");
		scanf("%c", &yn);
		if(yn != 'y'){
			printf("terminated\n");
			return 0;
		}
	}

	if(unlink(argv[1]) == -1)
		printf("unlink failed\n");
	else
		printf("deleted\n");
	return 0;
}

