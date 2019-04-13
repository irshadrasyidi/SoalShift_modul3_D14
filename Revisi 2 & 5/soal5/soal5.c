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

pthread_t tid[100];

char nama[50];
int hunger = 205;
int hygiene = 110;
int health = 295;

int stokmakan = 100;
int* stokmakantoko;

int cooldownbath = 0;
int enemyhealth;

char inp;
int status = 1;

int i;
void* basetime(void *arg)
{
	i = 1;
	while(1) {
		i++;
		sleep(1);
	}
}

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

void standbymode();
void battlemode();
void shopmode();

void standbymode(){
	while(1){
		system("clear");
		if(hunger <= 0) {
			printf("%s mati lapar\n", nama);
			sleep(1);
			exit(-1);
		}
		if(hygiene <= 0) {
			printf("%s mati sakit\n", nama);
			sleep(1);
			exit(-1);
		}

		printf("Standby Mode\n");
		printf("Health    : %d\n", health);
		printf("Hunger    : %d\n", hunger);
		printf("Hygiene   : %d\n", hygiene);
		printf("Food left : %d\n", stokmakan);
		if(cooldownbath == 0 || i - cooldownbath > 20){
			printf("Bath is ready\n");
		}
		else{
			printf("Bath will be ready in %d seconds\n", cooldownbath + 20 - i);
		}
		 
		printf("Choices\n");
		printf("1. Eat\n");
		printf("2. Bath\n");
		printf("3. Battle\n");
		printf("4. Shop\n");
		printf("5. Exit\n");
		inp = kbhit();
		
		if(inp == '1') {
			if(stokmakan > 0){
				if(hunger >= 200){
					printf("Full\n");
				}
				else if(stokmakan -15 < 0){
					hunger += stokmakan;
					stokmakan = 0;
				}
				else{
					stokmakan -= 15;
					hunger += 15;
				}
				
				if(hunger > 200) {
					stokmakan += (hunger-200);
					hunger = 200;
				}
			}
			else{
				printf("stok habis\n");
			}
		}
		else if(inp == '2'){
			if(cooldownbath == 0 || i - cooldownbath > 20) {
				cooldownbath = i;
				printf("Bath is ready\n");
				hygiene += 30;
			}
			else{
				printf("Bath will be ready in %ds\n", cooldownbath + 20 - i);
			}
		}
		else if(inp == '3'){
			enemyhealth = 100;
			system("clear");
			status = 0;
			
			battlemode();
		}
		else if(inp == '4'){
			system("clear");
			status = 0;
			
			shopmode();
		}	
		else if(inp == '5'){
			printf("Bye Bye\n");
			sleep(1);
			system("clear");
			exit(-1);
		}
		else{
			printf("action invalid\n");
		}
		
		system("clear");
		//sleep(1);
	}
	
}

void battlemode(){
	while(1){
		printf("Battle Mode\n");
		printf("Monster's Health : %d\n", health);
		printf("Enemy's Health : %d\n", enemyhealth);
		printf("Choices\n");
		printf("1. Attack\n");
		printf("2. Run\n");
		
		inp = kbhit();
		if(inp == '1'){
			enemyhealth -= 20;
			health -= 20;
			if(health <= 0){
				printf("%s kalah hiya3x\n", nama);
				sleep(1);
				system("clear");
				exit(-1);
			}

			if(enemyhealth <= 0){
				printf("%s menang\n", nama);
				sleep(1);
				system("clear");
				status = 1;
				
				standbymode();
			}
		}		
		else if(inp == '2'){
			printf("kaboor\n");
			sleep(1);
			system("clear");
			
			standbymode();
		}
		else{
			printf("action invalid\n");
		}
		system("clear");
	}
}

void shopmode(){
	while(1){
		system("clear");
		printf("Shop Mode\n");
		printf("Shop food stock : %d\n", *stokmakantoko);
		printf("Your food stock : %d\n", stokmakan);
		printf("Choices\n");
		printf("1. Buy\n");
		printf("2. Back\n");
		
		inp = kbhit();
		if(inp == '1'){
			if(*stokmakantoko > 0) {
				printf("Transaksi berhasil\n");
				*stokmakantoko = *stokmakantoko - 1;
				stokmakan++;
			}
			else printf("Transaksi gagal\n");
		}		
		else if(inp == '2'){
			printf("kembali\n");
			sleep(1);
			system("clear");
			status = 1;
			
			standbymode();
		}
		else{
			printf("action invalid\n");
		}
		system("clear");
	}
}

void* eat(void *arg){
	while(1){
		while(status == 0){}
		hunger -= 5;
		sleep(3);
		//sleep(10);
	}
}

void* bath(void *arg){
	while(1){
		while(status == 0){}
		hygiene -= 10;
		sleep(6);
		//sleep(30);
	}
}

void* regen(void *arg){
	while(1){
		while(status == 0){}
		health += 5;
		sleep(3);
		//sleep(10);
	}
}



int main(){
	//get key press in c
	//char temp[100];
	printf("Beri nama : ");
	scanf("%s", nama);
	
	printf("%s\n", nama);
	
	key_t key = 1234;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
	stokmakantoko = shmat(shmid, NULL, 0);

	pthread_create(&(tid[0]), NULL, &eat, NULL);
	pthread_create(&(tid[1]), NULL, &bath, NULL);
	pthread_create(&(tid[2]), NULL, &regen, NULL);
	pthread_create(&(tid[3]), NULL, &basetime, NULL);
	
	standbymode();
	
	return 0;
	
}
