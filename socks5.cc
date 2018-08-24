#include "socks5.h"

void Sock5Server::ConnectEventHandle(int connectfd)
{
	TraceLog("new conenct event:%d", connectfd);

	// 添加connectfd到epoll，监听读事件
	SetNonblocking(connectfd);
	OPEvent(connectfd, EPOLLIN, EPOLL_CTL_ADD);

	Connect* con = new Connect;
	con->_state = AUTH;
	con->_clientChannel._fd = connectfd;
	_fdConnectMap[connectfd] = con;
}
//0表示
int Sock5Server::AuthHandle(int fd)
{
	char buf[260]
	int rlen = recv(fd,buf,260,MSG_PEEK);
	if (rlen==-1)
	{
		return -1;
	}
	else if (rlen<3)
	{
		return 0;
	}
	else
	{
		recv(fd,buf,rlen,0);
		if (buf[0]!=0x05)
		{
			ErrorLog("not socks5 protocol");
		}

	}
}

int Sock5Server::EstablishmentHandle(int fd)
{
	char buf[256];
	int rlen = recv(fd,buf,256,MSG_PEEK);
	if (rlen<10)
	{
		return -2;
	}
	else if (rlen==-1 )
	{
		return =-1;
	}
	else
	{
		char ip[4];
		char port[2];
		recv(fd,buf,4,0);
		char addresstpye = buf[3];
		if (addresstpye==0x01)//ipv4
		{
			recv(fd,ip,4,0);
			recv(fd,port,2,0);
		}
		else if (addresstpye==0x03)//yu min
		{
			char len = 0;
			recv(fd,,&len,1,0);
			recv(fd,buf,len,0);
			recv(fd,port,2,0);
			TraceLog("yumin:%s",buf);
			buf[len] = '\0';
			struct hostent* hptr = gethostbyname(buf);
			if (hptr == NULL)
			{
				ErrorLog("gethostbyname(): %s",buf);
				return -1;
			}
			memcpy(ip, hptr->h_addr, hptr->h_length);
		
		} 
		else if (addresstpye==0x04)//ipv6
		{
			ErrorLog("not support ipv6");
			return -1;
		} 
		else
		{
			ErrorLog("invalid address type");
			return -1;
		}

		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(struct sockaddr_in));
		addr.sin_family = AF_INET;
		memcpy(&addr.sin_addr.s_addr, ip, 4);
		addr.sin_port = *((uint16_t*)port);
		int serverfd = socket(AF_INET, SOCK_STREAM, 0);
		if(serverfd < 0)
		{
			ErrorLog("socket");
			return -1;
		}
		if (connect(serverfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
		{
			ErrorLog("connect error");
			close(serverfd);
			return -1;
		}
		return serverfd;
	}
}
void Sock5Server::ReadEventHandle(int connectfd)
{
	TraceLog("read event:%d", connectfd);
	map<int,Connect*> ::iterator it = _fdConnectMap.find(connectfd);
	if (it!=_fdConnectMap.end())
	{
		Connect* con = it->second;
		if (con->_state==AUTH)
		{
			char reply[2];
			reply[0]==0x05;
			int ret = AuthHandle(connectfd);
			if (ret ==0)
			{

			}
			else if (ret==1)
			{
			} 
			else if(ret==-1) 
			{
			}
		}
		else if (con->_state==ESTABLISHMENT)
		{
			char reply[10] = {0};
			reply[0] = 0x05;
			int serverfd = EstablishmentHandle(connectfd);
			if (serverfd==-1)
			{
				ErrorLog("EstablishmentHandle");
				//RemoveConnect(connectfd);
				reply[1]==0x01;
				
			}
			else if (serverfd==-2)
			{
				return;
			}
			else
			{
				reply[1] = 0x00;
			}
			if (send(connectfd,reply,10,0)!=10)
			{
				ErrorLog("EstablishmentHandle reply");
			}
			SetNonblocking(serverfd);
			OPEvent(serverfd,EPOLLIN,EPOLL_CTL_ADD);
			con->_serverChannel._fd = serverfd;
			_fdConnectMap[serverfd] = con;
			con->_state = FORWARDING;
		
		} 
		else if (con->_state==FORWARDING)
		{
			TraceLog("forwarding");
			Channel* clientChannel = &con->_clientChannel;
			Channel* serverChannel = &con->_serverChannel;
			if (connectfd==serverChannel->_fd)
			{
				swap(clientChannel,serverChannel);
			}
		} 
		else
		{
		}
	}
	else
	{
		assert(false);
	}
}

void Sock5Server::WriteEventHandle(int connectfd)
{
	TraceLog("");
}

int main()
{
	Sock5Server server;
	server.Start();
}
