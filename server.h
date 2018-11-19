#ifndef SERVER_H_
#define SERVER_H_

#include <boost/asio.hpp> 
#include <string> 
#include <iostream>
#include <boost\shared_ptr.hpp>
#include <boost\bind.hpp>
#include <boost\asio.hpp>
#include <boost\array.hpp>
#include <boost\weak_ptr.hpp>
#include <fstream>
#include <boost\thread.hpp>
using namespace std;
class Server
{
	
public:
#define WR_SIZE 1024 * 1042 * 10	 //10M
	typedef boost::asio::io_service service_type;

	typedef boost::asio::ip::tcp::resolver resolver_type;

	typedef boost::asio::ip::tcp::socket socket_type;

	typedef boost::asio::ip::tcp::endpoint endpoint_type;

	typedef boost::asio::ip::tcp::acceptor acceptor_type;

	typedef boost::asio::ip::address address_type;

	typedef boost::shared_ptr<socket_type> socket_ptr;

	

	Server(string & address, int port_num);
	void start();

	void setRecvFile(string &filename)
	{
		filename_ = filename;
	}
private:
	void write_handler(const boost::system::error_code &ec);

	void accept_handler(const boost::system::error_code &ec, socket_ptr socket);

	void read_handler(const boost::system::error_code &ec, socket_ptr &socket, size_t bytes_transferred);

	void save(const boost::system::error_code & ec, socket_ptr socket, size_t bytes_transferred);

	void printProcess();

	void show();

	void prepare();

	
private:

	string data;
	service_type io_service_;
	address_type address_;
	endpoint_type endpoint_;
	acceptor_type acceptor_;
	
	boost::array<char, 4096> recev_buffer;
	int port_num_;
	size_t offset_;
	FILE *stream_wr;

	string filename_;
};




#endif
