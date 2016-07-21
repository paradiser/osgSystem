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
#include "client.h"



char recv_osg_image[BUFFER_SIZE_FILE]; //osgImage大小

int recv_Image(int *client_sockfd , int CNT) {
	
	char osg_image_name[BUFFER_SIZE] = "/home/paradiser/desktop/recvImage/recv_image";
	int t = CNT % 1001;
    char a[BUFFER_SIZE];
    memset(a , 0 , sizeof(a));
    int idx = 0;
    while(t > 0) {
    	int tmp = t % 10;
    	a[idx++] = tmp + '0';
    	t = t / 10;
    }
    strcat(osg_image_name , a);
	//printf("%s\n" , osg_image_name);
    char sendMsg[BUFFER_SIZE];
    char recvMsg[BUFFER_SIZE];
    int RET = 0;

    RET = recv(*client_sockfd , recvMsg , sizeof(recvMsg) , 0);
    send(*client_sockfd , "get lsize" , sizeof(sendMsg) , 0);
    //如果服务器读取文件失败
    if(strcmp(recvMsg , "failed") == 0)
        return -1;
    if(RET <= 0) {
        printf("\nConnection error!\n");
        return -1;
    }
    //获取接收osgImage的size和传送次数
    int lSize;
    memcpy(&lSize , recvMsg , sizeof(lSize));
    //RET = recv(*client_sockfd , recvMsg , sizeof(int) , 0);
    int cnt = lSize / BYTES_PER_TRANS;
    //memcpy(&cnt , recvMsg , sizeof(cnt));
    printf("lSize: %d  BYTES_PER_SEND: %d\n" , lSize , BYTES_PER_TRANS);
	printf("CNT: %d\n" , cnt);
    FILE* fp = fopen(osg_image_name, "w");
    if(NULL == fp) {
        printf("File:\t%s Can Not Open To Write\n", osg_image_name);
        return -1;
    }
    
    //连续recv，直到recv完整数据
    int INDEX = 0; 
    //0 ~ cnt - 1
    for(int i=0; i<cnt; i++) {
        RET = 0;
        while(RET < BYTES_PER_TRANS) {
            RET += recv(*client_sockfd , recv_osg_image + RET , sizeof(recv_osg_image) , 0); 
            printf("count: %d  RET: %d\n" , i + 1 , RET);
        }
        if(RET <= 0) {
            printf("\nConnection error!\n");
            return -1;
        }
        //接收osgImage并写入本地
        printf("count: %d  recvSize: %d\n" , i + 1 , RET);
        
        if(fwrite(recv_osg_image, sizeof(char), RET, fp) < RET){
            printf("File: %s write failed", recv_osg_image);
        }
        INDEX += RET;
        sprintf(sendMsg , "Received");
        send(*client_sockfd , sendMsg , sizeof(sendMsg) , 0);
    }
    // cnt
    RET = 0;
    while(RET < lSize - cnt * BYTES_PER_TRANS) {
        RET += recv(*client_sockfd , recv_osg_image + RET , sizeof(recv_osg_image) , 0); 
        printf("count: %d  RET: %d\n" , cnt + 1 , RET);
    }
    if(RET <= 0) {
        printf("\nConnection error!\n");
        return -1;
    }
    //接收osgImage并写入本地
    printf("count: %d  recvSize: %d\n\n" , cnt + 1 , RET);
    if(fwrite(recv_osg_image, sizeof(char), RET, fp) < RET){
            printf("File: %s write failed", recv_osg_image);
        }
    fclose(fp);
    //printf("data: \n%s\n" , recv_osg_file);
    sprintf(sendMsg , "Client has received your osg image!\n");
    send(*client_sockfd , sendMsg , sizeof(sendMsg) , 0);
    return 0;
}
