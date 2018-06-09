#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define N 4

char buf1[N] = {0};
char buf2[N] = {0};
int buf1_in;
int buf2_in;
int buf1_out;
int buf2_out;

int isBuf1Empty() {
    return buf1_in = buf1_out;
}

int isBuf2Empty() {
    return buf2_in = buf2_out;
}

int isBuf1Full() {
    return (buf1_in + 1) % N == buf1_out;
}

int isBuf2Full() {
    return (buf2_in + 1) % N == buf2_out;
}

int getItem1() {
    
}

void* producer() {
   
}

void* computer() {

}

void* customer() {

}

int main() {
    // Initial
    pthread_t prod_tid;
    pthread_t comp_tid;
    pthread_t cust_tid;

    pthread_create(&prod_tid, NULL, producer, NULL);
    pthread_create(&comp_tid, NULL, computer, NULL);
    pthread_create(&cust_tid, NULL, customer, NULL);

    pthread_join(prod_tid, NULL);
    pthread_join(prod_tid, NULL);
    pthread_join(prod_tid, NULL);

    return 0;
}