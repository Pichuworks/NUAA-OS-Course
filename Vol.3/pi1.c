#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define LENGTH 10000

volatile double sub_data;
double main_data;

void* subThread() {
	double sum = 0.0;
	double j = LENGTH + 1.0;
	double signal = 1.0;
	for(int i = 1; i <= LENGTH/2; i++) {
		sum += signal * (1.0 / j);
		// printf("[subThread] %f * (1 / %f)\n", signal, j);
		j += 2.0;
		signal = -signal;
	}
	sub_data = sum;
	return;
}

void masterThread() {
	double sum = 0.0;
	double j = 1.0;
	double signal = 1.0;
	for(int i = 1; i <= LENGTH / 2; i++) {
		sum += signal * (1.0 / j);
		// printf("[masterThread] %f * (1 / %f)\n", signal, j);
		j += 2.0;
		signal = -signal;
	}
	main_data = sum;
	return;
}

int main() {
	
	double fuck_vim;

	printf("===START===\n");
	
	pthread_t sub_tid;
	pthread_create(&sub_tid, NULL, &subThread, NULL);
	
	masterThread();
	
	pthread_join(sub_tid, NULL);
	fuck_vim = main_data + sub_data;
	fuck_vim *= 4;
	
	printf("[main] main_data = %f, sub_data = %f, fuck_vim = %f\n===END===\n", main_data, sub_data, fuck_vim);

	return 0;
}
