/*
 * socket.c
 *      function: 
 *      Created on: 2018年3月10日
 *      Author: cjiejie
 *     Mail: cjiejie@outlook.com  
 */

#include <stdio.h>
#include<sys/socket.h>
#include <string.h>
#include <errno.h>
#include<sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

/**
 * 函数名：CreatLocalSocketServer
 * 功能：创建本地socket
 * 参数：
 * 返回：
 * 备注：
 *
 */
int CreatLocalSocketServer(const  char *socket_name, int block_flag)
{
	int listen_fd = -1,ret = -1;
	struct sockaddr_un srv_addr = {0};
	listen_fd=socket(AF_UNIX,SOCK_STREAM,0);
	if(listen_fd < 0) {
		printf("<%s,%d> socket err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
		return -1;
	}
	// 设置服务器地址参数
	srv_addr.sun_family=AF_UNIX;
	strncpy(srv_addr.sun_path,socket_name,sizeof(srv_addr.sun_path)-1);
	unlink(socket_name);
	//bind sockfd&addr
	ret=bind(listen_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
	if(ret<0){
		printf("<%s,%d> bind err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
	close(listen_fd);
	unlink(socket_name);
	return -1;
	}
	//listen sockfd
	ret=listen(listen_fd,1);
	if(ret<0){
		printf("<%s,%d> listen err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
	 close(listen_fd);
	 unlink(socket_name);
	 return -1;
	}
	return listen_fd; //ready to accept
}

/**
 * 函数名：ConnectLocalSocket
 * 功能：链接本地socket
 * 参数：
 * 返回：
 * 备注：
 *
 */
int ConnectLocalSocket(const  char *socket_name)
{
	int ret = -1,connect_fd = -1;
	struct sockaddr_un srv_addr = {0};
	connect_fd=socket(PF_UNIX,SOCK_STREAM,0);
	if(connect_fd  < 0) {
		printf("<%s,%d> socket err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
		return -1;
	}
	srv_addr.sun_family=AF_UNIX;
	strcpy(srv_addr.sun_path,socket_name);
	ret=connect(connect_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
	if (ret<0) {
		printf("<%s,%d> connect err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
		close(connect_fd);
		return -1;
	}
	return connect_fd;
}

/**
 * 函数名：CreatTCPSocketServer
 * 功能：创建TCP socket 服务器
 * 参数：
 * 返回：
 * 备注：当前仅支持ip
 *
 */
int CreatTCPSocketServer(char* ip_name, int port, int block_flag)
{
	int listen_fd = -1,ret = -1;
	 struct sockaddr_in srv_addr = {0};
	listen_fd=socket(AF_INET,SOCK_STREAM,0);
	if(listen_fd < 0) {
		printf("<%s,%d> socket err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
		return -1;
	}
    //绑定信息，即命名socket
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(port);
    //inet_pton函数将用点分十进制字符串表示的IPv4/IPv6地址转化为用网络
    //字节序整数表示的IPv4/IPv6地址
    inet_pton(AF_INET, ip_name, &srv_addr.sin_addr);
	//bind sockfd&addr
	ret=bind(listen_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
	if(ret<0){
		printf("<%s,%d> bind err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
		close(listen_fd);
		return -1;
	}
	//listen sockfd
	ret=listen(listen_fd,5);
	if(ret<0){
		printf("<%s,%d> listen err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
	 close(listen_fd);
	 return -1;
	}
	return listen_fd; //ready to accept
}

/**
 * 函数名：ConnectTCPSocketServer
 * 功能：连接TCP socket 服务器
 * 参数：
 * 返回：
 * 备注：当前仅支持ip
 *
 */
int ConnectTCPSocketServer( char* ip_name, int port)
{
	int ret = -1,connect_fd = -1;
	struct sockaddr_in srv_addr = {0};
	connect_fd=socket(AF_INET,SOCK_STREAM,0);
	if(connect_fd  < 0) {
		printf("<%s,%d> socket err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
		return -1;
	}
	srv_addr.sin_family=AF_INET;
	srv_addr.sin_port = htons(port);
	srv_addr.sin_addr.s_addr = inet_addr(ip_name);
	ret=connect(connect_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
	if (ret<0) {
		printf("<%s,%d> connect err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
		close(connect_fd);
		return -1;
	}
	return connect_fd;
}

/**
 * 函数名：CreatUDPSocketServer
 * 功能：创建UDP socket服务器
 * 参数：
 * 返回：
 * 备注：当前仅支持ip
 *
 */
int CreatUDPSocketServer(char* ip_name, int port, long int ms)
{
	int sock_fd = -1,ret = -1;
		 struct sockaddr_in srv_addr = {0};
		 sock_fd=socket(AF_INET,SOCK_DGRAM,0);
		if(sock_fd < 0) {
			printf("<%s,%d> socket err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
			return -1;
		}
	    //绑定信息，即命名socket
	    srv_addr.sin_family = AF_INET;
	    srv_addr.sin_port = htons(port);
	    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		//bind sockfd&addr
		ret=bind(sock_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
		if(ret<0){
			printf("<%s,%d> bind err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
			close(sock_fd);
			return -1;
		}
		return sock_fd; //ready to recvform
}

/**
 * 函数名：ConnectUDPSocketServer
 * 功能：连接UDP socket服务器
 * 参数：
 * 返回：
 * 备注：当前仅支持ip
 *
 */
int ConnectUDPSocketServer(char* ip_name, int port)
{
	int sock_fd = -1;
	sock_fd=socket(PF_UNIX,SOCK_DGRAM,0);
	if(sock_fd  < 0) {
		printf("<%s,%d> socket err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
		return -1;
	}
	return sock_fd;
}

/**
 * 函数名：ReadSocket
 * 功能：读取Socket
 * 参数：
 * 返回：
 * 备注：
 *
 */
int ReadSocket(int socket_fd, void *buf, int buf_len, long int time_out_ms)
{
	int flags = 0,bak_flag = 0;
	fd_set rfds;
	struct timeval tv;
	int ret;

	if(buf_len <= 0) {
		printf("<%s,%d>please make sure buf_len:%d. \r\n",__func__,__LINE__,buf_len);
		return -1;
	}

	if(socket_fd < 0){
		printf("<%s,%d>err: socket_fd:%d \r\n",__func__,__LINE__,socket_fd);
		return -1;
	}

	//获取参数，保存参数,改为阻塞方式
	if ((flags = fcntl(socket_fd, F_GETFL, 0)) == -1) {
		flags = 0;
	}
	if(0 == (flags & O_NONBLOCK)) {
		flags &= ~O_NONBLOCK;
		fcntl(socket_fd, F_SETFL, flags);
		bak_flag = flags;
	}
	FD_ZERO(&rfds);
	FD_SET(socket_fd, &rfds);

	tv.tv_sec = time_out_ms/1000;
	tv.tv_usec = (time_out_ms)*1000;

	ret = select(socket_fd + 1, &rfds, NULL, NULL, &tv);
	if (ret > 0) {
		if(FD_ISSET(socket_fd, &rfds)) {
			usleep(100000);
			ret = recv(socket_fd, buf , buf_len , 0);
			if(ret < 0) {
				printf("<%s,%d> select err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
			}else if(0 == ret) {
				printf("<%s,%d>socket disconnect! \r\n",__func__,__LINE__);
				ret = -1;
			}
		}
	} else if(0 == ret) {
		//timeout
	} else {
		printf("<%s,%d> select err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
	}

	//还原参数
	if(0 != bak_flag) {
		fcntl(socket_fd, F_SETFL, bak_flag);
	}
	return ret;
}

/**
 * 函数名：WriteSocket
 * 功能：写入socket
 * 参数：
 * 返回：
 * 备注：
 *
 */
int WriteSocket(int socket_fd, void *buf, int buf_len, long int time_out_ms)
{

	int flags = 0,bak_flag = 0;
	fd_set rfds;
	struct timeval tv;
	int ret;

	if(buf_len <= 0) {
		printf("<%s,%d>please make sure buf_len:%d. \r\n",__func__,__LINE__,buf_len);
		return -1;
	}

	if(socket_fd < 0){
		printf("<%s,%d>err: socket_fd:%d \r\n",__func__,__LINE__,socket_fd);
		return -1;
	}

	//获取参数，保存参数,改为阻塞方式
	if ((flags = fcntl(socket_fd, F_GETFL, 0)) == -1) {
		flags = 0;
	}
	if(0 == (flags & O_NONBLOCK)) {
		flags &= ~O_NONBLOCK;
		fcntl(socket_fd, F_SETFL, flags);
		bak_flag = flags;
	}

	FD_ZERO(&rfds);
	FD_SET(socket_fd, &rfds);

	tv.tv_sec = time_out_ms/1000;
	tv.tv_usec = (time_out_ms)*1000;
	ret = select(socket_fd + 1, NULL, &rfds, NULL, &tv);
	if (ret > 0) {
		if(FD_ISSET(socket_fd, &rfds)){
			usleep(100000);
			ret = send(socket_fd, buf , buf_len , 0);

			if(ret < 0) {
				printf("<%s,%d> select err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
			}else if(0 == ret) {
				printf("<%s,%d>socket disconnect! \r\n",__func__,__LINE__);
				ret = -1;
			}

			if(0 == ret){
				ret = -1;
			}
		}
	} else if(0 == ret) {
		//timeout
	} else {
		printf("<%s,%d> select err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
	}

	//还原参数
	if(0 != bak_flag) {
		fcntl(socket_fd, F_SETFL, bak_flag);
	}

	return ret;
}


