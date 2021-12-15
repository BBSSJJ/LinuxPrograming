#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int oct[4];

int lsoct(char* ls){
	int top = 1;
	int sum = 0;
	oct[0] = 0;
	for(int i = 0; i < 9; i++){
		if(ls[i] == 'r')
			sum += 4;
		else if(ls[i] == 'w')
			sum += 2;
		else if(ls[i] == 'x')
			sum += 1;
		else if(ls[i] != '-'){
			printf("wrong permission input\n");
			return 0;
		}
		if(i == 2 || i == 5 || i == 8){
			oct[top] = sum;
			top += 1;
			sum = 0;	
		}
	}
	return 1;
}

void setperm(char* filename){
	int newPerm = oct[1]*64 + oct[2]*8 + oct[3]*1;
	if(chmod(filename, newPerm) == -1)
		perror("call to chmod failed");
}

int main(int argc, char* argv[]){
	int length = strlen(argv[1]);
	if(length == 9){
		if(!lsoct(argv[1]))
			return 0;
	}
	else if(length == 4){
		for(int i = 0; i < 4; i++){
			oct[i] = argv[1][i] - '0';
			if(oct[i] < 0 || oct[i] > 7 || oct[0] != 0){
				printf("wrong permission input\n");
				return 0;
			}
		}
	}
	else{
		printf("wrong permission input\n");
		return 0;
	}
	setperm(argv[2]);
}
