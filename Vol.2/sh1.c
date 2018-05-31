#include <stdlib.h>
#include <stdio.h>
#include <string.h>  
#include <unistd.h>  
#include <errno.h>
#include "sys/types.h"

int getcmd(char *buf, int nbuf);

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
    while(getcmd(buf, sizeof(buf)) >= 0){
        if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
            // Chdir must be called by the parent, not the child.
            buf[strlen(buf)-1] = 0;  // chop \n
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
            printf("%s\n", buf);

        wait();
    }

  return 0;

}

int getcmd(char *buf, int nbuf) {
  printf("$ ");
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}