#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#define _OPEN_THREADS

pthread_t tid[9999];
long arr[9999];
long long fakto[9999];

void swap(long *xp, long *yp) 
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubbleSort(long arr[], int n) 
{ 
	int i, j; 
	for (i = 0; i < n-1; i++){
		for (j = 0; j < n-i-1; j++){
			if (arr[j] > arr[j+1]){
				swap(&arr[j], &arr[j+1]);
			}
		}
	}
} 

//algo faktorial
void *faktorial(void* arg){
	//printf("fakto\n");
	unsigned long long hasil = 1;
	long f, awal;
	awal = (long)arg;
	for(f = (long)arg; f > 0; f--){
		hasil *= (unsigned long long)f;
	}
	printf("%ld! = %llu\n", awal, hasil);
	return NULL;
}

int main(int argc, char *argv[]){
	//int n;
	//scanf("%d", &n);
	int i;
	
	long konvers;
	char *ptr;
	//konversi string ke long
	for(i = 0; i < argc - 1; i++){
		//printf("%s\n", argv[i + 1]);
		konvers = strtol(argv[i + 1], &ptr, 10);
		//printf("%ld\n\n", result);
		arr[i] = konvers;
	}
	
	//sort
	bubbleSort(arr, argc);
	for(i = 0; i < argc - 1; i++){
		//printf("%ld\n", arr[i]);
	}
	
	int err;
	for(i = 0; i < argc - 1; i++){
		err = pthread_create(&(tid[i]), NULL, &faktorial, (void*)arr[i + 1]);
		if(err != 0){
			//printf("gagal\n");
		}else{
			//printf("sukses ");
		}
		pthread_join(tid[i], NULL);
	}

	exit(0);
	return 0;
}
