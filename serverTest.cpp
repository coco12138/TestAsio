#include "server.h"
#include <boost\thread.hpp>
int main(int argc, char **argv)
{
	if (argc < 4)
	{ 
		cerr << "input parm less than 4" << endl;
		system("pause");
		return 0;
	}
	int port = atoi(argv[2]);
	//192.168.20.5
	Server server(string(argv[1]), port);
	server.setRecvFile(string(argv[3]));
	server.start();
	system("pause");
}
