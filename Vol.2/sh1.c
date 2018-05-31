#include <stdlib.h>
#include <stdio.h>
#include <string.h>  
#include <unistd.h>  
#include <errno.h>
#include <fcntl.h>
#include "sys/types.h"

#define EXEC  1
#define REDIR 2
#define PIPE  3
#define LIST  4
#define BACK  5

#define MAXARGS 10

struct command {
  int type;
};

struct execCMD {
  int type;
  char *argv[MAXARGS];
  char *eargv[MAXARGS];
};

int getCMD(char *buf, int nbuf);
int analysisCMD(char *buf, int nbuf);

int main(int argc, char** argv) {
    static char buf[255];
    int fd;
    int pid;

    while((fd = open("console", O_RDWR)) >= 0){
        if(fd >= 3){
        close(fd);
        break;
        }
    }

    // Read and run input commands.
    while(getCMD(buf, sizeof(buf)) >= 0){
        if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
            // cd命令需要在主进程实现[这是为什么呢，XV6实现是这样的]
            // 我傻了，cd肯定要在主进程切换目录
            buf[strlen(buf)-1] = 0;
            if(chdir(buf+3) < 0)
                printf(2, "cannot cd %s\n", buf+3);
            continue;
        }
        
        pid = fork();
        if(pid == -1) {
            printf("fork ERR\n");
            continue;
        }

        if(pid == 0)
            analysisCMD(buf, sizeof(buf));

        wait();
    }

  return 0;

}

int getCMD(char* buf, int nbuf) {
  printf("$ ");
  memset(buf, 0, nbuf);
  gets(buf);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

int analysisCMD(char* buf, int nbuf) {
  // Debug
  printf("%s\n", buf);

  char *ebuf;
  struct cmd *cmd;

  ebuf = buf + strlen(buf);
  cmd = parse(&buf, ebuf);

  return 0;
}

int parse(char** ps, char* es) {
  
}
