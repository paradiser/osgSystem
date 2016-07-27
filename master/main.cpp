#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "master.h"


class getServInfo {
public:
    getServInfo(int server_num , char server_ip[][BUFFER_SIZE]) {
        serverinfo(server_num , server_ip);
    }
protected:
    serverInfo serverinfo; //实例一个保存所有服务器信息的类
}

int get_All_Server_Info() {
    //数据库中读取服务器节点信息
    //
    getServInfo  getservinfo( , );
}

char *load_Balance() {
    int conn = MAX_CONNECTION + 1;
    int num = getservinfo.serverinfo.getServerNum();
    int id = -1;
    for(int i=0; i<num; i++) {
    	if(conn < getservinfo.serverinfo.getServerCons(i)) {
    		conn = getservinfo.serverinfo.getServerCons(i);
    		id = i;
    	}
    }
    if(id == -1) return NULL;
   	return getservinfo.serverinfo.getServerIp(id);
}

void * thread_function(void * arg) {
    int* client_sockfd = (int*)arg;
    char sendMsg[BUFFER_SIZE];
    char recvMsg[BUFFER_SIZE];
    int RET = 0;
    //负载均衡 为客户端选择最佳服务器
    /*...*/
    char *dest_ip = load_Balance();
    strcpy(sendMsg , dest_ip);
    //传送匹配的服务器地址
    send(* client_sockfd , sendMsg , sizeof(sendMsg) , 0);
    recv(* client_sockfd , recvMsg , sizeof(recvMsg) , 0);
    close(*client_sockfd);
    printf("close client socket!\n");
    printf("exit process thread!\n");
    printf("server waiting...\n");
    pthread_exit((void*)"thread exit");

}

int main() {

    //获取所有服务器节点信息
    if(get_All_Server_Info() == 0) {
        printf("get serverinfo error!\n");
        return -1;
    }

    int master_sockfd, client_sockfd;
    struct sockaddr_in master_address, client_address;

    master_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(master_sockfd < 0){
        printf("create socket error!\n");
        return -1;
    }
    master_address.sin_family = AF_INET;
    master_address.sin_addr.s_addr = INADDR_ANY;
    master_address.sin_port = htons(MASTER_PORT);
     
    //设置close()后无需TIME_WAIT
    linger lin;
    lin.l_onoff = 1;
    lin.l_linger = 0;
    if(setsockopt(master_sockfd,SOL_SOCKET,SO_LINGER,(const char*)&lin,sizeof(linger)) == -1)
        printf("set_so_linger error: %s\n" , strerror(errno));
    
    int master_len = sizeof(master_address);
    if(bind(master_sockfd, (struct sockaddr*)&master_address, master_len) < 0){
        printf("bind socket error!:%s\n",strerror(errno));
        close(master_sockfd);
        return -1;
    }

    listen(master_sockfd, MAX_CONNECTION);

    printf("master waiting...\n");
    while(1) {
        int client_len = sizeof(client_address);
        client_sockfd = accept(master_sockfd, (struct sockaddr*)&client_address,(socklen_t *)&client_len);
        if(client_sockfd < 0){
            printf("client accept failed!\n");
            close(client_sockfd);
            return -1;
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
