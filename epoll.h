#define __EPOLL_H__
#ifdef __EPOLL_H__
#include "common.h"

class EpollServer
{
public:
	EpollServer(int port=8000)
		:_port(port)
		,_listenfd(-1)
		,_eventfd(-1)
	{}
	~EpollServer()
	{
		if (_listenfd!=-1)
			close(_listenfd);
	}

	void OPEvent(int fd,int events,int op)
	{
		struct epoll_event event;
		event.events = events;
		event.data.fd = fd;
		if(epoll_ctl(_eventfd,fd,op,&event)<0)
		{
			ErrorLog("epoll_ctl(op:%d,fd:%d)",op,fd);
		}
	}
	void Start();
	void EventLoop();
	//多态实现的虚函数
	virtual void ConnectEventHandle(int connectfd) = 0;
	virtual void ReadEventHandle(int connectfd) = 0;
	virtual void WriteEventHandle(int connectfd) = 0;
protected:
	int _port;//端口 
	int _listenfd;//监听描述符
	int _eventfd;//时间描述符

};

#endif //__EPOLL_H__