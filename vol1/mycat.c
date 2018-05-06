#include <stdlib.h>
#include <stdio.h>
int fileCopy(FILE *src, FILE *dest) {
    int c;
    while((c = getc(src)) != EOF)
        putc(c, dest);
    return c;
}
int main(int argc, char** argv) {
    if(argc == 1) {
        fileCopy(stdin, stdout);
    }
    else {
        char *input;
        FILE *inf;
        int count = 0;
        while(--argc>0) {
            input = argv[argc];
            if((inf = fopen(input, "r")) == NULL) {
                printf("\nCan't open %s\n");
                continue;
            }
            count++;
            fileCopy(inf, stdout);
            fclose(inf);
        }
        // printf("**Cat %d file**", count);
        // printf("\n");
        return 0;
    }
}
