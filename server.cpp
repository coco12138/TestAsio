#include "server.h"





Server::Server(string & address, int port_num)
	:address_(address_type::from_string(address)),
	port_num_(port_num),
	endpoint_(address_, port_num),
	acceptor_(io_service_, endpoint_),
	offset_(0),
	stream_wr(NULL)
{
	//acceptor_.listen();
}

void Server::write_handler(const boost::system::error_code & ec)
{
	std::cout << "link success" << std::endl;
}



void Server::accept_handler(const boost::system::error_code & ec, socket_ptr socket)
{
	if (ec)
	{
		cout << "error" << endl;
	}
	else if(!ec)
	{	
		socket->async_write_some(boost::asio::buffer(data), boost::bind(&Server::write_handler,
			this, boost::asio::placeholders::error));
		offset_ = 0;
		fstream file;
		file.open(filename_, ios::out);//创建文件
		file.close();
		if(stream_wr)
		{ 
			fclose(stream_wr);
			stream_wr = NULL;
		}
		if (fopen_s(&stream_wr, filename_.c_str(), "wb"))
			return;
		memset(recev_buffer.data(), 0, recev_buffer.size());
		socket->async_read_some(boost::asio::buffer(recev_buffer), boost::bind(&Server::save,
			this, boost::asio::placeholders::error, socket, boost::asio::placeholders::bytes_transferred));
		boost::thread(&Server::printProcess, this);
		/*socket->async_read_some(boost::asio::buffer(recev_buffer), boost::bind(&Server::read_handler,
			this, boost::asio::placeholders::error, socket, boost::asio::placeholders::bytes_transferred));*/
	}
	prepare();
}

void Server::read_handler(const boost::system::error_code & ec, socket_ptr &socket, size_t bytes_transferred)
{
	if (!ec)
	{
		
		cout << recev_buffer.data() << endl;
		/*socket->async_read_some(boost::asio::buffer(string(readbuffer)), boost::bind(&Server::save,
			this, "save.txt", readbuffer, socket, boost::asio::placeholders::bytes_transferred));*/
	}
}

void Server::save(const boost::system::error_code & ec, socket_ptr socket,size_t bytes_transferred)
{
	if (ec)
		return;
	size_t num_written = 0;
	//cout << recev_buffer.data() << endl;
	//stream_wr = NULL;
	//cout << bytes_transferred << endl;
	num_written = fwrite(recev_buffer.data(), sizeof(char), bytes_transferred, stream_wr);

	offset_ += num_written;

	_fseeki64(stream_wr, offset_, SEEK_SET);
	
	try
	{
		memset(recev_buffer.data(), 0, recev_buffer.size());
		socket->async_read_some(boost::asio::buffer(recev_buffer), boost::bind(&Server::save,
			this, boost::asio::placeholders::error, socket, boost::asio::placeholders::bytes_transferred));
	}
	catch (const boost::system::error_code & ec1)
	{
		socket->shutdown(boost::asio::socket_base::shutdown_both);
	}
	

}

void Server::printProcess()
{
	int tmp_offset = 0;
	while (tmp_offset != offset_ || offset_ == 0)
	{
		tmp_offset = offset_;
		boost::this_thread::sleep(boost::posix_time::seconds(2));
		if(offset_ != 0)
			cout << "transport : " << offset_ << endl;
		
	}
	fclose(stream_wr);
	stream_wr = NULL;
	if(tmp_offset != offset_)
		cout << "transport : " << offset_  << endl;
}

void Server::show()
{
	cout << "address : " << address_.to_string() << "\tport : " << port_num_ << endl;
}

void Server::prepare()
{
	
	socket_ptr sock(new socket_type(io_service_));
	
	acceptor_.async_accept(*sock, boost::bind(&Server::accept_handler, this,
		boost::asio::placeholders::error, sock));

}

void Server::start()
{
	prepare();
	cout << "Service start....wait for connecting" << endl;
	show();
	io_service_.run();
}
