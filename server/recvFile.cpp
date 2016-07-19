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


char recv_osg_file[BUFFER_SIZE_FILE]; //osg文件大小

int recv_File(int *client_sockfd) {
    char sendMsg[BUFFER_SIZE];
    char recvMsg[BUFFER_SIZE];
    int RET = 0;

    sprintf(sendMsg , "Your osg file path is : \n");
    send(*client_sockfd , sendMsg , sizeof(sendMsg) , 0);

    RET = recv(*client_sockfd , recvMsg , sizeof(recvMsg) , 0);
    send(*client_sockfd , "get lSize" , sizeof(sendMsg) , 0);
    //如果客户端读取文件失败，则重新提供指令操作
    if(strcmp(recvMsg , "failed") == 0)
        return -1;
    if(RET <= 0) {
        printf("\nConnection error!\n");
        return -1;
    }
    //获取接收osg文件的size和传送次数
    int lSize;
    memcpy(&lSize , recvMsg , sizeof(lSize));
    int cnt = lSize / BYTES_PER_TRANS;

    FILE* fp = fopen(osg_file_name, "w");
    if(NULL == fp) {
        printf("File:\t%s Can Not Open To Write\n", osg_file_name);
        return -1;
    }
    printf("lSize: %d  BYTES_PER_SEND: %d\n" , lSize , BYTES_PER_TRANS);
    //连续recv，直到recv完整数据
    int INDEX = 0; 
    //0 ~ cnt - 1
    for(int i=0; i<cnt; i++) {
        RET = 0;
        while(RET < BYTES_PER_TRANS) {
            RET += recv(*client_sockfd , recv_osg_file + RET , sizeof(recv_osg_file) , 0); 
            printf("count: %d  RET: %d\n" , i + 1 , RET);
        }
        if(RET <= 0) {
            printf("\nConnection error!\n");
            return -1;
        }
        //接收osg文件并写入本地
        printf("count: %d  recvSize: %d\n" , i + 1 , RET);
        if(fwrite(recv_osg_file, sizeof(char), RET, fp) < RET){
            printf("File: %s write failed", osg_file_name);
        }
        INDEX += RET;
        sprintf(sendMsg , "Received");
        send(*client_sockfd , sendMsg , sizeof(sendMsg) , 0);
    }
    // cnt
    RET = 0;
    while(RET < lSize - cnt * BYTES_PER_TRANS) {
        RET += recv(*client_sockfd , recv_osg_file + RET , sizeof(recv_osg_file) , 0); 
        printf("count: %d  RET: %d\n" , cnt + 1 , RET);
    }
    if(RET <= 0) {
        printf("\nConnection error!\n");
        return -1;
    }
    //接收osg文件并写入本地
    printf("count: %d  recvSize: %d\n\n" , cnt + 1 , RET);
    if(fwrite(recv_osg_file, sizeof(char), RET, fp) < RET){
        printf("File: %s write failed\n", osg_file_name);
    }
    fclose(fp);
    //printf("data: \n%s\n" , recv_osg_file);
    sprintf(sendMsg , "Server has received your osg file!\n");
    send(*client_sockfd , sendMsg , sizeof(sendMsg) , 0);
    return 0;
}
