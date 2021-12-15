#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void lsoct(char* ls);
void octls(char* oct);
int main(int argc, char* argv[]){
	if(strcmp(argv[1], "lsoct") == 0){
			lsoct(argv[2]);
	}
	else if(strcmp(argv[1], "octls") == 0){
			octls(argv[2]);
	}
	else
		return 0;

}

void lsoct(char* ls){
	int oct[4];
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
		if(i == 2 || i == 5 || i == 8){
			oct[top] = sum;
			top += 1;
			sum = 0;	
		}
	}
	printf("%d%d%d%d\n", oct[0], oct[1], oct[2], oct[3]);
}

void  octls(char* oct){
	char ls[9];
	for(int i = 1; i < 4; i++){
		if(oct[i] == '0')
			strcat(ls, "---");
		else if(oct[i] == '1')
			strcat(ls, "--x");
		else if(oct[i] == '2')
			strcat(ls, "-w-");
		else if(oct[i] == '3')
			strcat(ls, "-wx");
		else if(oct[i] == '4')
			strcat(ls, "r--");
		else if(oct[i] == '5')
			strcat(ls, "r-x");
		else if(oct[i] == '6')
			strcat(ls, "rw-");
		else if(oct[i] == '7')
			strcat(ls, "rwx");
	}
	printf("%s\n", ls);
}
