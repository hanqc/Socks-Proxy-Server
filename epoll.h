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
	//TODO
	void SetNonblocking(int fd)
	{
		
	}
	enum Scoks5State
	{
		AUTH,
		ESTABLISHMENT,
		FORWARDING,
	};
	struct Channel
	{
		int _fd;//������

		Channel()
			:_fd(-1)
		{}
	};
	struct Connect
	{
		Scoks5State _state;
		Channel _clientChannel;
		Channel _serverChannel;
		Connect()
			:_state(AUTH)
		{}
	};
	void Start();
	void EventLoop();
	//��̬ʵ�ֵ��麯��
	virtual void ConnectEventHandle(int connectfd) = 0;
	virtual void ReadEventHandle(int connectfd) = 0;
	virtual void WriteEventHandle(int connectfd) = 0;
protected:
	int _port;//�˿� 
	int _listenfd;//����������
	int _eventfd;//ʱ��������

	map<int,Connect*> _fdConnectMap;//fdӳ�����ӵ�map����

};

#endif //__EPOLL_H__