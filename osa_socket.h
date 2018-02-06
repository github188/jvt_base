#ifndef OSA_SOCKET_POSIX_INCLUDED
#define OSA_SOCKET_POSIX_INCLUDED

#include <iostream>  
#include<sys/socket.h>  
#include<sys/un.h>  
#include<netinet/in.h>  
#include <string.h>  
#include <sys/types.h>  
#include <unistd.h>  
#include <errno.h>  
#include <arpa/inet.h>  
#include <fcntl.h>  
#include <netdb.h>  
#include <stdio.h>  
#include <netinet/tcp.h>  


namespace OSA {

	class CSocket  
	{  
	public:  
		CSocket();	
		virtual ~CSocket();  
		int  getLocalAddr();
		std::string getCliIp();  
		int getSockfd() { return sockfd;}  
		bool close_fd();  
		bool connect_fd();  
		bool send_msg(char* sendbuf, int len);	
		bool recv_msg(char* recvbuf, int len, int timeout);  
	  
		bool setSoLinger(bool dolinger, int seconds);  
		bool setIntOptions(int option, int value);	
		bool setTimeout(int option, int milliseconds);	
		bool setNonBlock(bool isnonsocket);  
		bool setNoDelay(bool nodelay);	
	  
		bool setUp(int sockfd, struct sockaddr_in* addr);  
		bool setAddr(const char* addr, int port);  
	protected:	
		int sockfd;  
		char ip[16];
		struct sockaddr_in sockAddr;	  
		CSocket(const CSocket&);  
		CSocket& operator=(const CSocket&);  
	};	



}

