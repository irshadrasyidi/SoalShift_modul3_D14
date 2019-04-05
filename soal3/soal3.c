#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int wakeup, spirit;
int f_agmal, f_iraj;
int dis_agmal, dis_iraj;

pthread_t tid[4];

int waktu = 0;
int mulai_dis_agmal = 0;
int mulai_dis_iraj = 0;

void* agmal_bangun(void *arg){
	while(1){
		if(f_iraj >= 3){
			mulai_dis_agmal = waktu;
			dis_agmal = 1;
			f_iraj = 0;
		}
	}
}

void* iraj_tidur(void *arg){
	while(1){
		if(f_agmal >= 3){
			mulai_dis_iraj = waktu;
			dis_iraj = 1;
			f_agmal = 0;
		}
	}
}

void* basetime(void *arg){
	while(1){
		sleep(1);
		waktu++;
		//printf("%d\n", waktu);
	}
}

int main(){
	wakeup = 0;
	spirit = 100;
	
	f_agmal = 0;
	f_iraj = 0;
	
	dis_agmal = 0;
	dis_iraj = 0;
	
	char action[100];
	strcpy(action, "");
	
	pthread_create(&(tid[0]), NULL, &agmal_bangun, NULL);
	pthread_create(&(tid[1]), NULL, &iraj_tidur, NULL);
	pthread_create(&(tid[2]), NULL, &basetime, NULL);
	
	printf("masukkan 'b' untuk 'Agmal Ayo Bangun'\n");
	printf("masukkan 't' untuk 'Iraj Ayo Tidur'\n");
	printf("masukkan 's' untuk 'All Status'\n");
	
	while(1){
		if(wakeup >= 100){
			printf("Agmal Terbangung,mereka bangun pagi dan berolahraga\n");
			exit(0);
		}
		if(spirit <= 0){
			printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
			exit(0);
		}
		//printf("Agmal WakeUp_Status = %d\n", wakeup);
		//printf("Iraj Spirit_Status = %d\n", spirit);
		//printf("f_agmal = %d\n", f_agmal);
		//printf("f_iraj = %d\n", f_iraj);
		
		scanf("%[^'\n']s", action);
		if(!strcmp(action, "s")){
			//printf("all status\n");
			printf("Agmal WakeUp_Status = %d\n", wakeup);
			printf("Iraj Spirit_Status = %d\n", spirit);
			//printf("f_agmal = %d\n", f_agmal);
			//printf("f_iraj = %d\n", f_iraj);
			//printf("mulai_dis_agmal = %d\n", mulai_dis_agmal);
			//printf("mulai_dis_iraj = %d\n", mulai_dis_iraj);
			//printf("waktu = %d\n", waktu);
		}
		
		if(!strcmp(action, "b")){
			if(waktu - mulai_dis_agmal <= 10 && mulai_dis_agmal != 0){
				while(waktu - mulai_dis_agmal <= 10){
					printf("Agmal Ayo Bangun disabled 10 s\n");
					sleep(1);
				}
				dis_agmal = 0;
				//continue;
			}
			else if(dis_agmal == 0){
				f_agmal++;
				wakeup += 15;
			}
		}
		
		if(!strcmp(action, "t")){
			if(waktu - mulai_dis_iraj <= 10 && mulai_dis_iraj != 0){
				while(waktu - mulai_dis_agmal <= 10){
					printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
					sleep(1);
				}
				dis_iraj = 0;
				//continue;
			}
			else if(dis_iraj == 0){
				f_iraj++;
				spirit -= 20;
			}
		}
		getchar();
	}
	
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	
	return 0;
}
