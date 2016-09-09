#include <sstream>
#include "master.h"

using namespace std;

int main()
{
/*	std::string text = "152\0";
	int number = 0;
	std::stringstream ss;

	ss << text;//可以是其他数据类型
	ss >> number; //string -> int
	if (! ss.good())
	{
		if(ss.bad()) {
			printf("ss is bad\n");
		}
		if(ss.fail()) {
			printf("ss is fail\n");
		}
		if(ss.eof()) {
			printf("ss is eof\n");
		}
		printf("%d\n", number);
		printf("ss is not good\n"); //错误发生
	}

*/
	string server_id[2] = {"192.168.0.137", "192.168.1.114"};
	serverInit(2, server_id);
	addServerConsViaId(1);
/*	cout << getServerIp(1) << endl;
	cout << getServerConsViaIp("192.168.0.137") << endl;
	cout << getServerConsViaId(2) << endl;
	cout << getServerId("192.168.0.137") << endl;
	addServer("192.168.1.113");
	removeServer(1);
	addServer("192.168.1.115");
	addServer("192.168.1.116");
	removeServer("192.168.1.113");
*/
	cout << getServerWithMinLoad() << endl;
	serverInit();
	cout << getServerWithMinLoad() << endl;
}