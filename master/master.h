#ifndef _SENDFILE_H_
#define _SENDFILE_H_

#include <osgDB/ReadFile> 
#include <osgViewer/Viewer> 
#include <osg/Group> 
#include <osgGA/GUIEventHandler> 
#include <osgUtil/LineSegmentIntersector> 
#include <iostream>

using namespace std;

#define MASTER_PORT 6666
#define SERVER_PORT 36677
#define CLIENT_DATA_PORT 8888
#define BUFFER_SIZE 1024 /// 控制命令缓冲区大小
#define BUFFER_SIZE_FILE 1024 * 1024
#define MAX_CONNECTION 10 // 最大连接客户端个数
#define BYTES_PER_TRANS 100000
#define MAX_SERVER_NUM 100 //
#define SERVER_INFO_PATH "../files/info/allServerInfo.csv"
//#define TASK_INFO_PATH "../files/info/taskInfo.csv"

/*class serverInfo {
public:
    serverInfo();
*/

vector<string> split(string rawString, char spliter);
string join(vector<string> vec, char joiner);
vector<vector<string>> readCsv(string filePath);
string getCsvValue(string filePath, int row, int col);
int setCsvValue(string filePath, int row, int col, string setValue);
bool appendInfoToCsv(string filePath, string info);
bool deleteInfoFromCsv(string filePath, string keyword);

int serverInit();
int serverInit(int server_num , string server_ip[]);
int getServerNum();
string getServerIp(int server_id);
int getServerId(string ip);
int getServerConsViaId(int server_id);
int setServerConsViaId(int server_id, int cons);
int addServerConsViaId(int server_id);
int decServerConsViaId(int server_id);
int getServerConsViaIp(string server_ip);
int setServerConsViaIp(string server_ip, int cons);
int addServerConsViaIp(string server_ip);
int decServerConsViaIp(string server_ip);
int addServer(string ip);
int removeServer(int server_id);
int removeServer(string ip);
//int getServerWithMinLoad();
string getServerWithMinLoad();
bool IsServerExistent(string ip);
    
/*protected:
//  char serverIp[MAX_SERVER_NUM][BUFFER_SIZE];
//  int serverCons[MAX_SERVER_NUM];
    int serverNum;
};
*/
/*class taskInfo {
public:
    taskInfo();
	int init();
	int init(int task_num , char task_server_ip[][BUFFER_SIZE]);
    int getServerCons(int server_id);
    int addServerCons(int server_id);
    int decServerCons(int server_id);
    char * getServerIp(int server_id);
    int getServerId(char * ip);
    int getServerNum();
    
protected:
    char serverIp[MAX_SERVER_NUM][BUFFER_SIZE];
    int serverCons[MAX_SERVER_NUM];
    int serverNum;
};*/


#endif
