#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <termios.h>


int *stokmakantoko;
pthread_t tid;

int kbhit()
{
	struct termios oldt, newt;
	int ch;
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	
	ch = getchar();
	
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
	return ch;
}

void* inshopmode(void *arg){
	while(1) {
		printf("Shop\n");
		printf("Food stock : %d\n", *stokmakantoko);
		printf("Choices\n");
		printf("1. Restock\n");
		printf("2. Exit\n");
		sleep(1);
        
		system("clear");
    }
}

int main() {
	key_t key = 1234;

	int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
	stokmakantoko = shmat(shmid, NULL, 0);
	
	system("clear");

	char inp;
    pthread_create(&tid, NULL, &inshopmode, NULL);

	while(1){
		inp = kbhit();
		if(inp == '1'){
			*stokmakantoko = *stokmakantoko + 1;
		}
		else if(inp == '2'){
			printf("Exit\n");
			sleep(1);
			exit(-1);
			system("clear");
        }
    }
    system("clear");
}
