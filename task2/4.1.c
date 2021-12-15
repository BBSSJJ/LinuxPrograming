#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

int my_double_ls(const char* name, int skip){
	struct dirent* d;
	DIR* dp;

	if((dp = opendir(name)) == NULL)
		return -1;

	while(d = readdir(dp)){
		if(d->d_ino != 0){
			if(d->d_name[0] == '.' && skip)
				continue;
			else
				printf("%s\n", d->d_name);
		}
	}
	closedir(dp);
	return 0;
}

int main(int argc, char* argv[]){
	int skip = argv[2][0] - '0';
	my_double_ls(argv[1], skip);
}
