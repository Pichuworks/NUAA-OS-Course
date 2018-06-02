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

char** analyCMD(char src[]) {
	
	char** dest = NULL;
	char *fuckyou = src;
	dest = (char**) malloc(10 * sizeof(char*));
	char *result = NULL;
	char fuckspace[] = " ";
	result = strtok(fuckyou, fuckspace);
	int i = 0; 
	while(result != NULL) {
		dest[i] = (char*) malloc(255 * sizeof(char));
		strcpy(dest[i], result);
		result = strtok(NULL, fuckspace);  
		i++;
	}
	dest[i] = NULL;
	
	return dest;
}

int execCMD(char** cmd) {
	
	if(strcmp(cmd[0], "echo") == 0) {
		int pid = fork();
		if(pid == 0) {
			int i = 0;
			int is = 0;
			for(i = 1; cmd[i] != NULL; i++) {
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
				for(j = 1; cmd[j] != NULL; j++) {
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
			execvp("/bin/ls", cmd);
		}
		else {
			int status;
			wait(&status);
		}
	}
	
	else if(strcmp(cmd[0], "ll") == 0) {
		int pid = fork();
		if(pid == 0) {
			char** ll = (char**) malloc(11 * sizeof(char*));
			ll[0] = "ls";
			ll[1] = "-l";
			ll[2] = NULL;
			int i = 1;
			int j = 2;
			while(cmd[i] != NULL) {
				ll[j] = (char*) malloc(255 * sizeof(char));
				strcpy(ll[j], cmd[i]);
				i++;
				j++;
			}
			ll[j] = NULL;
			execvp("/bin/ls", ll);
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
			char pwd[255];
			if(!getcwd(pwd, 255)){  
		        perror("getcwd");  
		        return 1;  
		    }  
    		printf("%s\n", pwd);  
		}
		else {
			int status;
			wait(&status);
		}
	}
	
	else if(strcmp(cmd[0], "exit") == 0) {
		return -1;
	}
	
	else if(strcmp(cmd[0], "cat") == 0) {
		int pid = fork();
		if(pid == 0) {
			execvp("cat", cmd);
		}
		else {
			int status;
			wait(&status);
			printf("\n");
		}
	} 
	
	return 1;
	
}

int main(int argc, char** argv) {
	
	system("clear");
	
	while(1) {
		char command[LENGTH];

		char **splitArray = NULL;
		
		char *file_path_getcwd;
		file_path_getcwd = (char *) malloc(255);
		getcwd(file_path_getcwd,255);
				
		printf("[pichu@Cinnamon* %s]$ ", file_path_getcwd);
		
		gets(command);
		splitArray = analyCMD(command);

		if(execCMD(splitArray) == -1) {
			return 0;
		}
	}
	
}
