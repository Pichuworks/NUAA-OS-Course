#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define ARRAY_N 11

struct thread_arg {
	int sortArr[ARRAY_N/2];
};

void randInt(int* array) {
	srand((unsigned int) time(NULL));
	for(int i = 0; i < ARRAY_N; i++) {
		array[i] = rand()%1000 + 1;
	}
	return;
}

void* sortThread(void* arg) {
	
	struct thread_arg* ta = (struct thread_arg*) arg;
	int* sortArr = malloc(ARRAY_N * sizeof(int));
	int mainN = ARRAY_N - (ARRAY_N / 2);

	for(int i = 0; i < mainN; i++) {
		sortArr[i] = ta -> sortArr[i];
	}
	

	for(int i = 0; i < mainN - 1; i++) {
		int k = i;
		for(int j = k + 1; j < mainN; j++) {
			if(sortArr[j] < sortArr[k]) {
				k = j;
			}
		}
		if(k != i) {
			int tmp = sortArr[i];
			sortArr[i] = sortArr[k];
			sortArr[k] = tmp;
		}
	}

	return sortArr;
}

void dispArr(int* array, int n) {
	for(int i = 0; i < n; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
	return;
}

void merge(int* mainArr, int* subArr, int* resArr) {
	int mainN = ARRAY_N / 2;
	int subN = ARRAY_N - (ARRAY_N / 2);
	int i = 0;	// main
	int j = 0;	// sub
	int k = 0;	// res
	
	while(i < mainN && j < subN) {
		if(mainArr[i] < subArr[j]) {
			resArr[k] = mainArr[i];
			k++;
			i++;
		}
		else {
			resArr[k] = subArr[j];
			k++;
			j++;
		}
	}
	
	while(i < mainN) {
		resArr[k] = mainArr[i];
		k++;
		i++;
	}

	while(j < subN) {
		resArr[k] = subArr[j];
		k++;
		j++;
	}

	return;

}

int main() {
	int sortArr[ARRAY_N];
	int mainN = ARRAY_N / 2;
	int subN = ARRAY_N - mainN;
	int mainSA[mainN];

	pthread_t sub_tid;
	randInt(sortArr);
	// printf("[mainThread] ");
	// dispArr(sortArr);

	printf("[mainThread] ");
	dispArr(sortArr, ARRAY_N);
	
	// 先给分线程封装
	struct thread_arg ta;
	for(int i = mainN; i < ARRAY_N; i++) {
		ta.sortArr[i - mainN] = sortArr[i];
	}
	void* arg = (void*) &ta;
	pthread_create(&sub_tid, NULL, sortThread, arg);

	// 主进程排序
	for(int i = 0; i < mainN; i++) {
		mainSA[i] = sortArr[i];
	}
	// 选择排序
	for(int i = 0; i < mainN - 1; i++) {
		int k = i;
		for(int j = k + 1; j < mainN; j++) {
			if(mainSA[j] < mainSA[k]) {
				k = j;
			}
		}
		if(k != i) {
			int tmp = mainSA[i];
			mainSA[i] = mainSA[k];
			mainSA[k] = tmp;
		}
	}

	// 获取子线程
	int* result;
	pthread_join(sub_tid, &result);
	
	// Debug
	printf("[mainThread] ");
	dispArr(mainSA, mainN);
	printf("[sortThread] ");
	dispArr(result, subN);
	
	// merge
	merge(mainSA, result, sortArr);

	printf("[mainThread] ");
	dispArr(sortArr, ARRAY_N);

	return 0;
}
