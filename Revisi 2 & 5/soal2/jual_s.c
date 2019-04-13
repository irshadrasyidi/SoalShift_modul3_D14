#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PORT 9099

int *stok;
pthread_t tid;

void* cetakstok(void *arg){
	while(1){
		printf("stok barang = %d\n", *stok);
		sleep(3);
	}
}

void* jual(void* sock){
	int new_socket = *(int*)sock;
	char buffer[1024] = {0};
	int valread;
	while((valread = read(new_socket ,buffer, 1024)) > 0){
		if(!strcmp(buffer, "tambah")){
			*stok += 1;
		}

		memset(buffer, 0, 1024);
	}
}



int main(int argc, char const *argv[]) {
    int server_fd, new_socket, wkread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    
    char sell[50];

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

	//thread print stok
    pthread_create(&tid, NULL, &cetakstok, NULL);
    
    int cekkon = 0;
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    while (1){
		if(new_socket < 0){
			return 0;
		}
		else if(new_socket > 0 && cekkon == 0){
			cekkon = 1;
			//printf("%d\n", new_socket);
			pthread_create(&tid, NULL, &jual, (void*)&new_socket);
			
		}
    }
    
}
