#include <string>
#include <sstream>
#include "master.h"

using namespace std;
/*serverInfo::serverInfo(int server_num , char server_ip[][BUFFER_SIZE]) {
	serverNum = server_num;
	for(int i=0; i<server_num; i++) {
		strcpy(serverIp[i] , server_ip[i]);
		serverCons[i] = 0;
	}
}
*/
int serverInit() {
	if (freopen (SERVER_INFO_PATH, "w", stdout) == NULL){
		return -1;
	}
	cout << "0,3," << endl;
	fclose(stdout);
	freopen("/dev/tty", "w", stdout);
	return 0;
}

int serverInit(int server_num , string server_ip[]) {
	if (freopen (SERVER_INFO_PATH, "w", stdout) == NULL){
		return -1;
	}
	cout << server_num << ",3," << endl;
	for(int i = 0; i < server_num; i ++) {
		cout << i + 1 << "," << server_ip[i] << "," << "0," << endl;
	}
	fclose(stdout);
	freopen("/dev/tty", "w", stdout);
	return 0;
}

int getServerNum() {
	std::stringstream ss;
	ss << getCsvValue(SERVER_INFO_PATH, 0, 0);
	int serverNum;
	ss >> serverNum; //string -> int
	return serverNum;
}

string getServerIp(int server_id) {
	return getCsvValue(SERVER_INFO_PATH, server_id, 1);
}

int getServerId(string ip) {
	int serverNum;
	serverNum = getServerNum();
	for(int i = 1; i <= serverNum; i ++) {
		if(strcmp(ip.c_str(), getServerIp(i).c_str()) == 0) {
			return i;
		}
	}
	return 0;
}

int getServerConsViaId(int server_id) {
	std::stringstream ss;
	ss << getCsvValue(SERVER_INFO_PATH, server_id, 2);
	int cons;
	ss >> cons; //string -> int
/*	if (! ss.good())
	{
		return -1;
	}
*/
//	printf("cons = %d\n", cons);
	return cons;
}

int setServerConsViaId(int server_id, int cons) {
	std::stringstream ss;
	ss << cons;
	string cons_str = ss.str(); //int -> string
/*	if (! ss.good())
	{
		return -1;
	}
*/
	setCsvValue(SERVER_INFO_PATH, server_id, 2, cons_str);
	return 0;
}
    
int addServerConsViaId(int server_id) {
	setServerConsViaId(server_id, getServerConsViaId(server_id) + 1);
	return 0;
}

int decServerConsViaId(int server_id) {
	setServerConsViaId(server_id, getServerConsViaId(server_id) - 1);
	return 0;
}

int getServerConsViaIp(string server_ip) {
	return getServerConsViaId(getServerId(server_ip));
}

int setServerConsViaIp(string server_ip, int cons) {
	setServerConsViaId(getServerId(server_ip), cons);
	return 0;
}
    
int addServerConsViaIp(string server_ip) {
	int server_id;
	server_id = getServerId(server_ip);
	setServerConsViaId(server_id, getServerConsViaId(server_id) + 1);
	return 0;
}

int decServerConsViaIp(string server_ip) {
	int server_id;
	server_id = getServerId(server_ip);
	setServerConsViaId(server_id, getServerConsViaId(server_id) - 1);
	return 0;
}

int addServer(string ip) {
	return 0;
}

int removeServer(int server_id) {
	return 0;
}

int removeServer(string ip) {
	return 0;
}

/*int getServerId(char * ip) {
	for(int i=0; i<getServerNum(); i++) {
		if(strcmp(getServerIp(i) , ip) == 0)
			return i;
	}
	return -1;
}
*/
