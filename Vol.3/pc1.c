#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define CAPACITY 4
#define ITEM_COUNT CAPACITY * 2

char buf1[CAPACITY] = {0};
char buf2[CAPACITY] = {0};
int buf1_in;
int buf2_in;
int buf1_out;
int buf2_out;

pthread_mutex_t mutex;
pthread_mutex_t mutey;
pthread_cond_t wait_empty_buf1;
pthread_cond_t wait_empty_buf2;
pthread_cond_t wait_full_buf1;
pthread_cond_t wait_full_buf2;

int isBuf1Empty() {
    return buf1_in == buf1_out;
}

int isBuf2Empty() {
    return buf2_in == buf2_out;
}

int isBuf1Full() {
    return (buf1_in + 1) % CAPACITY == buf1_out;
}

int isBuf2Full() {
    return (buf2_in + 1) % CAPACITY == buf2_out;
}

char getItem1() {
    int item;

    item = buf1[buf1_out];
    buf1_out = (buf1_out + 1) % CAPACITY;
    return item;
}

char getItem2() {
    int item;

    item = buf2[buf2_out];
    buf2_out = (buf2_out + 1) % CAPACITY;
    return item;
}

void putItem1(char item) {
    buf1[buf1_in] = item;
    buf1_in = (buf1_in + 1) % CAPACITY;
}

void putItem2(char item) {
    buf2[buf2_in] = item;
    buf2_in = (buf2_in + 1) % CAPACITY;
}

void* producer() {
   char item;
   for(unsigned short i = 0; i < ITEM_COUNT; i++) {
       pthread_mutex_lock(&mutex);
       while(isBuf1Full()) {
           pthread_cond_wait(&wait_empty_buf1, &mutex);
        }
        
        item = 'a' + i;
        // printf("[producer] %c\n", item);
        putItem1(item);
        pthread_cond_signal(&wait_full_buf1);
        pthread_mutex_unlock(&mutex);
    }
}

void* computer() {
    char item;
    for(int i = 0; i < ITEM_COUNT; i++) {

        pthread_mutex_lock(&mutex);
        while(isBuf1Empty()) {
            pthread_cond_wait(&wait_full_buf1, &mutex);
        }

        item = getItem1();
        
        // printf("[computer] get %c\n", item);
        item -= 32;

        pthread_cond_signal(&wait_empty_buf1);
        pthread_mutex_unlock(&mutex);
        
        // 
        
        pthread_mutex_lock(&mutey);
        while(isBuf2Full()) {
            pthread_cond_wait(&wait_empty_buf2, &mutey);
        }

        putItem2(item);
        // printf("[computer] put %c\n", item);

        pthread_cond_signal(&wait_full_buf2);
        pthread_mutex_unlock(&mutey);
    }

}

void* customer() {
    // printf("[customer] showtime\n");
    char item;
    for(int i = 0; i < ITEM_COUNT; i++) {
        pthread_mutex_lock(&mutey);
        while(isBuf2Empty()) {
            pthread_cond_wait(&wait_full_buf2, &mutey);
        }

        item = getItem2();
        printf("[customer] %c\n", item);

        pthread_cond_signal(&wait_empty_buf2);
        pthread_mutex_unlock(&mutey);
    }
}

int main() {
    
    printf("[mainThread] Initial\n");

    pthread_t prod_tid;
    pthread_t comp_tid;
    pthread_t cust_tid;

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutey, NULL);
    pthread_cond_init(&wait_empty_buf1, NULL);
    pthread_cond_init(&wait_empty_buf2, NULL);
    pthread_cond_init(&wait_full_buf1, NULL);
    pthread_cond_init(&wait_full_buf2, NULL);

    printf("[mainThread] Create Computer\n");
    pthread_create(&comp_tid, NULL, computer, NULL);
    printf("[mainThread] Create Customer\n");
    pthread_create(&cust_tid, NULL, customer, NULL);
    producer();
    
    
    pthread_join(comp_tid, NULL);
    pthread_join(cust_tid, NULL);

    return 0;
}
