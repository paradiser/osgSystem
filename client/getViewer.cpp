#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <osgDB/ReadFile>
#include <osg/DrawPixels>
#include "client.h"
using namespace std;


PickHandler::PickHandler(int *sockfd) {

	client_sockfd = sockfd;
}

float * PickHandler::getpickArray() {

	return pickArray;
}

void PickHandler::getMessage()
{
/*	for(int i=0;i<3;i++)
		cout<<pickArray[i]<<"  ";
	cout<<pickArray[3]<<endl;
*//*	if(pickArray[0] == 32 | pickArray[0]) {
		printf("Esc pressed! : 65307\n");
	}
*/	char recvMsg[BUFFER_SIZE];
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

volatile bool read_image_lock_flag = false; // false: can not read
volatile bool write_image_lock_flag = false;// false: can not write

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
		printf("client_getViewer line 133: bind socket error!:%s\n",strerror(errno));
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
  		if(recv_Image(data_sockfd_tmp , cnt) == 0) {
  			write_image_lock_flag = false;
  			read_image_lock_flag = true;
//			printf("getViewer line 157: read_image_lock_flag is %d\n" , read_image_lock_flag);
			while(write_image_lock_flag == false) {}
		}
		recv(*data_sockfd_tmp , recvMsg , sizeof(recvMsg) , 0);
	   // printf("%d: %s\n" , cnt, recvMsg);
		send(*data_sockfd_tmp , "continue" , sizeof(recvMsg) , 0);
		if(strcmp(recvMsg , "close") == 0) 
			break;
  	}
  	//服务器停止send后关闭线程
	printf("recv %d images\n" , cnt);
	close(*data_sockfd_tmp);
	close(client_data_sockfd);
	printf("close client data socket!\n");
	printf("exit process thread!\n");
	pthread_exit((void*)"thread exit");
}

int get_Viewer(int *client_sockfd) {
	
	//创建新的data的线程
	char recvMsg[BUFFER_SIZE];
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
  	recv(*client_sockfd , recvMsg , sizeof(recvMsg) , 0);
  	//阻塞进程，避免读图片先于写图片
  	while(read_image_lock_flag == false) {}
	osgViewer::Viewer viewer; 
	osg::ref_ptr<osg::DrawPixels> bmp = new osg::DrawPixels;
	bmp->setPosition(osg::Vec3(0.0f, 0.0f, 0.0f));
	char image_file_path[BUFFER_SIZE] = "../files/recvImage/recv_image.png";
	FILE *fp = NULL;
	bmp->setImage(osgDB::readImageFile(image_file_path));
	write_image_lock_flag = true;
	read_image_lock_flag = false;
	/*fp = fopen(image_file_path, "rb");
	if(0 == flock(fileno(fp), LOCK_EX)) {
		printf("getViewer line 207: Lock()\n");
		bmp->setImage(osgDB::readImageFile(image_file_path));
		flock(fileno(fp), LOCK_UN);
		printf("getViewer line 210: unLock()\n");
		fclose(fp);
	} else {
		printf("client_getViewer line 223: lock failed\n");
		fclose(fp);
	}*/
	osg::ref_ptr<osg::Group> root = new osg::Group;
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(bmp.get());
	root->addChild(geode.get());
	viewer.setSceneData(root.get());
	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
	osg::ref_ptr<PickHandler> eventPickHandler = new PickHandler(client_sockfd);
	viewer.addEventHandler(eventPickHandler) ; 
	// 旋转一定角度(弧度值)。 
	viewer.realize();
	osg::Matrix trans;
	trans.makeTranslate( -1, -1, 0);
	double angle( 0. );
	//开始渲染
	while (!viewer.done())
	{
		// 创建旋转矩阵。
		bmp = new osg::DrawPixels;
		osg::Matrix rot;
		rot.makeRotate( angle, osg::Vec3( 0, 0, 0 ) );
		angle += 0.01;
		// 设置视口矩阵(旋转矩阵和平移矩阵连乘)
		viewer.getCamera()->setProjectionMatrix(rot * trans );
		root->removeChild(geode);
		while(read_image_lock_flag == false){
			if(*(eventPickHandler->getpickArray() + 1) == ESCAPE_NUM)
				return 0;
		}
	  	bmp->setImage(osgDB::readImageFile(image_file_path));
	  	write_image_lock_flag = true;
	  	read_image_lock_flag = false;
		
		/*osg::ref_ptr<osg::Geode>*/ geode = new osg::Geode;
		geode->addDrawable(bmp.get());
		viewer.setSceneData(geode.get());
		viewer.getCamera()->setViewMatrix( rot * trans );

		//geode->addDrawable(bmp.get());
		//viewer.setSceneData(geode.get());
		//viewer.realize();
		viewer.frame();
	}
//	viewer.realize(); 
//	viewer.run(); 
  	return 0;
}
