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

typedef struct parameters
{
	int param_screenshot_cnt;
	int * param_data_sockfd;
	float * param_event_Array;
	volatile bool * param_capture_screenshot_lock_flag;
	volatile bool * param_send_image_lock_flag;
} parameters, * parameters_link;

int parameters_init(parameters_link & link) {
	* (link -> param_capture_screenshot_lock_flag) = true;
	* (link -> param_send_image_lock_flag) = false;
	return 0;
}

typedef struct parameters2
{
	int param_screenshot_cnt;
	float * param_event_Array;
	volatile bool * param_capture_screenshot_lock_flag;
	volatile bool * param_send_image_lock_flag;
} parameters2, * parameters2_link;

int parameters2_init(parameters2_link & link) {
	* (link -> param_capture_screenshot_lock_flag) = true;
	* (link -> param_send_image_lock_flag) = false;
	return 0;
}

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
	parameters_link args = (parameters_link) arg;
	printf("arg1_address == %lx\n", arg);
	printf("args1_address == %lx\n", args);
	int * data_sockfd = args -> param_data_sockfd;
	int screenshot_cnt = args -> param_screenshot_cnt;
	float * event_Array = args -> param_event_Array;

	volatile bool * send_image_lock_flag = args -> param_send_image_lock_flag;
	volatile bool * capture_screenshot_lock_flag = args -> param_capture_screenshot_lock_flag;
	int cnt = 0;
	char recvMsg[BUFFER_SIZE];
	while(1) {
		cnt ++;
		int cnt_temp = 0;///////////////
		while(* send_image_lock_flag == false) {
			cnt_temp ++;
			if(cnt_temp % 10000000 == 0) {///////////////
				cnt_temp = 0;/////////////////
				printf("thread:send_image_lock_flag == false, screenshot_cnt = %d\n", screenshot_cnt);
			}/////////////////
		}
		printf("thread:send_image_lock_flag == true, %d, screenshot_cnt = %d\n",* send_image_lock_flag, screenshot_cnt);
		send_image(data_sockfd, screenshot_cnt);
		* capture_screenshot_lock_flag = true;
		* send_image_lock_flag = false;
		
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

void * create_screenshot_thread_function(void * arg) {
//	printf("thread:arg->capture_screenshot_lock_flag == t? %d\n", * ((param)arg->capture_screenshot_lock_flag));
	parameters2_link args = (parameters2_link) arg;
	printf("getViewer():create_screenshot_thread_function:arg2_address == %lx\n", arg);
	printf("getViewer():create_screenshot_thread_function:rgs2_address == %lx\n", args);
	int screenshot_cnt = args -> param_screenshot_cnt;
	float * event_Array = args -> param_event_Array;
	volatile bool * send_image_lock_flag = args -> param_send_image_lock_flag;
	volatile bool * capture_screenshot_lock_flag = args -> param_capture_screenshot_lock_flag;
	printf("thread:capture_screenshot_lock_flag == t? %d, screenshot_cnt = %d\n", * capture_screenshot_lock_flag, screenshot_cnt);
	create_screenshot(event_Array, capture_screenshot_lock_flag, send_image_lock_flag, screenshot_cnt);
	//关线程
	printf("exit cs process thread!\n");
	pthread_exit((void*)"thread exit");
}


int get_viewer(int *client_sockfd, int screenshot_cnt) {
	/*
	osgViewer::Viewer viewer; 
	osg::ref_ptr<osg::Group> root = new osg::Group(); 
	root ->addChild(osgDB::readNodeFile(osg_file_name)); 
	viewer.setSceneData(root.get ());
	viewer.addEventHandler(new PickHandler) ; 
	viewer.realize(); 
	viewer.run(); 
	*/
	float event_Array[10] = {0};
	volatile bool capture_screenshot_lock_flag = true;
	volatile bool send_image_lock_flag = false;
	printf("getViewer():capture_screenshot_lock_flag_address == %lx\n", & capture_screenshot_lock_flag);
	printf("getViewer():send_image_lock_flag_address == %lx\n", & send_image_lock_flag);
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
	printf("getViewer():client_data_addr_address == %lx\n", & client_data_addr);
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
	parameters2_link args2 = (parameters2_link)malloc(sizeof(parameters2));
	args2 -> param_screenshot_cnt = screenshot_cnt;
	args2 -> param_event_Array = event_Array;
	args2 -> param_send_image_lock_flag = & send_image_lock_flag;
	args2 -> param_capture_screenshot_lock_flag = & capture_screenshot_lock_flag;
//	parameters2_init(args2);
	printf("getViewer():capture_screenshot_lock_flag == t? %d\n", capture_screenshot_lock_flag);
	printf("getViewer():args2->param_capture_screenshot_lock_flag == t? %d\n", * (args2 -> param_capture_screenshot_lock_flag));
	if(pthread_create(&c_thread, NULL, create_screenshot_thread_function, (void *) args2) != 0){
		printf("create server data thread error\n");
		return -1;
	}else {
		printf("create server data thread success\n");
	}

	pthread_t r_thread;
	parameters_link args = (parameters_link)malloc(sizeof(parameters));;
	args -> param_screenshot_cnt = screenshot_cnt;
	args -> param_data_sockfd = &data_sockfd;
	args -> param_event_Array = event_Array;
	args -> param_send_image_lock_flag = & send_image_lock_flag;
	args -> param_capture_screenshot_lock_flag = & capture_screenshot_lock_flag;
	if(pthread_create(&r_thread, NULL, server_data_thread_function, (void *) args) != 0){
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
		printf("%d ", screenshot_cnt);
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
			printf("getViewer() 360:ESCAPE\n");
			break;
		}
	}
	printf("getViewer() finished\n");
	return 0;
}
