#ifndef  CLIENT_H_
#define CLIENT_H_

#include <boost/asio.hpp> 
#include <string> 
#include <iostream>
#include <boost\shared_ptr.hpp>
#include <boost\bind.hpp>
#include <boost\asio.hpp>
#include <boost/array.hpp> 
#include <fstream>
#include <boost\thread.hpp>
using namespace std;
class Client
{
public:
#define WR_SIZE 1024 * 1024 * 10
	typedef boost::asio::io_service service_type;

	typedef boost::asio::ip::tcp::resolver resolver_type;

	typedef boost::asio::ip::tcp::socket socket_type;

	typedef boost::asio::ip::tcp::resolver::query query_type;

	typedef boost::asio::ip::address address_type;

	Client(string &address, string &port_num);

	void setSendFile(string &file_path);
	
	bool sendHugeFile(string &file_path);

	void printProgress();

	void start();
private:

	void resolve_handler(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it);
	void connect_handler(const boost::system::error_code &ec)
	{
		if (!ec)
		{
			sendHugeFile(filename_);
			//socket_.shutdown(boost::asio::socket_base::shutdown_both);
			//boost::asio::write(socket_, boost::asio::buffer(send_buffer));
			/*socket_.async_read_some(boost::asio::buffer(buffer), 
				boost::bind(&Client::read_handler, this, boost::asio::placeholders::error));*/
		}
	}
	void read_handler(const boost::system::error_code &ec)
	{
		if (!ec)
		{
			std::cout << std::string(buffer.data()) << std::endl;
			socket_.async_read_some(boost::asio::buffer(buffer), 
				boost::bind(&Client::read_handler, this, boost::asio::placeholders::error));
		}
	}
	service_type io_service_;
	socket_type socket_;
	string port_num_;
	query_type query_;
	address_type address_;
	resolver_type resolver_;
	
	string filename_;
	long long fileoffset_;
	int filelength_;
	boost::array<char, 4096> buffer;
	boost::array<char, 4096> send_buffer;
	bool isFileExit_;
};
