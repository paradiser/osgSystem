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
#define ESCAPE_NUM 65293
#define IMAGE_FORMAT ".jpg"
#define osg_file_name "../files/recvOsg/recv.osg"


class PickHandler : public osgGA::GUIEventHandler 
{ 
public:
	PickHandler(){};
	~PickHandler(){}
	//´¦Àí£¨ÊÂ¼þ½Ó¿Ú¡¢¶¯×÷½Ó¿Ú£©
    bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
	void getMessage();
	float * getPickArray();
protected :
	float pickArray[5];//ÊÂ¼þ¼üÖµ£¬¼üÂë£¨Êó±êÄ¬ÈÏ0£©£¬X,Y×ø±ê
};

class PickInfoHandler : public osgGA::GUIEventHandler
{
public:
	PickInfoHandler(float * event_Array);	//¹¹Ôìº¯Êýº¬³õÊ¼»¯
	virtual bool handle( const osgGA::GUIEventAdapter& ea,
		osgGA::GUIActionAdapter& aa );//ÖØÔØÊÂ¼þ´¦Àíº¯Êý
	void PrintInfo();	//´òÓ¡²âÊÔº¯Êý
//	bool CopyInfo(float pickInfo[5]);	//¿½±´ÐÅÏ¢º¯Êý
	void ClearInfo();	//Çå¿Õº¯Êý£¬ÒÑÍê³ÉÏìÓ¦
	int getPick(int n);	//»ñÈ¡ÐÅÏ¢µÄµÚnÎ»Êý¾Ý
	bool getSignal();	//»ñÈ¡ÐÅÏ¢½ÓÊÕ±êÖ¾
protected:
	float * pickArray;
	bool _signalInfo;
};

class finalDraw : public osg::Camera::DrawCallback//相机更新回调类
{
public:	 
	finalDraw(volatile bool * capture_screenshot_lock_flag_in, volatile bool * send_image_lock_flag_in, int screenshot_cnt);
	/*virtual*/ void operator () (osg::RenderInfo& renderInfo) const;
protected:	 
	osg::ref_ptr<osg::Image> _image;//图片变量
	volatile bool * _capture_screenshot_lock_flag_in;
	volatile bool * _send_image_lock_flag_in;
	int _screenshot_cnt;
//	mutable OpenThreads::Mutex _mutex;//线程保护对象变量
};

struct UserInfo : public osg::Referenced //ÓÃ»§×Ô¶¨ÒåÊÂ¼þ
{
	UserInfo( unsigned int c ) : _count(c) {}//¹¹Ôìº¯Êý
	unsigned int _count;
};

int recv_file(int *client_sockfd);
int send_image(int *client_sockfd, int screenshot_cnt);
int get_viewer(int *client_sockfd, int screenshot_cnt);
int create_screenshot(float *event_Array, volatile bool * capture_screenshot_lock_flag_in, volatile bool * send_image_lock_flag_in, int screenshot_cnt);
#endif
