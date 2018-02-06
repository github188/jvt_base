#include "osa_socket.h"


namespace OSA
{

	CSocket::CSocket()	
	{  
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd <= 0)  
		{  
			std::cout << "create socket err\n";  
		}  
		memset(ip, 0, sizeof(ip));	
	}  
	  
	CSocket::~CSocket()  
	{  
		Close(); 
	}  
	  
	bool CSocket::Close()
	{  
		if (sockfd > 0)
			close(sockfd);	
		else  
			return false;  
		sockfd = -1;  
		return true;  
	}  
	  
	int  CSocket::getLocalAddr(char *interface_name, char *ipaddr)
	{  
		int s;
		if (NULL == ipaddr)
		{
			return -1;
		}

		if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) 
		{
			return -1;
		}

		struct ifreq ifr;

		strcpy(ifr.ifr_name, interface_name);

		if (ioctl(s, SIOCGIFADDR, &ifr) < 0) 
		{
			return -1;
		}

		struct sockaddr_in *ptr;
		ptr = (struct sockaddr_in *) &ifr.ifr_ifru.ifru_addr;
		snprintf(ipaddr, 16, "%s", inet_ntoa(ptr->sin_addr));
		close(s);
	}  
	  
	std::string CSocket::getCliIp()  
	{  
		char cliIp[16];  
		struct sockaddr_in cliAddr;  
		socklen_t size = sizeof(cliAddr);  
		if(getpeername(sockfd, (sockaddr*)&cliAddr, &size))  
		{  
			strcpy(cliIp, "0.0.0.0");  
		}  
		else  
		{  
			sprintf(cliIp, "%d.%d.%d.%d", ((unsigned char*)&cliAddr.sin_addr)[0],  
					((unsigned char*)&cliAddr.sin_addr)[1],  
					((unsigned char*)&cliAddr.sin_addr)[2],  
					((unsigned char*)&cliAddr.sin_addr)[3]);  
		}  
		return cliIp;
	}  
	  
	bool CSocket::setIntOptions(int option, int value)	
	{  
		bool res = false;  
		if (sockfd)	
		{  
			res = (setsockopt(sockfd, SOL_SOCKET, option, (const void*)&value, sizeof(value)) == 0);  
		}  
		return res;  
	}  
	  
	bool CSocket::setSoLinger(bool dolinger, int seconds)  
	{  
		bool res = false;  
		if (sockfd)	
		{  
			struct linger ling;  
			ling.l_onoff = dolinger?1:0;  
			ling.l_linger = seconds;  
			res = (setsockopt(sockfd,SOL_SOCKET, SO_LINGER, (const char *)&ling, sizeof(struct linger)) == 0);	
			res = true; 	  
		}  
		return res;  
	}  
	  
	bool CSocket::setTimeout(int option, int milliseconds)	
	{  
		bool res = false;  
		if (sockfd)	
		{  
			struct timeval timeout;  
			timeout.tv_sec = milliseconds/1000;  
			timeout.tv_usec = (milliseconds%1000)*1000000;	
			res = (setsockopt(sockfd, SOL_SOCKET, option, (const void*)&timeout, sizeof(timeout)) == 0);  
			res = true; 	  
		}  
		return res;  
	}  
	  
	bool CSocket::setNonBlock(bool isnonsocket)  
	{  
		bool res = false;  
		if(sockfd)	
		{  
			int oldfd = fcntl(sockfd, F_GETFL);  
			res = (fcntl(sockfd, F_SETFL, oldfd | O_NONBLOCK) <0);	
			res = true; 	  
		}  
		return res;  
	}  

	bool CSocket::setNoDelay(bool nodelay)  
	{  
	    bool res = false;  
	    if(sockfd)  
	    {  
	        int ndelay = nodelay?1:0;  
	        res = (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY,(const void*)&ndelay, sizeof(ndelay)) == 0);  
	        res =true;        
	    }  
	    return res;  
	}  
  
	bool CSocket::Send(char *sendbuf, int len)  
	{  
	    if (sockfd <0 || sendbuf==NULL || len < 0)
	        return false;  
	    int dataleft = len, total = 0, ret =0;  
	    while (dataleft > 0)  
	    {  
	        ret = send(sockfd, sendbuf+total, dataleft, 0);  
	        if(ret < 0)  
	        {  
	            if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)  
	            {  
	                usleep(50000);  
	                ret = 0;  
	            }  
	        }  
	        total += ret;  
	        dataleft = len-total;  
	    }  
	    return total == len;  
	}  
  
	bool CSocket::Recv(char *recvbuf, int len, int timeout)  
	{  
	    if (sockfd <0 || recvbuf==NULL || len < 0)  
	        return false;  
	    fd_set fds;  
	    struct timeval interval;  
	    interval.tv_sec = timeout;  
	    interval.tv_usec = 0;  
	    int recvlen = 0;  
	   	while(1) 
	    {  
	        FD_ZERO(&fds);  
	        FD_SET(sockfd, &fds);  
	        int res = select(sockfd + 1, &fds, NULL, NULL, &interval);  
	        if(res == 0)  
	            continue;  
	        if(res < 0)  
	        {  
	            close(sockfd);  
	            sockfd = -1;  
	            return false;  
	        }             
	        else  
	        {  
	            if (FD_ISSET(sockfd, &fds))
	            {  
	                recvlen = recv(sockfd, recvbuf, len, 0);  
	                break;  
	            }  
	        }  
	    }  
	    return recvlen == len;  
	}  
  
	bool CSocket::setUp(int fd, struct sockaddr_in* addr)  
	{  
	    close_fd();  
	    this->sockfd = fd;  
	    memcpy(&sockAddr, addr, sizeof(sockAddr));  
	    return true;  
	}  
  
	bool CSocket::connect_fd()  
	{  
	    if(sockfd < 0)  
	    {  
	        return false;  
	    }  
	    return (connect(sockfd, (struct sockaddr*)&sockAddr, sizeof(sockAddr)));  
	}  

}
