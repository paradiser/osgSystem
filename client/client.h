#ifndef _SENDFILE_H_
#define _SENDFILE_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <osgDB/ReadFile>
#include <osg/DrawPixels>
#include <osgViewer/Viewer> 
#include <osg/Group> 
#include <osgGA/GUIEventHandler> 
#include <osgUtil/LineSegmentIntersector> 
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>

using namespace std;

#define MASTER_PORT 6666
#define CLIENT_PORT 9988
#define SERVER_PORT 36699
#define CLIENT_DATA_PORT 8888
#define BUFFER_SIZE 1024 /// 控制命令缓冲区大小
#define BUFFER_SIZE_FILE 1024 * 1024
#define MAX_CONNECTION 10 // 最大连接客户端个数
#define OPERATION_NUMBER 10 //操作数
#define BYTES_PER_TRANS 100000
#define ESCAPE_NUM 65293
#define IMAGE_FORMAT ".jpg"
#define SELF_INFO_PATH "../files/info/selfInfo.csv"
#define MASTER_INFO_PATH "../files/info/masterInfo.csv"

vector<string> split(string rawString, char spliter);
string join(vector<string> vec, char joiner);
vector<vector<string>> readCsv(string filePath);
string getCsvValue(string filePath, int row, int col);
int setCsvValue(string filePath, int row, int col, string setValue);
bool appendInfoToCsv(string filePath, string info);
bool deleteInfoFromCsv(string filePath, string keyword);

class PickHandler : public osgGA::GUIEventHandler 
{ 
public:
	PickHandler(int *sockfd);
	~PickHandler(){}
	//处理（事件接口、动作接口）
    bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
	void getMessage();
	float *getpickArray();
protected :
	float pickArray[5];//事件键值，键码，x，y坐标 
    int *client_sockfd;
};

int send_File(int client_sockfd);
int recv_Image(int *client_sockfd , int cnt);
int get_Viewer(int *client_sockfd);
int copyFile(char *name , char *osg_image_name);
#endif
