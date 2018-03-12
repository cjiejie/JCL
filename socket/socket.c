/*
 * socket.c
 *      function: 
 *      Created on: 2018��3��10��
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

/**
 * ��������CreatLocalSocketServer
 * ���ܣ���������socket
 * ������
 * ���أ�
 * ��ע��
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
	// ���÷�������ַ����
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
 * ��������ConnectLocalSocket
 * ���ܣ����ӱ���socket
 * ������
 * ���أ�
 * ��ע��
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
 * ��������CreatTCPSocketServer
 * ���ܣ�����TCP socket ������
 * ������
 * ���أ�
 * ��ע����ǰ��֧��ip
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
    //����Ϣ��������socket
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(port);
    //inet_addr�������õ��ʮ�����ַ�����ʾ��IPv4��ַת��Ϊ������
    //�ֽ���������ʾ��IPv4��ַ
    srv_addr.sin_addr.s_addr = inet_addr(ip_name);
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
 * ��������ConnectTCPSocketServer
 * ���ܣ�����TCP socket ������
 * ������
 * ���أ�
 * ��ע����ǰ��֧��ip
 *
 */
int ConnectTCPSocketServer( char* ip_name, int port)
{
	int ret = -1,connect_fd = -1;
	struct sockaddr_in srv_addr = {0};
	connect_fd=socket(PF_UNIX,SOCK_STREAM,0);
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
 * ��������CreatUDPSocketServer
 * ���ܣ�����UDP socket������
 * ������
 * ���أ�
 * ��ע����ǰ��֧��ip
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
	    //����Ϣ��������socket
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
 * ��������ConnectUDPSocketServer
 * ���ܣ�����UDP socket������
 * ������
 * ���أ�
 * ��ע����ǰ��֧��ip
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
 * ��������ReadSocket
 * ���ܣ���ȡSocket
 * ������
 * ���أ�
 * ��ע����ǰ��֧��ip
 *
 */
int ReadSocket(int socket_fd, void *buf, int buf_len, long int time_out_ms)
{
	return 0;
}

/**
 * ��������WriteSocket
 * ���ܣ�д��socket
 * ������
 * ���أ�
 * ��ע����ǰ��֧��ip
 *
 */
int WriteSocket(int socket_fd, void *buf, int buf_len, long int time_out_ms)
{
	return 0;
}


