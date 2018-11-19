#if 1
#include "client.h"

int main(int argc, char **argv)
{
	int num = 1024 * 1024 * 10 ;
	char *content = new char[num];

	for (int i = 0; i < num; i++)
	{
		content[i] = '1';
	}
	fstream file1;
	file1.open("new.txt", ios::out);//创建文件
	file1.close();
	FILE * stream_wr;
	if (fopen_s(&stream_wr, "new.txt", "wb"))
		return 0;

	fwrite(content, sizeof(char), num, stream_wr);
	fclose(stream_wr);
	if (argc < 4)
	{
		cerr << "input parm less than 4" << endl;
		system("pause");
		return 0;
	}
	string address = argv[1];
	string port = argv[2];
	string file = argv[3];
	Client client(address, port);
	client.setSendFile(file);
	client.start();
	
	system("pause");
}
#endif
