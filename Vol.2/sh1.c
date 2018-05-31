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
	char *fuckyou = src;
	char *result = NULL;
	char fuckspace[] = " ";
	char *copy = malloc(10*sizeof(char));
	result = strtok(fuckyou, fuckspace);
	int i = 0; 
	while(result != NULL) {
		strcpy(dest[i], result);
		result = strtok(NULL, fuckspace);  
		i++;
	}
	
	/*
	i = 0; 
	while(dest[i][0] != '\0') {
		printf("argv[%d] = %s\n",i ,dest[i]);
		if(strcmp(dest[i], "echo") == 0) {
			printf("is echo!\n");
		}
		i++;
	}
	*/
	
	return;
}

int execCMD(char cmd[ARRAYX][ARRAYY]) {
	
	/*
	int i = 0;
	while(cmd[i][0] != '\0') {
		printf("argv[%d] = %s\n",i ,cmd[i]);
		if(strcmp(cmd[i], "echo") == 0) {
			printf("is echo!\n");
		}
		i++;
	}
	*/
	
	if(strcmp(cmd[0], "echo") == 0) {
		int pid = fork();
		if(pid == 0) {
			int i = 0;
			int is = 0;
			for(i = 1; cmd[i][0]!='\0'; i++) {
				if(cmd[i][0] == '>') {
					is = 1;
					break;
				}
			}
			if(is == 1) {
				puts(cmd[i+1]);
				FILE *fp = fopen(cmd[i+1], "w+");
				int j = 0;
				for(j = 1; j < i; j++) {
					fseek(fp, 0, SEEK_END);
					fwrite(cmd[j], strlen(cmd[j]), 1, fp);
				}
				fclose(fp);
			}
			else {
				int j = 0;
				for(j = 1; cmd[j][0] != '\0'; j++) {
					printf("%s", cmd[j]);
					printf(" ");
				}
				printf("\n");
			}
		}
		else {
			int status;
			wait(&status);
		}
	}
	
	else if(strcmp(cmd[0], "ls") == 0) {
		int pid = fork();
		if(pid == 0) {
			if(cmd[1][0] == '\0') {
				execlp("/bin/ls", "ls", "./", NULL, NULL, NULL);
			}
			else {
				execlp("/bin/ls", "ls", cmd[1], NULL, NULL, NULL);
			}
		}
		else {
			int status;
			wait(&status);
		}
	}
	
	else if(strcmp(cmd[0], "ll") == 0) {
		int pid = fork();
		if(pid == 0) {
			if(cmd[1][0] == '\0') {
				execlp("/bin/ls", "ls", "-l", "./", NULL, NULL, NULL);
			}
			else {
				execlp("/bin/ls", "ls", "-l", cmd[1], NULL, NULL, NULL);
			}
		}
		else {
			int status;
			wait(&status);
		}
	}
	
	else if(strcmp(cmd[0], "cd") == 0) {
		chdir(cmd[1]);
	}
	
	else if(strcmp(cmd[0], "pwd") == 0) {
		int pid = fork();
		if(pid == 0) {
			system("pwd");
		}
		else {
			int status;
			wait(&status);
		}
	}
	
	else if(strcmp(cmd[0], "exit") == 0) {
		return -1;
	}
	
	return 1;
	
}

int main(int argc, char** argv) {
	system("clear");
	while(1) {
		char command[LENGTH];
		char splitArray[ARRAYX][ARRAYY] = {{'\0'}};
		
		char *file_path_getcwd;
		file_path_getcwd=(char *)malloc(255);
		getcwd(file_path_getcwd,255);
				
		printf("[pichu@Cinnamon* %s]$ ", file_path_getcwd);
		
		gets(command);
		analyCMD(command, splitArray);
		int i = 0;
		if(execCMD(splitArray) == -1) {
			return 0;
		}
	}
}
