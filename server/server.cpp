#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "server.h"

void * thread_function(void * arg) {
    int* client_sockfd = (int*)arg;
    char sendMsg[BUFFER_SIZE];
    char recvMsg[BUFFER_SIZE];
    int RET = 0;
    sprintf(sendMsg , "Connected!\n");
    send(*client_sockfd , sendMsg , sizeof(sendMsg), 0);

    while(1) {
        int option[OPERATION_NUMBER] = {1 , 2 , 3};
        sprintf(sendMsg , "\nNow you can do next operations:\n%d. upload osg file\n%d. getViewer\n%d. Quit\nYou want to : " ,
                option[0] , option[1] , option[2]);
        send(*client_sockfd , sendMsg , sizeof(sendMsg), 0);
        RET = recv(*client_sockfd , recvMsg , sizeof(recvMsg) , 0);
        if(RET <= 0) {
            printf("\nConnection error!\n");
            break;
        }
        int op;
        memcpy(&op , recvMsg , sizeof(op));

        switch(op) {
            case 1: {
                recv_File(client_sockfd);
                break;
            }
            case 2: {
            	get_Viewer(client_sockfd);
                printf("get_Viewer() finished!\n");
                break;
            }
            default : {
                break;
            }
        }
        if(op != 1 && op != 2) {
            sprintf(sendMsg , "Thank you for your use!\n");
            send(*client_sockfd , sendMsg , sizeof(sendMsg) , 0);
            printf("\n The client has left...\n");
        }
    }

    close(*client_sockfd);
    printf("close client socket!\n");
    printf("exit process thread!\n");
    printf("server waiting...\n");
    pthread_exit((void*)"thread exit");

}

int main() {

    int server_sockfd, client_sockfd;
    struct sockaddr_in server_address, client_address;

    server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(server_sockfd < 0){
        printf("create socket error!\n");
        return 1;
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);
	 
    //设置close()后无需TIME_WAIT
	linger lin;
	lin.l_onoff = 1;
	lin.l_linger = 0;
	if(setsockopt(server_sockfd,SOL_SOCKET,SO_LINGER,(const char*)&lin,sizeof(linger)) == -1)
		printf("set_so_linger error: %s\n" , strerror(errno));
	
    int server_len = sizeof(server_address);
    if(bind(server_sockfd, (struct sockaddr*)&server_address, server_len) < 0){
        printf("bind socket error!:%s\n",strerror(errno));
        close(server_sockfd);
        return 1;
    }

    listen(server_sockfd, MAX_CONNECTION);

    printf("server waiting...\n");
    while(1) {
        int client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_address,(socklen_t *)&client_len);
        if(client_sockfd < 0){
            printf("client accept failed!\n");
            close(client_sockfd);
            return 1;
        }

        int res;
        pthread_t p_thread;
        void * thread_result;
        res = pthread_create(&p_thread, NULL, thread_function, (void*)&client_sockfd);
        if(res != 0){
            printf("thread create error!\n");
            continue;
        } else{
            printf("thread create successfully!\n");
        }
    }
    return 0;
}
