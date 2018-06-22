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

typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} semaphore;

semaphore mutex1_sema;
semaphore mutex2_sema;
semaphore empty_buf1_sema;
semaphore empty_buf2_sema;
semaphore full_buf1_sema;
semaphore full_buf2_sema;


void semaInit(semaphore* sema, int value) {
    sema->value = value;
    pthread_mutex_init(&sema->mutex, NULL);
    pthread_cond_init(&sema->cond, NULL);
}

void semaWait(semaphore* sema) {
    pthread_mutex_lock(&sema->mutex);
    
    while(sema->value <= 0) {
        pthread_cond_wait(&sema->cond, &sema->mutex);
    }
    sema->value--;
    pthread_mutex_unlock(&sema->mutex);
}

void semaSignal(semaphore* sema) {
    pthread_mutex_lock(&sema->mutex);
    sema->value++;
    pthread_cond_signal(&sema->cond);
    pthread_mutex_unlock(&sema->mutex);
}

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
        // printf("fuck prod\n");

        semaWait(&empty_buf1_sema);
        semaWait(&mutex1_sema);
        
        item = 'a' + i;
        putItem1(item);
        
        semaSignal(&mutex1_sema);
        semaSignal(&full_buf1_sema);

        // printf("[producer] %c\n", item);
   }
}

void* computer() {
    
    char item;
    for(int i = 0; i < ITEM_COUNT; i++) {
        // printf("fuck comp\n");

        semaWait(&full_buf1_sema);
        semaWait(&mutex1_sema);

        item = getItem1();
        
        semaSignal(&mutex1_sema);
        semaSignal(&empty_buf1_sema);
        
        // printf("[computer] get %c\n", item);
        
        //======================================= 
        
        semaWait(&empty_buf2_sema);
        semaWait(&mutex2_sema);

        item -= 32;
        putItem2(item);

        semaSignal(&mutex2_sema);
        semaSignal(&full_buf2_sema);

        // printf("[computer] put %c\n", item);
    }
 

}

void* customer() {
    // printf("fuck cust\n");

    char item;
    for(int i = 0; i < ITEM_COUNT; i++) {
        semaWait(&full_buf2_sema);
        semaWait(&mutex2_sema);

        item = getItem2();
        printf("[customer] %c\n", item);

        semaSignal(&mutex2_sema);
        semaSignal(&empty_buf2_sema);
    }
}

int main() {
    
    // printf("[mainThread] Initial\n");

    pthread_t comp_tid;
    pthread_t cust_tid;
    pthread_t proc_tid;

    semaInit(&mutex1_sema, 1);
    semaInit(&mutex2_sema, 1);
    semaInit(&empty_buf1_sema, CAPACITY - 1);
    semaInit(&empty_buf2_sema, CAPACITY - 1);
    semaInit(&full_buf1_sema, 0);
    semaInit(&full_buf2_sema, 0);

    pthread_create(&cust_tid, NULL, customer, NULL);
    pthread_create(&proc_tid, NULL, producer, NULL);
    pthread_create(&comp_tid, NULL, computer, NULL);
      
    pthread_join(cust_tid, NULL);
    pthread_join(proc_tid, NULL);
    pthread_join(comp_tid, NULL);

    return 0;
}
