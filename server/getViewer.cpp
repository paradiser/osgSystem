#include "server.h"
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

float event_Array[5];
volatile bool capture_screenshot_lock_flag = false;
volatile bool send_image_lock_flag = false;

void PickHandler::getMessage()
{
	for(int i=0;i<4;i++)
		std::cout<<"\t"<<pickArray[i];
	std::cout<<"\t"<<pickArray[4]<<std::endl;
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
					pickArray[2]=ea.getUnmodifiedKey();
				    pickArray[3]=0;
					pickArray[4]=ea.getTime();
					getMessage();
				return false;
				break;
			}
			case(osgGA::GUIEventAdapter::KEYUP):
			{
					pickArray[0]=osgGA::GUIEventAdapter::KEYUP;
					pickArray[1]=ea.getKey();
					pickArray[2]=ea.getUnmodifiedKey();
				    pickArray[3]=0;
					pickArray[4]=ea.getTime();
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
				pickArray[4]=ea.getTime();
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
				pickArray[4]=ea.getTime();
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
				pickArray[4]=ea.getTime();
				getMessage();
				return true; 
				break;
			}
			case(osgGA::GUIEventAdapter::FRAME):
			{
				pickArray[0]=ea.getEventType();
				pickArray[1]=0;
				pickArray[2]=0;
				pickArray[3]=0;
				pickArray[4]=ea.getTime();
				return true; 
				break;
			}
			case(osgGA::GUIEventAdapter::PEN_PRESSURE):
			{
				pickArray[0]=ea.getEventType();
				pickArray[1]=ea.getPenPressure();
				pickArray[2]=0;
				pickArray[3]=0;
				pickArray[4]=ea.getTime();
				return true; 
				break;
			}
			case(osgGA::GUIEventAdapter::PEN_ORIENTATION):
			{
				pickArray[0]=ea.getEventType();
				pickArray[1]=ea.getPenRotation();
				pickArray[2]=ea.getPenTiltX();
				pickArray[3]=ea.getPenTiltY();
				pickArray[4]=ea.getTime();
				return true; 
				break;
			}
			case(osgGA::GUIEventAdapter::PEN_PROXIMITY_ENTER):
			{
				pickArray[0]=ea.getEventType();
				pickArray[1]=ea.getTabletPointerType();
				pickArray[2]=0;
				pickArray[3]=0;
				pickArray[4]=ea.getTime();
				return true; 
				break;
			}
			
			case(osgGA::GUIEventAdapter::PEN_PROXIMITY_LEAVE):
			{
				pickArray[0]=ea.getEventType();
				pickArray[1]=ea.getTabletPointerType();
				pickArray[2]=0;
				pickArray[3]=0;
				pickArray[4]=ea.getTime();
				return true; 
				break;
			}
			case(osgGA::GUIEventAdapter::SCROLL):
			{
				pickArray[0]=ea.getEventType();
				pickArray[1]=ea.getScrollingMotion();
				pickArray[2]=ea.getX();
				pickArray[3]=ea.getY(); 
				pickArray[4]=ea.getTime();
				getMessage();
				return true; 
				break;
			}
			case(osgGA::GUIEventAdapter::DRAG):
			{
				pickArray[0]=ea.getEventType();
				pickArray[1]=ea.getButtonMask();
				pickArray[2]=ea.getX();
				pickArray[3]=ea.getY(); 
				pickArray[4]=ea.getTime();
				getMessage();
				return true; 
				break;
			}
			default :
			{
				pickArray[0]=ea.getEventType();
				pickArray[1]=ea.getButton();
				pickArray[2]=ea.getX();
				pickArray[3]=ea.getY(); 
				pickArray[4]=ea.getTime();
				getMessage();
				return true; 
				break;
			}
		} 
		return false; 
}

float * PickHandler::getPickArray() {
	return pickArray;
}


void * server_data_thread_function(void *arg) {
	int * data_sockfd = (int *) arg;
    int cnt = 0;
    char recvMsg[BUFFER_SIZE];
    while(1) {
        cnt ++;
    	send_Image(data_sockfd);
    	capture_screenshot_lock_flag = true;
    	send_image_lock_flag = false;
    	while(send_image_lock_flag == false) {}
    	
        if(event_Array[1] == ESCAPE_NUM) {
            send(*data_sockfd , "close" , 111 , 0);
            recv(*data_sockfd , recvMsg , sizeof(recvMsg) , 0);
            break;
        } else {
            send(*data_sockfd , "continue" , 111 , 0);
            recv(*data_sockfd , recvMsg , sizeof(recvMsg) , 0);
        }
    }
    printf("send %d images\n" , cnt);
    close(*data_sockfd);
    //关线程
    printf("close server data socket!\n");
    printf("exit process thread!\n");
    pthread_exit((void*)"thread exit");
}

void * create_screenshot_thread_function(void *arg) {
	create_screenshot(event_Array, &capture_screenshot_lock_flag, &send_image_lock_flag);
    //关线程
    printf("exit cs process thread!\n");
    pthread_exit((void*)"thread exit");
}


int get_Viewer(int *client_sockfd) {
	/*
    osgViewer::Viewer viewer; 
    osg::ref_ptr<osg::Group> root = new osg::Group(); 
    root ->addChild(osgDB::readNodeFile(osg_file_name)); 
    viewer.setSceneData(root.get ());
    viewer.addEventHandler(new PickHandler) ; 
    viewer.realize(); 
    viewer.run(); 
    */
	memset(event_Array , -1 ,sizeof(event_Array));
	char recvMsg[BUFFER_SIZE];
	char sendMsg[BUFFER_SIZE];
	
	//接收客户端已创建data线程的响应
	recv(*client_sockfd , recvMsg , sizeof(recvMsg) , 0);
	printf("%s\n" , recvMsg);
    //122 ~ 160创建新的sendImage的线程
	
    int data_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(data_sockfd < 0){
        printf("data socket create error!\n");
        return -1;
    }
    struct sockaddr_in client_data_addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    int res = getpeername(*client_sockfd, (struct sockaddr*)&client_data_addr, &addr_size);
  	if(res != 0){
    	printf("get client socket address error!\n");
    	return -1;
  	}
  	client_data_addr.sin_family = AF_INET;
  	client_data_addr.sin_addr.s_addr = client_data_addr.sin_addr.s_addr;
  	client_data_addr.sin_port = htons(CLIENT_DATA_PORT);
	
	//设置close()后无需TIME_WAIT
	linger lin;
	lin.l_onoff = 1;
	lin.l_linger = 0;
	if(setsockopt(data_sockfd,SOL_SOCKET,SO_LINGER,(const char*)&lin,sizeof(linger)) == -1)
		printf("set_so_linger error: %s\n" , strerror(errno));
	
  	// 连接数据信道
  	if(connect(data_sockfd, (struct sockaddr*)&client_data_addr, sizeof(struct sockaddr_in)) < 0){
    	printf("connect error: %d, %s\n", errno, strerror(errno));
    	close(data_sockfd);
    	return -1;
  	}else {
    	printf("connect client data socket success\n");
  	}

  	pthread_t c_thread;
  	if(pthread_create(&c_thread, NULL, create_screenshot_thread_function, (void *) NULL) != 0){
    	printf("create server data thread error\n");
    	return -1;
  	}else {
    	printf("create server data thread success\n");
  	}

  	pthread_t r_thread;
  	if(pthread_create(&r_thread, NULL, server_data_thread_function, (void *) & data_sockfd) != 0){
    	printf("create server data thread error\n");
    	return -1;
  	}else {
    	printf("create server data thread success\n");
  	}
	
	//向客户端发送已创建server data 线程的消息
	send(*client_sockfd , sendMsg , sizeof(sendMsg) , 0);
  	//获取客户端事件响应
  	int RET = 0;
  	while(1) {
  		RET = recv(*client_sockfd , recvMsg , sizeof(recvMsg) , 0);
  		if(RET <= 0) {
            printf("\nConnection error!\n");
            break;
        }
  		memcpy(&event_Array , recvMsg , sizeof(event_Array));
  		for(int i=0; i<4; i++) {
  			printf("%f " , event_Array[i]);
  		}
  		printf("\n");
  		sprintf(sendMsg , "received data!");
  		send(*client_sockfd , sendMsg , sizeof(sendMsg) , 0);
  		if(event_Array[1] == ESCAPE_NUM) {
//			pthread_cancel(c_thread);//c_thread
//			pthread_cancel(c_thread);
//			printf("wojiaotacancelledanshibuzhidaoyoumeiyouchenggong\n");
  			break;
  		}
  	}
  	return 0;
}
