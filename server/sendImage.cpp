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

#define osg_image_path "/home/paradiser/download/OpenSceneGraph-Data/Images/blueFlowers.png"

char osg_image[BUFFER_SIZE_FILE]; //osgImage大小

int send_Image(int *client_sockfd) {
    char recvMsg[BUFFER_SIZE];
    char replyMsg[BUFFER_SIZE];
    //char osg_image_path[BUFFER_SIZE]; //osgImage路径
    //recv(client_sockfd , recvMsg , sizeof(recvMsg) , 0);
    //printf("%s", recvMsg);
    //scanf("%s" , osg_file_path);

    //读入osgImage
    FILE* fp = fopen(osg_image_path, "r");
    if(fp == NULL) {
        printf("Can't Open image %s\n" , osg_image_path);
        sprintf(replyMsg , "failed");
        send(*client_sockfd , replyMsg , sizeof(replyMsg) , 0);
        recv(*client_sockfd , recvMsg , sizeof(recvMsg) , 0);
        return -1;
    }
    //获取文件大小
    fseek (fp , 0 , SEEK_END);  
    int lSize = ftell (fp);  
    rewind (fp);
    printf("Bytes of The osg image: %d\n" , lSize);
    
    //每次传BYTE_PER_TRANS字节，传cnt + 1次
    send(*client_sockfd , (char *)&lSize , sizeof(int) , 0);
    recv(*client_sockfd , recvMsg , sizeof(recvMsg) , 0);
    int cnt = lSize / BYTES_PER_TRANS;
    for(int i=0; i<cnt; i++) {
        fread(osg_image, sizeof(char) , BYTES_PER_TRANS, fp);
        int tmp = send(*client_sockfd , osg_image , BYTES_PER_TRANS , 0);
        printf("count: %d  sendSize: %d\n" , i + 1 , tmp);
        recv(*client_sockfd , recvMsg , sizeof(recvMsg) , 0);
    }
    fread(osg_image, sizeof(char) , lSize - cnt * BYTES_PER_TRANS, fp);
    int tmp = send(*client_sockfd , osg_image , lSize - cnt * BYTES_PER_TRANS , 0);
    printf("count: %d  sendSize: %d\n" , cnt + 1 , tmp);
    
    //fread(osg_file , sizeof(char) , sizeof(char)*lSize , fp);
    //printf("osg_file: %s\n" , osg_file);
    //int tmp = send(client_sockfd , osg_file , sizeof(char)*lSize , 0);
    //printf("sendSize: %d\n" , tmp);

    fclose(fp);
    //send(client_sockfd , osg_file_path , sizeof(osg_file_path) , 0);
    recv(*client_sockfd , recvMsg , sizeof(recvMsg) , 0);
    printf("%s",recvMsg);
    return 0;
}
