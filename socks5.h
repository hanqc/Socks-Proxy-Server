#define __SOCKS5_H__
#ifdef __SOCKS5_H__

#include "epoll.h"

class Sock5Server : public EpollServer
{
public:
	Sock5Server()
	{}

	virtual void ConnectEventHandle(int connectfd);
	virtual void ReadEventHandle(int connectfd);
	virtual void WriteEventHandle(int connectfd);

protected:

};

#endif //__SOCKS5_H__
