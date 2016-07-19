#ifndef _SENDFILE_H_
#define _SENDFILE_H_

#include <osgDB/ReadFile> 
#include <osgViewer/Viewer> 
#include <osg/Group> 
#include <osgGA/GUIEventHandler> 
#include <osgUtil/LineSegmentIntersector> 
#include <iostream>
using namespace std;

#define SERVER_PORT 36677
#define CLIENT_DATA_PORT 8888
#define BUFFER_SIZE 1024 /// 控制命令缓冲区大小
#define BUFFER_SIZE_FILE 1024 * 1024
#define MAX_CONNECTION 10 // 最大连接客户端个数
#define OPERATION_NUMBER 10 //操作数
#define BYTES_PER_TRANS 100000
#define osg_file_name "/home/paradiser/desktop/recv.osg"


class PickHandler : public osgGA::GUIEventHandler 
{ 
public:
	PickHandler(){};
	~PickHandler(){}
	//´¦Àí£¨ÊÂ¼þ½Ó¿Ú¡¢¶¯×÷½Ó¿Ú£©
    bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
	void getMessage();
protected :
	float pickArray[4];//ÊÂ¼þ¼üÖµ£¬¼üÂë£¨Êó±êÄ¬ÈÏ0£©£¬X,Y×ø±ê
};

int recv_File(int *client_sockfd);
int send_Image(int *client_sockfd);
int get_Viewer(int *client_sockfd);
#endif
