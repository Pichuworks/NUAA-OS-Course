#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define N 50

void* subThread(void* arg) {
    int* num;
    num = (int*) arg;
    int fuck = *num;
    printf("fuck %d\n", fuck);
    fuck++;
    *num = fuck;
    return (void*) num;
}

int main() {
    pthread_t pid[N];
    int* result = malloc(sizeof(int));
    *result = 0;
    
    pthread_create(&pid[0], NULL, subThread, result);
    pthread_join(pid[0], (void*) &result);
    
    int i = 0;
    while(i < N) {
        printf("from Thread #%d ", i+1);
        i = (i + 1) % N;
        printf("to #%d send %d\n", i+1, *result);
        pthread_create(&pid[i], NULL, subThread, (void*) result);
		pthread_join(pid[i], (void*) &result);
        sleep(1);
    }
    
    return 0;
}