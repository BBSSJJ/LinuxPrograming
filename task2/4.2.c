#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

int my_double_ls(const char* name){
	struct dirent* d;
	struct stat s;
	DIR* dp;

	if((dp = opendir(name)) == NULL)
		return -1;

	while(d = readdir(dp)){
		if(d->d_ino != 0){
			stat(d->d_name, &s);
			if(S_ISDIR(s.st_mode))
				printf("%s*\n", d->d_name);
			else
				printf("%s\n", d->d_name);
		}
	}
	closedir(dp);
	return 0;
}

int main(int argc, char* argv[]){
	my_double_ls(argv[1]);
}

