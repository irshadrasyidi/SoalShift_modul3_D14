#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>

#define PORT 8088

pthread_t tid;
int *stok;

void* beli(void* sock){
	int new_socket = *(int*)sock;
	char buffer[1024] = {0};
	int valread;
	
	char buy[50];
	
	while((valread = read(new_socket ,buffer, 1024)) > 0){
		if(!strcmp(buffer, "beli")){
			if(*stok <= 0){
				strcpy(buy, "transaksi gagal");
				send(new_socket, buy, strlen(buy), 0);
					
			}
			else{	
				*stok -= 1;
				strcpy(buy, "transaksi berhasil");
				send(new_socket, buy, strlen(buy), 0);
			}
		}

		memset(buy, 0, 50);
		memset(buffer, 0, 1024);
   	}
}


int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    
    char buy[50];
    
    //shared memory
    key_t key = 1234;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    stok = shmat(shmid, NULL, 0);
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }


	int cekkon = 0;
	new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
	while(1) {
		if(new_socket < 0){
			return 0;
		}
		else if(new_socket > 0 && cekkon == 0){
			cekkon = 1;
			pthread_create(&tid, NULL, &beli, (void*)&new_socket);
			
		}
	}
	
	return 0;
}
