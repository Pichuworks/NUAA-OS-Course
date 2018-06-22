#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <string.h>  
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LENGTH 512

int global_argc;

// 求字符串的argc
int num(char *buf) {
	char *index;
	index = buf;
	int result = 1;

    // 砍句前空格
	while(*index == ' ')
		index++;

	if(*index == '\0' || *index == '\n')
		return 0;

    // 逐个扫描开始计数
	while(*index != '\0' && *index != '\n')
	{
		if(*index == ' ')
			result++;
		index++;
	}

	return result;
}

// 分割字符串
char** pare(char* buf) {
	char **argv;

    // 砍句前空格
	while(*buf == ' ')
		buf++;

    // 求argc
	global_argc = num(buf);

    // 申请argv
	argv = (char**) malloc(sizeof(char*) * global_argc);   // malloc 了一堆一维数组指针
	int i = 0, j = 0;
	char tmp[LENGTH];   // 获取argv
	for(i = 0; i < global_argc; i++) {
		argv[i] = (char*) malloc(sizeof(char) * LENGTH);    // malloc 的真正的玩意
		while(*buf != ' ' && *buf != '\n') {
			if(*buf == '\0' || *buf == '\n')
				break;  // 词尾，结束
			tmp[j] = *buf;
			buf++;
			j++;
		}
		tmp[j] = '\0';
		buf++;
		j = 0;
		strcpy(argv[i],tmp);
	}

    argv[i] = NULL;
	return argv;
}

// 内置命令
int buildinCMD(char **argv) {
    if(strcmp(argv[0],"exit") == 0) {
        exit(0);
        return 1;
    }

    if(strcmp(argv[0],"pwd") == 0) {
        char *buf = getcwd(NULL,0);
        printf("%s\n",buf);
        return 1;
    }

    if(strcmp(argv[0],"cd") == 0) {
        if(chdir(argv[1])) {
            printf("-bash: no such directory\n");
        }
        return 1;
    }
    return 0;
}

// 运行
void eval(char *cmdstring) {
    char **argv = (char**)malloc(sizeof(char*));
    char buf[LENGTH];
    strcpy(buf, cmdstring);
    argv = pare(buf);       // 分割字符串

    if(argv[0] == NULL)
        return;
    
    if(buildinCMD(argv) != 0) return;

    // 执行其他命令
    int pid = fork();
    if(pid == 0) {

        // 沙雕重定向！！！
        int i = 0;
        char** tmpcmd = (char**) malloc(sizeof(char*) * global_argc); 
        int tcmdp = 0;  // tmp cmd pointer
        int fd[LENGTH] = {0};
        int recur = 0;
        
        int sfd = dup(STDOUT_FILENO);

        // printf("[eval] global_argc = %d\n", global_argc);

        while(argv[i] != '\0') {
            // printf("[eval] argv[%d] = %s\n", i, argv[i]);
            if(strcmp(argv[i], ">") == 0) {
                if(strcmp(argv[i+1], ">") == 0) {
                    printf("-bash: syntax error near unexpected token '>'\n");
                    return;
                }
                // 打开后面的文件
                fd[recur] = open(argv[i+1], O_CREAT|O_RDWR, 0755);
                recur++;
                i+=2;
            }
            else {
                tmpcmd[tcmdp] = (char*) malloc(sizeof(char) * LENGTH);
                strcpy(tmpcmd[tcmdp], argv[i]);
                i++;
                tcmdp++;
            }
        }
        tmpcmd[tcmdp] = NULL;

        recur--;    // 多加了一个=。=

        if(recur != 0) {
            dup2(fd[recur], 1); // 重定向输出
        }

        if(execvp(tmpcmd[0], tmpcmd) < 0) {
            dup2(sfd, STDOUT_FILENO);
            printf("-bash: %s: command not found\n", argv[0]);
            return;
        }

        while(recur > 0) {
            close(fd[recur]);
            recur--;
        }
        close(fd[0]);

        dup2(sfd, STDOUT_FILENO);   // 恢复输出
    }
    wait(&pid);
}

int main(int argc,char *argv[]) {
    system("clear");
    char cmdstring[LENGTH];
    int n;
    while(1) {
        char *file_path_getcwd;
		file_path_getcwd = (char *) malloc(255);
		getcwd(file_path_getcwd,255);
        printf("[pichu@Cinnamon* %s]$ ", file_path_getcwd);
        fflush(stdout);
        if((n = read(0, cmdstring, 512)) < 0) {
            printf("-bash: read error\n");
            continue;
        }
        eval(cmdstring);
    }
    return 0;
}