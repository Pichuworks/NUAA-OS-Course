#include <stdlib.h>
#include <stdio.h>
#include <string.h>  
#include <unistd.h>  
#include <errno.h>

int system(const char* cmdstring) {
    pid_t pid;
    int status;

    // 命令为空直接跑路
    if(cmdstring == NULL) {        
        return (1);
    }

    // fork一个进程 这里是 pid<0 爆炸？
    if((pid = fork())<0) {
        status = -1;
    }

    // fork的子进程，因为fork在子进程里是返回0的
    else if(pid == 0) {
        // int execl(const char *path, const char *arg, ...)
        // 调用execl
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        _exit(127);
    }
    else {
        // 主进程
        while(waitpid(pid, &status, 0) < 0) {
            // 执行成功？
            if(errno != EINTER) {
                status = -1;
                break;
            }
        }
    }
    return status;
 }

int main(int argc, char** argv) {
    if(argc == 1) {
        perror("argc");
        exit(1);
    }

    char cmdstring[255];
    strcpy(cmdstring, argv[1]);
    for(int i=2; i<argc; i++) {
        strcat(str, " ");
        strcat(str, argv[i]);
    }

    int status = system(cmdstring);

    // debug
    printf("\n------------\nStatus: %d", status);

    return 0;
}


