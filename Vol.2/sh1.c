#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <string.h>  
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LENGTH 255
#define ARRAYX 255
#define ARRAYY 10

void analyCMD(char src[], char dest[ARRAYX][ARRAYY]) {
	char *p;
	p = strsep(&src, " ");
	
	int i = 0;
	for(i=0; p[i] != '\0'; i++) {
		dest[0][i] = p[i];
	}
	dest[0][i] = '\0';
	
	int j = 1;
	while(p) {
		p = strsep(&src, " ");
		if(p) {
			for(i=0; p[i]!='\0'; i++) {
				dest[j][i] = p[i];
			}
			dest[j][i] = '\0';
			j++;
		}
	}
	
	return;
}

int execCMD(char cmd[ARRAYX][ARRAYY]) {
	// Debug
	printf("%s", cmd[0]);
	return -1;
}

int main(int argc, char** argv) {
	while(1) {
		char command[LENGTH] = {0};
		char splitArray[ARRAYX][ARRAYY] = {{0}};
		printf("$ ");
		gets(command);
		analyCMD(command, splitArray);
		int i = 0;
		if(execCMD(splitArray) == -1) {
			return 0;
		}
	}
}
