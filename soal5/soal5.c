#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

pthread_t tid[100];

struct hewan{
	char nama[100];
	int hunger;
	int hygiene;
	int health;
};

struct hewan monster;

int stokmakan;
int stokmakantoko;
int cooldownbath;
int enemyhealth;

char inp;

int st = 0;

void standbymode(){
	while(1){
		st = 0;
		printf("Standby Mode\n");
		printf("Health : %d\n", monster.health);
		printf("Hunger : %d\n", monster.hunger);
		printf("Hygiene : %d\n", monster.hygiene);
		printf("Food left : %d\n", stokmakan);
		printf("Bath will be ready in %d\n", cooldownbath);
		printf("Choices\n");
		printf("1. Eat\n");
		printf("2. Bath\n");
		printf("3. Battle\n");
		printf("4. Shop\n");
		printf("5. Exit\n");
		
		
		scanf("%c", &inp);
		//~ if(inp == '1'){
			//~ printf("eat\n");
			//~ st = 1;
		//~ }
		//~ else if(inp == '2'){
			//~ printf("eat\n");
			//~ st = 1;
		//~ }
		//~ else if(inp == '3'){
			//~ printf("eat\n");
			
		//~ }
		//~ else if(inp == 4){
			//~ printf("eat\n");
			
		//~ }
		//~ else if(inp == 5){
			//~ printf("eat\n");
			
		//~ }
		sleep(1);
		system("clear");
	}
	
}

void battlemode(){
	while(1){
		printf("Battle Mode\n");
		printf("Monster's Health : %d\n", monster.health);
		printf("Enemy's Health : %d\n", enemyhealth);
		printf("Choices\n");
		printf("1. Attack\n");
		printf("2. Run\n");
	}
}

void shopmode(){
	while(1){
		printf("Shop Mode\n");
		printf("Shop food stock : %d\n", stokmakantoko);
		printf("Your food stock : %d\n", stokmakan);
		printf("Choices\n");
		printf("1. Buy\n");
		printf("2. Back\n");
	}
}

void* eat(void *arg){
	while(1){
		while(st != 0){}
		monster.hunger -= 5;
		sleep(3);
	}
}

void* bath(void *arg){
	while(1){
		while(st != 0){}
		monster.hygiene -= 10;
		sleep(6);
	}
}

void* regen(void *arg){
	while(1){
		while(st != 0){}
		monster.health += 5;
		sleep(3);
	}
}



int main(){
	monster.health = 295;
	monster.hunger = 205;
	monster.hygiene = 110;
	
	
	//char temp[100];
	printf("Beri nama : ");
	scanf("%s", monster.nama);
	
	printf("%s\n", monster.nama);

	pthread_create(&(tid[0]), NULL, &eat, NULL);
	pthread_create(&(tid[1]), NULL, &bath, NULL);
	pthread_create(&(tid[2]), NULL, &regen, NULL);
	
	while(1){
		st = 0;
		printf("Standby Mode\n");
		printf("Health : %d\n", monster.health);
		printf("Hunger : %d\n", monster.hunger);
		printf("Hygiene : %d\n", monster.hygiene);
		printf("Food left : %d\n", stokmakan);
		printf("Bath will be ready in %d\n", cooldownbath);
		printf("Choices\n");
		printf("1. Eat\n");
		printf("2. Bath\n");
		printf("3. Battle\n");
		printf("4. Shop\n");
		printf("5. Exit\n");
		
		sleep(1);
		system("clear");
		//scanf("%c", &inp);
		if(inp == '1'){
			printf("eat\n");
			st = 1;
		}
		//~ else if(inp == '2'){
			//~ printf("eat\n");
			//~ st = 1;
		//~ }
		//~ else if(inp == '3'){
			//~ printf("eat\n");
			
		//~ }
		//~ else if(inp == 4){
			//~ printf("eat\n");
			
		//~ }
		//~ else if(inp == 5){
			//~ printf("eat\n");
			
		//~ }
		
	}

	
}
