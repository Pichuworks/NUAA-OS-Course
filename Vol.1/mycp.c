#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    if(argc != 3) {
        perror("argc");
        exit(1);
    }
    int fd1, fd2;
    int ret = 1;
    char buf[100];
    
    fd1 = open(argv[1], O_RDONLY);
    if(fd1 == -1) {
        perror("open");
        exit(1);
    }
    fd2 = creat(argv[2], 0655);
    if(fd2 == -1) {
        perror("create");
        exit(1);
    }
    while(1) {
        if(ret == 0) break;
        ret = read(fd1, buf, 99);
        write(fd2, buf, ret);
    }
    close(fd1);
    close(fd2);
    return 0;
}
