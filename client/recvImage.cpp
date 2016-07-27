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
#include <sys/file.h>
#include <errno.h>
#include "client.h"
#include "md5.h"

char recv_osg_image[BUFFER_SIZE_FILE]; //osgImage大小


int copyFile(char *name , char *osg_image_name) {
	int c;
	FILE *fpSrc = NULL, *fpDest = NULL;  //定义两个指向文件的指针
	fpSrc = fopen(osg_image_name, "rb+");
	if(fpSrc == NULL){
		printf( "client_recvImage line 26: originalImage open failure.");  //源文件不存在的时候提示错误
		fclose(fpDest);
		fclose(fpSrc); 
		return -1;
	}
	fpDest = fopen(name, "w");  //以写入二进制的方式打开目标文件
	if(fpDest==NULL){
		printf("client_recvImage line 33: recvImage open failure.");
		fclose(fpDest);
		fclose(fpSrc); 
		return -1;
	}
	if(0 == flock(fileno(fpDest), LOCK_EX))
    {
        printf("client_recvImage line 40: recvImage locked.\n");
		while((c=fgetc(fpSrc))!=EOF){  //从源文件中读取数据结尾
			fputc(c, fpDest);
		}
		fclose(fpSrc);  //关闭文件指针，释放内存
        flock(fileno(fpDest), LOCK_UN);
		fclose(fpDest);
    	printf("client_recvImage line 47: Copyed!\n");
    }
    else
    {
        printf("client_recvImage line 51: lock failed\n");
		fclose(fpDest);
		fclose(fpSrc);  //关闭文件指针，释放内存
    }
    return 0;
}

int recv_Image(int *client_sockfd , int CNT) {
	
	char sourceMD5[MD5_LEN + 1];
	char fileMD5[MD5_LEN + 1];
	char sendMsg[BUFFER_SIZE];
	char recvMsg[BUFFER_SIZE];
	char cntString[BUFFER_SIZE];
	char name[BUFFER_SIZE] = "../files/recvImage/recv_image";
	char osg_image_name[BUFFER_SIZE] = "../files/originalImage/recv_image";
	int t = CNT % 1001;
	memset(cntString , 0 , sizeof(cntString));
	int idx = 0;
	while(t > 0) {
		int tmp = t % 10;
		cntString[idx++] = tmp + '0';
		t = t / 10;
	}
	strcat(osg_image_name , cntString);
	strcat(osg_image_name , ".png");
	strcat(name , cntString);
	strcat(name , ".png");
	//printf("%s\n" , osg_image_name);
	
	int RET = 0;
	//接收源文件的MD5值
	RET = recv(*client_sockfd , sourceMD5 , sizeof(sourceMD5) , 0);
	send(*client_sockfd , "get sourceMD5" , sizeof(sendMsg) , 0);
	printf("client_recvImage line 82: %s\n" , sourceMD5);
	RET = recv(*client_sockfd , recvMsg , sizeof(recvMsg) , 0);
	send(*client_sockfd , "get file size" , sizeof(sendMsg) , 0);
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
	if(lSize == 0) 
		return -1;
	FILE* fp = fopen(osg_image_name, "w");
	if(NULL == fp) {
		printf("File:\t%s Can Not Open To Write\n", osg_image_name);
		fclose(fp);
		return -1;
	}
	
	//连续recv，直到recv完整数据
	int INDEX = 0; 
	//0 ~ cnt - 1
	for(int i=0; i<cnt; i++) {
		RET = 0;
		while(RET < BYTES_PER_TRANS) {
			RET += recv(*client_sockfd , recv_osg_image + RET , sizeof(recv_osg_image) , 0); 
			//printf("count: %d  RET: %d\n" , i + 1 , RET);
		}
		if(RET <= 0) {
			printf("\nclient_recvImage line 75: recv error!\n");
			fclose(fp);
			return -1;
		}
		//接收osgImage并写入本地
		//printf("count: %d  recvSize: %d\n" , i + 1 , RET);
		
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
		//printf("count: %d  RET: %d\n" , cnt + 1 , RET);
	}
	if(RET <= 0) {
		printf("\nclient_recvImage line 141: Connection error!\n");
		fclose(fp);
		return -1;
	}
	//接收osgImage并写入本地
	//printf("count: %d  recvSize: %d\n\n" , cnt + 1 , RET);
	if(fwrite(recv_osg_image, sizeof(char), RET, fp) < RET){
		printf("File: %s write failed", recv_osg_image);
	}
	fclose(fp);
	//sprintf(sendMsg , "Client has received your osg image!\n");
	send(*client_sockfd , sendMsg , sizeof(sendMsg) , 0);
	//MD5 judge

	if(!CalcFileMD5(osg_image_name, fileMD5))
		puts("Error occured!");
	else
		//printf("Success! MD5 sum is :%s \n", fileMD5);

	if(strcmp(sourceMD5 , fileMD5) != 0) {
		//printf("client_recvImage line 159: MD5 not equal!\n");
		return -1;
	}
	//复制到另一个文件
	if(copyFile(name , osg_image_name) == -1)
		return -1;
	return 0;
}
