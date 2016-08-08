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

struct parameters
{
	int param_screenshot_cnt;
//	int * param_client_sockfd;
	int param_server_sockfd;
	sockaddr_in * param_sockaddr;
	socklen_t * param_socklen;
};

void * thread_function(void * arg) {
	struct parameters * args = (struct parameters *) arg;
//	int * client_sockfd = args -> param_client_sockfd;
	int screenshot_cnt = args -> param_screenshot_cnt;
	sockaddr_in * client_address = args -> param_sockaddr;
	socklen_t * client_len = args -> param_socklen;
	int server_sockfd = args -> param_server_sockfd;
	int client_sockfd_int;

	while(1) {
//		int client_len = sizeof(client_address);
		client_sockfd_int = accept(server_sockfd, (struct sockaddr*)client_address, (socklen_t *)client_len);
		if(client_sockfd_int < 0){
			printf("client accept failed!\n");
			close(client_sockfd_int);
			exit(0);
		}

		int cnt = 0;
		char sendMsg[BUFFER_SIZE];
		char recvMsg[BUFFER_SIZE];
		int RET = 0;
		sprintf(sendMsg , "Connected!\n");
		int * client_sockfd = & client_sockfd_int;
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
					recv_file(client_sockfd);
					break;
				}
				case 2: {
	//				screenshot_cnt ++;
					printf("screenshot_cnt is %d\n", screenshot_cnt);
					get_viewer(client_sockfd, (int)screenshot_cnt);
					printf("get_viewer(%d) finished!\n", screenshot_cnt);
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
}

int main() {
////////////
	int server_sockfd[3], client_sockfd[3], client_len[3];
	struct sockaddr_in server_address[3], client_address[3];
	linger lin[3];
	int server_len;
	int res[3];
	pthread_t p_thread[3];
	for(int i = 0; i < 3; i ++) {
		server_sockfd[i] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(server_sockfd[i] < 0){
			printf("create socket[%d] error!\n", i);
			return 1;
		}
		server_address[i].sin_family = AF_INET;
		server_address[i].sin_addr.s_addr = INADDR_ANY;
		server_address[i].sin_port = htons(SERVER_PORT + 11 * i);
		
		//设置close()后无需TIME_WAIT
		lin[i].l_onoff = 1;
		lin[i].l_linger = 0;
		if(setsockopt(server_sockfd[i], SOL_SOCKET, SO_LINGER, (const char*)&lin[i], sizeof(linger)) == -1)
			printf("set_so_linger[%d] error: %s\n" , i, strerror(errno));
		
		server_len = sizeof(server_address[i]);
		if(bind(server_sockfd[i], (struct sockaddr*)&server_address[i], server_len) < 0){
			printf("bind socket[%d] error!:%s\n", i, strerror(errno));
			close(server_sockfd[i]);
			return 1;
		}
		listen(server_sockfd[i], /*MAX_CONNECTION*/1);
	}
////////////
	printf("server waiting...\n");
	struct parameters args[3];
	for(int i = 0; i < 3; i ++) {
		client_len[i] = sizeof(client_address[i]);
		args[i].param_screenshot_cnt = i;
		args[i].param_server_sockfd = server_sockfd[i];
//		args[i].param_client_sockfd = &client_sockfd;
		args[i].param_sockaddr = & client_address[i];
		args[i].param_socklen = (socklen_t *) & client_len[i];

		res[i] = pthread_create(&p_thread[i], NULL, thread_function, (void *) & args[i]);
		if(res[i] != 0){
			printf("thread create error!\n");
			continue;
		} else{
//			screenshot_cnt ++;
			printf("thread create successfully!\n");
		}
	}
//	volatile int screenshot_cnt = 0;
/*	while(1) {
		int client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_address,(socklen_t *)&client_len);
		if(client_sockfd < 0){
			printf("client accept failed!\n");
			close(client_sockfd);
			return 1;
		}

//		void * thread_result;
		struct parameters args;
		args.param_screenshot_cnt = screenshot_cnt;
		args.param_client_sockfd = &client_sockfd;
		res = pthread_create(&p_thread, NULL, thread_function, (void *) & args);
		if(res != 0){
			printf("thread create error!\n");
			continue;
		} else{
			screenshot_cnt ++;
			printf("thread create successfully!\n");
		}
	}
*/	
	while(1) {}
	return 0;
}
