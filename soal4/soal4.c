#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

pthread_t tid[2];

//simpan proses ke file
void* ps(void *arg){
	int* urutan = (int*)arg;

	char str[250];
	strcpy(str, "");
	sprintf(str, "ps -aux | head -11 | tail -10 > ~/Documents/FolderProses%d/SimpanProses%d.txt", *urutan, *urutan);
	system(str);
}

//kompres ke zip
void* kompres(void *arg){
	int* urutan = (int*)arg;

	char str[250];
	strcpy(str, "");
	sprintf(str, "zip -m -j -q ~/Documents/FolderProses%d/KompresProses%d.zip ~/Documents/FolderProses%d/SimpanProses%d.txt", *urutan, *urutan, *urutan, *urutan);
	system(str);
}

//ekstrak
void* unzip(void *arg){
	int* urutan = (int*)arg;

	char str[250];
	strcpy(str, "");
	sprintf(str, "unzip -q ~/Documents/FolderProses%d/KompresProses%d.zip -d ~/Documents/FolderProses%d/", *urutan, *urutan, *urutan);
	system(str);
}

int main(){
	int* file1 = (int*)1;
	int* file2 = (int*)2;

	//simpan proses ke file
	pthread_create(&tid[0], NULL, &ps, (void*)&file1);
	pthread_create(&tid[1], NULL, &ps, (void*)&file2);
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);

	//kompres ke zip
	pthread_create(&tid[0], NULL, &kompres, (void*)&file1);
	pthread_create(&tid[1], NULL, &kompres, (void*)&file2);
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);

	printf("Menunggu 15 detik untuk mengekstrak kembali\n");
	sleep(15);

	//ekstrak
	pthread_create(&tid[0], NULL, &unzip, (void*)&file1);
	pthread_create(&tid[1], NULL, &unzip, (void*)&file2);
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);

	return 0;
}
