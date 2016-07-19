#ifndef _SENDFILE_H_
#define _SENDFILE_H_

#include <osgDB/ReadFile> 
#include <osgViewer/Viewer> 
#include <osg/Group> 
#include <osgGA/GUIEventHandler> 
#include <osgUtil/LineSegmentIntersector> 
#include <osgGA/StateSetManipulator>
#include <iostream>
using namespace std;

#define CLIENT_PORT 9988
#define SERVER_PORT 36677
#define CLIENT_DATA_PORT 8888
#define BUFFER_SIZE 1024 /// 控制命令缓冲区大小
#define BUFFER_SIZE_FILE 1024 * 1024
#define MAX_CONNECTION 10 // 最大连接客户端个数
#define OPERATION_NUMBER 10 //操作数
#define BYTES_PER_TRANS 100000

class PickHandler : public osgGA::GUIEventHandler 
{ 
public:
	PickHandler(int *sockfd);
	~PickHandler(){}
	//处理（事件接口、动作接口）
    bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
	void getMessage();
protected :
	float pickArray[4];//事件键值，键码（鼠标默认0），X,Y坐标
    int *client_sockfd;
};

int send_File(int client_sockfd);
int recv_Image(int *client_sockfd);
int get_Viewer(int *client_sockfd);
#endif
