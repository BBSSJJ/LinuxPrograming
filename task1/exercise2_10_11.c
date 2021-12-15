#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define NAMELENGTH 41
#define NROOMS 10

char namebuf[NAMELENGTH];
int rooms[NROOMS];

char* getoccupier(int roomno){
	int infile = -1;
	off_t offset;
	ssize_t nread;

	if(infile == -1 && (infile = open("residents", O_RDWR)) == -1)
		return NULL;

	offset = (roomno - 1) * NAMELENGTH;


	if(lseek(infile, offset, SEEK_SET) == -1)
		return NULL;

	if((nread = read(infile, namebuf, NAMELENGTH)) <= 0)
		return NULL;
	
	namebuf[nread - 1] = '\0';
	return namebuf;
}
char* findfree(){
	/*
	off_t offset = 0;
	ssize_t nread;
	int infile = -1;
	
	if(infile == -1 && (infile = open("residents", O_RDONLY)) == -1)
		return NULL;
	
	for(int i = 1; i <= NROOMS; i++){
		if(lseek(infile, offset, SEEK_SET) == -1)
			return -1;
		if((nread = read(infile, namebuf, NAMELENGTH)) <= 0)
			return i;
		offset += NAMELENGTH;
	}
	*/

	
	printf("Room ");
	for(int i = 0; i < NROOMS; i++){
		if(rooms[i] == 0)
			printf("%d ", i + 1);
	}
	printf("is empty\n");
}

void freeroom(int roomnumber){
	int infile = -1;
	off_t offset;
	
	if(infile == -1 && (infile = open("residents", O_RDWR)) == -1)
		return;
	offset = (roomnumber - 1) * NAMELENGTH;
	for(int i = 0; i < NAMELENGTH; i++)
		namebuf[i] = ' ';
	lseek(infile, offset, SEEK_SET);
	write(infile, namebuf, NAMELENGTH);
	rooms[roomnumber - 1] = 0;
	close(infile);
}

void addguest(int roomnumber, char* name){
	int infile = -1;
	off_t offset;
	char *p = malloc(sizeof(char) * 40 + 1);
	
	if(infile == -1 && (infile = open("residents", O_RDWR)) == -1)
		return;
	if(rooms[roomnumber - 1] == 1){
		printf("Room %d is not empty\n", roomnumber);
		return;
	}
	strcpy(p, name);
	offset = (roomnumber - 1) * NAMELENGTH;
	lseek(infile, offset, SEEK_SET);
	write(infile, p, NAMELENGTH);
	rooms[roomnumber - 1] = 1;
	close(infile);
}

int main(){
	char* getoccupier(int), *p;
	int select;
	int roomnumber;
	char name[NAMELENGTH];
	
	while(1){
		printf("1. get occupier\n2. free room\n3. add guest\n4. find free\nselect:");
		scanf("%d", &select);
		if(select == 1){
			for(int j = 1; j <= NROOMS; j++){
				if(p = getoccupier(j))
					printf("Room %2d %s\n", j, p);
				else
					printf("Room %2d \n", j);
			}
		}
		else if(select == 2){
			printf("Room number to free: ");
			scanf("%d", &roomnumber);
			freeroom(roomnumber);
		}
		else if(select == 3){
			printf("Room number: ");
			scanf("%d", &roomnumber);
			printf("Name: ");
			scanf("%s", name);

			addguest(roomnumber, name);
		}
		else if(select == 4){
			findfree();
		}
		else
			break;
	}
	
}
