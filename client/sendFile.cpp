#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include "client.h"


char osg_file[BUFFER_SIZE_FILE]; //osg文件大小

int send_File(int client_sockfd) {
    char recvMsg[BUFFER_SIZE];
    char replyMsg[BUFFER_SIZE];
    char osg_file_path[BUFFER_SIZE]; //osg文件路径
    recv(client_sockfd , recvMsg , sizeof(recvMsg) , 0);
    printf("%s", recvMsg);
    scanf("%s" , osg_file_path);
    //读入osg文件
    FILE* fp = fopen(osg_file_path, "r");
    if(fp == NULL) {
        printf("Can't Open file %s\n" , osg_file_path);
        sprintf(replyMsg , "failed");
        send(client_sockfd , replyMsg , sizeof(replyMsg) , 0);
        recv(client_sockfd , recvMsg , sizeof(recvMsg) , 0);
        return -1;
    }
    //获取文件大小
    fseek (fp , 0 , SEEK_END);  
    int lSize = ftell (fp);  
    rewind (fp);
    printf("Bytes of The osg file: %d\n" , lSize);
    
    //每次传1000000字节，传cnt + 1次
    send(client_sockfd , (char *)&lSize , sizeof(int) , 0);
    recv(client_sockfd , recvMsg , sizeof(recvMsg) , 0);
    int cnt = lSize / BYTES_PER_TRANS;
    for(int i=0; i<cnt; i++) {
        fread(osg_file, sizeof(char) , BYTES_PER_TRANS, fp);
        int tmp = send(client_sockfd , osg_file , BYTES_PER_TRANS , 0);
        printf("count: %d  sendSize: %d\n" , i + 1 , tmp);
        recv(client_sockfd , recvMsg , sizeof(recvMsg) , 0);
    }
    fread(osg_file, sizeof(char) , lSize - cnt * BYTES_PER_TRANS, fp);
    int tmp = send(client_sockfd , osg_file , lSize - cnt * BYTES_PER_TRANS , 0);
    printf("count: %d  sendSize: %d\n" , cnt + 1 , tmp);
    
    //fread(osg_file , sizeof(char) , sizeof(char)*lSize , fp);
    //printf("osg_file: %s\n" , osg_file);
    //int tmp = send(client_sockfd , osg_file , sizeof(char)*lSize , 0);
    //printf("sendSize: %d\n" , tmp);

    fclose(fp);
    //send(client_sockfd , osg_file_path , sizeof(osg_file_path) , 0);
    recv(client_sockfd , recvMsg , sizeof(recvMsg) , 0);
    printf("%s",recvMsg);
    return 0;
}
