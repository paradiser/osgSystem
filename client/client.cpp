#include "client.h"

int main() {
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
    //    printf("bind client socket error!\n");
    //    return 1;
    //}

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = inet_addr("192.168.0.137");
	
	//设置close()后无需TIME_WAIT
	bool bDontLinger = false;
	//setsockopt(client_sockfd,SOL_SOCKET,SO_DONTLINGER,(const char*)&bDontLinger,sizeof(bool));
	
    if(connect(client_sockfd, (struct sockaddr*)&server_address, sizeof(struct sockaddr_in)) < 0){
        printf("connect error: %d, %s\n", errno, strerror(errno));
        close(client_sockfd);
        return 1;
    }

    char recvMsg[BUFFER_SIZE];
    char replyMsg[BUFFER_SIZE];
    char osg_file_path[BUFFER_SIZE]; //osg文件路径

    recv(client_sockfd , recvMsg , sizeof(recvMsg) , 0);
    printf("%s\n", recvMsg);
    while(1) {
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
            recv(client_sockfd , recvMsg , sizeof(recvMsg) , 0);
            printf("%s", recvMsg);
            break;
        }
    }
    printf("End linking...\n");
	return 0;
}
