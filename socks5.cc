#include "socks5.h"

void Sock5Server::ConnectEventHandle(int connectfd)
{
	TraceLog("new conenct event:%d", connectfd);

	// ���connectfd��epoll���������¼�
	SetNonblocking(connectfd);
	OPEvent(connectfd, EPOLLIN, EPOLL_CTL_ADD);
}

void Sock5Server::ReadEventHandle(int connectfd)
{
	TraceLog("conenct event:%d", connectfd);
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
