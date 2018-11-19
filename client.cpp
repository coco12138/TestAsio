#include "client.h"


Client::Client(string &address, string &port_num)
	:address_(address_type::from_string(address)),
	port_num_(port_num),
	query_(address, port_num_),
	isFileExit_(false),
	socket_(io_service_),
	resolver_(io_service_),
	filename_(""),
	filelength_(0),
	fileoffset_(0)
{
}
void Client::resolve_handler(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it)
{
	if (!ec)
	{
		socket_.async_connect(*it,
			boost::bind(&Client::connect_handler, this, boost::asio::placeholders::error));
	}
}
void Client::setSendFile(string &file_path)
{
	filename_ = file_path;
	
}

bool Client::sendHugeFile(string & file_path)
{
	FILE *stream_rd = NULL;
	size_t read_num = 0;
	fileoffset_ = 0;

	char *buf = new char[WR_SIZE];
	memset(buf, 0, WR_SIZE);
	if (fopen_s(&stream_rd, file_path.c_str(), "rb"))
		isFileExit_ = false;
	else
		isFileExit_ = true;
	if(!isFileExit_)
		return false;
	ifstream is;
	is.open(file_path, ios::binary);
	is.seekg(0, ios::end);
	filelength_ = is.tellg();
	cout << "file content :  " << filelength_ << endl;
	boost::thread t(&Client::printProgress, this);
	while (!feof(stream_rd))
	{

		memset(buf, 0, WR_SIZE);
		read_num = fread(buf, sizeof(char), WR_SIZE, stream_rd);
		if (read_num == 0) break;
		fileoffset_ += read_num;
		_fseeki64(stream_rd, fileoffset_, SEEK_SET);
		//boost::asio::write(socket_, boost:a:asio::buffer(send_buffer));
		socket_.send(boost::asio::buffer(string(buf)));
	}
	//t.interrupt();
	t.join();
	//cout << buf << endl;
	delete[]buf;
	fclose(stream_rd);
	return true;
}

void Client::printProgress()
{
	while(fileoffset_ != filelength_)
	{ 
		cout << "percent : " << fileoffset_ * 100.0 / filelength_ << " % " << endl;
		boost::this_thread::sleep(boost::posix_time::seconds(1));
	}
	cout << "percent : " << fileoffset_ << " % " << endl;
}

void Client::start()
{
	resolver_.async_resolve(query_, 
		boost::bind(&Client::resolve_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator));
	io_service_.run();
}
