#include "client.h"

int main() {

	int client_master_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(client_master_sockfd < 0){
		printf("client_master_socket create error!\n");
		return 1;
	}

	struct sockaddr_in client_master_address;
	client_master_address.sin_family = AF_INET;
	client_master_address.sin_port = htons(MASTER_PORT);
	client_master_address.sin_addr.s_addr = inet_addr(getCsvValue(MASTER_INFO_PATH, 0, 0).c_str());
	
	//设置close()后无需TIME_WAIT
	//bool bDontLinger = false;
	//setsockopt(client_sockfd,SOL_SOCKET,SO_DONTLINGER,(const char*)&bDontLinger,sizeof(bool));

	linger lin_m;
	lin_m.l_onoff = 1;
	lin_m.l_linger = 0;
	if(setsockopt(client_master_sockfd,SOL_SOCKET,SO_LINGER,(const char*)&lin_m,sizeof(linger)) == -1)
		printf("set_so_linger error: %s\n" , strerror(errno));
	
	if(connect(client_master_sockfd, (struct sockaddr*)&client_master_address, sizeof(struct sockaddr_in)) < 0){
		printf("connect error: %d, %s\n", errno, strerror(errno));
		close(client_master_sockfd);
		return 1;
	}

	char recvMsg[BUFFER_SIZE];
	memset(recvMsg, 0, BUFFER_SIZE * sizeof(char));
	string sendMsg_str = "client";
	send(client_master_sockfd, sendMsg_str.c_str(), sendMsg_str.size(), 0);

	recv(client_master_sockfd, recvMsg, sizeof(recvMsg), 0);
	if(strcmp(recvMsg, "get") == 0) {
		string sendMsg_str_ip = getCsvValue(SELF_INFO_PATH, 0, 0);
//		cout << getCsvValue(SELF_INFO_PATH, 0, 0) << endl;
		send(client_master_sockfd, sendMsg_str_ip.c_str(), sendMsg_str_ip.size(), 0);
//		cout << "sizeof sendIp = " << sendMsg_str_ip.size() << endl;
		memset(recvMsg, 0, BUFFER_SIZE * sizeof(char));
		recv(client_master_sockfd, recvMsg, sizeof(recvMsg), 0);
	}
	else {
		printf("error: no reply message received!\n");
		return 1;
	}
	string server_ip(recvMsg);
	cout << "connect to server: " << server_ip << endl;
	if(server_ip == "0") {
		printf("error: no server available!\n");
		return 1;
	}



	int client_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(client_sockfd < 0){
		printf("client socket create error!\n");
		return 1;
	}
	
	//struct sockaddr_in client_address;
	//memset(&client_address, sizeof(client_address),0);
	//client_address.sin_family = AF_INET;
	//client_address.sin_port = htons(CLIENT_PORT);
	//client_address.sin_addr.s_addr = INADDR_ANY;
	//if(bind(client_sockfd, (struct sockaddr*)&client_address, sizeof(client_address))){
	//	printf("bind client socket error!\n");
	//	return 1;
	//}

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVER_PORT);
	server_address.sin_addr.s_addr = inet_addr(server_ip.c_str());
	
	//设置close()后无需TIME_WAIT
	//bool bDontLinger = false;
	//setsockopt(client_sockfd,SOL_SOCKET,SO_DONTLINGER,(const char*)&bDontLinger,sizeof(bool));
 
	linger lin;
	lin.l_onoff = 1;
	lin.l_linger = 0;
	if(setsockopt(client_sockfd,SOL_SOCKET,SO_LINGER,(const char*)&lin,sizeof(linger)) == -1)
		printf("set_so_linger error: %s\n" , strerror(errno));
	
	if(connect(client_sockfd, (struct sockaddr*)&server_address, sizeof(struct sockaddr_in)) < 0){
		printf("connect error: %d, %s\n", errno, strerror(errno));
		close(client_sockfd);
		return 1;
	}

//	char recvMsg[BUFFER_SIZE];
	char replyMsg[BUFFER_SIZE];
	char osg_file_path[BUFFER_SIZE]; //osg文件路径

	memset(recvMsg, 0, BUFFER_SIZE * sizeof(char));
	recv(client_sockfd , recvMsg , sizeof(recvMsg) , 0);
	printf("%s\n", recvMsg);
	while(1) {
		memset(recvMsg, 0, BUFFER_SIZE * sizeof(char));
		recv(client_sockfd , recvMsg , sizeof(recvMsg) , 0);
		printf("%s\n" , recvMsg);
		//send operator
		int op;
		scanf("%d" , &op);
		send(client_sockfd , (char *)&op , sizeof(int) , 0);
		switch(op) {
			case 1: {
				send_File(client_sockfd);
				break;
			}
			case 2: {
				int * client_sockfd_tmp = (int *) &client_sockfd;
				get_Viewer(client_sockfd_tmp);
				break;
			}
			default: {
				break;
			}
		}
		if(op != 1 && op != 2) {
			memset(recvMsg, 0, BUFFER_SIZE * sizeof(char));
			recv(client_sockfd , recvMsg , sizeof(recvMsg) , 0);
			printf("%s", recvMsg);
			break;
		}
	}
	printf("End linking...\n");
	return 0;
}
