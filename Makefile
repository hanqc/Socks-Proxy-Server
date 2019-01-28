cc = g++
.PHONY:all 
all:Socks5_server Transfer_server

Socks5_server:socks5.cc epoll.cc 
	$(cc) $^ -o $@

Transfer_server:tranfer.cc epoll.cc 
	$(cc) $^ -o $@
.PHONY:clean
clean:
	rm Socks5_server Transfer_server 

