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
using namespace std;


PickHandler::PickHandler(int *sockfd) {

	client_sockfd = sockfd;
}
void PickHandler::getMessage()
{
	//for(int i=0;i<3;i++)
	//	cout<<pickArray[i]<<"  ";
	//cout<<pickArray[3]<<endl;
	char recvMsg[BUFFER_SIZE];
	send(*client_sockfd , (char *)&pickArray , sizeof(pickArray) , 0);
	recv(*client_sockfd , recvMsg , sizeof(recvMsg) , 0);
	printf("%s\n" , recvMsg);

}
bool PickHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) 
{ 
		osgViewer::Viewer *viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		if(!viewer)
			return false;
		switch(ea.getEventType()) 
		{ 
				
			case(osgGA::GUIEventAdapter::KEYDOWN):
			{
					pickArray[0]=osgGA::GUIEventAdapter::KEYDOWN;
					pickArray[1]=ea.getKey();
					pickArray[2]=(ea.getXmin()+ea.getXmax())*0.5;
				    pickArray[3]=(ea.getYmin()+ea.getYmax())*0.5;
					getMessage();
				return false;
				break;
			}
			case(osgGA::GUIEventAdapter::KEYUP):
			{
					pickArray[0]=osgGA::GUIEventAdapter::KEYUP;
					pickArray[1]=ea.getKey();
					pickArray[2]=(ea.getXmin()+ea.getXmax())*0.5;
				    pickArray[3]=(ea.getYmin()+ea.getYmax())*0.5;
					getMessage();
				return false;
				break;
			}
			case(osgGA::GUIEventAdapter::RESIZE):
			{
				pickArray[0]=ea.getEventType();
				pickArray[1]=0;
				pickArray[2]=ea.getWindowWidth();
				pickArray[3]=ea.getWindowHeight();
				getMessage();
				return true; 
				break;
			}
			case(osgGA::GUIEventAdapter::CLOSE_WINDOW):
			{
				pickArray[0]=ea.getEventType();
				pickArray[1]=0;
				pickArray[2]=0;
				pickArray[3]=0;
				getMessage();
				return true; 
				break;
			}
			case(osgGA::GUIEventAdapter::QUIT_APPLICATION):
			{
				pickArray[0]=ea.getEventType();
				pickArray[1]=0;
				pickArray[2]=0;
				pickArray[3]=0;
				getMessage();
				return true; 
				break;
			}
			case(osgGA::GUIEventAdapter::FRAME):
			{
				return true; 
				break;
			}
			
			default :
			{
				pickArray[0]=ea.getEventType();
				pickArray[1]=ea.getButton();
				pickArray[2]=ea.getX();
				pickArray[3]=ea.getY(); 
				getMessage();
				return true; 
				break;
			}
		} 
		return false;  
} 

void * data_thread_function(void *arg) {
	int data_sockfd, client_data_sockfd;
  	struct sockaddr_in data_address, server_address;
  	client_data_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  	if(client_data_sockfd < 0){
    	printf("create data socket error!\n");
    	return NULL;
  	}
  	data_address.sin_family = AF_INET;
  	data_address.sin_addr.s_addr = INADDR_ANY;
  	data_address.sin_port = htons(CLIENT_DATA_PORT);
	
	//设置close()后无需TIME_WAIT
	
	linger lin;
	lin.l_onoff = 1;
	lin.l_linger = 0;
	if(setsockopt(client_data_sockfd,SOL_SOCKET,SO_LINGER,(const char*)&lin,sizeof(linger)) == -1)
		printf("set_so_linger error: %s\n" , strerror(errno));
	
  	int data_len = sizeof(data_address);
  	if(bind(client_data_sockfd, (struct sockaddr*)&data_address, data_len) < 0){
    	printf("bind socket error!:%s\n",strerror(errno));
    	close(client_data_sockfd);
    	return NULL;
  	}

  	listen(client_data_sockfd, MAX_CONNECTION);

  	int server_len = sizeof(server_address);
  	data_sockfd = accept(client_data_sockfd, (struct sockaddr*)&server_address,(socklen_t *)&server_len); 
  	if(data_sockfd < 0){
    	printf("accept server data error!\n");
    	close(data_sockfd);
    	pthread_exit((void*)"thread exit");
    	return NULL;
  	}
    char recvMsg[BUFFER_SIZE];
  	int * data_sockfd_tmp = (int *) &data_sockfd;
    int cnt = 0;
    while(1) {
        cnt ++;
  		recv_Image(data_sockfd_tmp);
        recv(*data_sockfd_tmp , recvMsg , sizeof(recvMsg) , 0);
       // printf("%d: %s\n" , cnt, recvMsg);
        send(*data_sockfd_tmp , "continue" , sizeof(recvMsg) , 0);
        if(strcmp(recvMsg , "close") == 0) 
            break;
  	}
    printf("recv %d images\n" , cnt);
    close(*data_sockfd_tmp);
    close(client_data_sockfd);
    printf("close client data socket!\n");
    printf("exit process thread!\n");
    pthread_exit((void*)"thread exit");
    //服务器停止send后关闭线程

}

int get_Viewer(int *client_sockfd) {
	
	//创建新的data的线程
    int res;
  	pthread_t d_thread;
  	void* thread_result;
  	res = pthread_create(&d_thread, NULL, data_thread_function, /*(void*)*/NULL);
  	if(res != 0){
    	printf("data thread create error!\n");
    	return -1;
  	}else {
    	printf("create data thread success!\n");
    	send(*client_sockfd , "create data thread success" , 111 , 0);
  	}
  	
    osgViewer::Viewer viewer; 
    osg::ref_ptr<osg::Group> root = new osg::Group(); 
    root ->addChild(osgDB::readNodeFile("/home/paradiser/download/OpenSceneGraph-Data/cow.osg")); 
    viewer.setSceneData(root.get ());
    viewer.addEventHandler(new PickHandler(client_sockfd)) ; 
    viewer.realize(); 
    viewer.run(); 
  	return 0;
}
