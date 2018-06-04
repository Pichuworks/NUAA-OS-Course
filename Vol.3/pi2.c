#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define LENGTH 100000000
#define THREAD_N 20

struct thread_arg {
	int serial;
	double start_value;
	double end_value;
};

volatile double sub_data[THREAD_N];

void* subThread(void* arg) {
	struct thread_arg *ta = (struct thread_arg*) arg;
	int serial = ta -> serial;
	double start_value = ta -> start_value;
	double end_value = ta -> end_value;
	double sign;
	// DEBUG
	// printf("[Thread #%d] start_value = %f, end_value = %f\n", serial, start_value, end_value);
	int int_s = (int) start_value;
	if((int_s / 2) % 2 == 0)
		sign = 1.0;
	else
		sign = -1.0;

	sub_data[serial] = 0;
	for(int i = start_value; i <= end_value; i += 2) {
		sub_data[serial] += sign * (1.0 / i);
		sign = -sign;
	}

	return;
}

int main() {
	
	double fuck_vim = 0;

	// printf("===START===\n");
	
	pthread_t sub_tid[THREAD_N];
	struct thread_arg ta[THREAD_N];

	double start_value = 1;
	int packet = LENGTH / THREAD_N;

	for(int i = 0; i < THREAD_N; i++) {
		ta[i].serial = i;
		ta[i].start_value = start_value;
		int tmp = start_value + (packet - 1) * 2;
		if(tmp > LENGTH * 2)
			ta[i].end_value = LENGTH;
		else
			ta[i].end_value = tmp;
		void* arg = (void*) &ta[i];
		pthread_create(&sub_tid[i], NULL, subThread, arg);
		start_value = start_value + packet * 2;
	}
	
	for(int i = 0; i < THREAD_N; i++) {
		pthread_join(sub_tid[i], NULL);
	}
	// fuck_vim = main_data + sub_data;
	// fuck_vim *= 4;
	
	for(int i = 0; i < THREAD_N; i++) {
		fuck_vim += sub_data[i];
		printf("Thread #%d = %f\n", i, sub_data[i]);
	}
	
	fuck_vim *= 4;
	printf("Fuck VIM! Result = %f\n", fuck_vim);

	return 0;
}
