/*
 * io.c
 *      function: 
 *      Created on: 2018��3��14��
 *      Author: cjiejie
 *     Mail: cjiejie@outlook.com  
 */

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/epoll.h>

#define MAX_EVENT_NUMBER 6

/**
 * ��������SetNonblock
 * ���ܣ�����һ���ļ�������Ϊnonblock
 * ������
 * ���أ�
 * ��ע��
 */
int SetNonblock(int fd)
{
	int flags;
	if ((flags = fcntl(fd, F_GETFL, 0)) == -1) {
		flags = 0;
	}
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

/**
 * ��������ReadSocket
 * ���ܣ���ȡSocket
 * ������
 * ���أ�
 * ��ע��
 *
 */
int SelectRead(int socket_fd, void *buf, int buf_len, long int time_out_ms)
{

	int flags = 0,bak_flag = 0;
	fd_set rfds;
	struct timeval tv;
	int ret;

	if(socket_fd < 0){
		printf("<%s,%d>err: socket_fd:%d \r\n",__func__,__LINE__,socket_fd);
		return -1;
	}

	//��ȡ�������������,��Ϊ������ʽ
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
	tv.tv_usec = (time_out_ms%1000)*1000;
	ret = select(socket_fd + 1, &rfds, NULL, NULL, &tv);
	if (ret > 0) {
		if(FD_ISSET(socket_fd, &rfds)){
			usleep(100000);
			ret = send(socket_fd, buf , buf_len , 0);
//			ret = read(socket_fd,buf,buf_len);
			if(0 == ret){
				ret = -1;
			}
		}
	} else if(0 == ret) {
		//timeout
	} else {
		printf("<%s,%d> select err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
	}

	//��ԭ����
	if(0 != bak_flag) {
		fcntl(socket_fd, F_SETFL, bak_flag);
	}

	return ret;
}

/**
 * ��������SelectWrite
 * ���ܣ���ʱд��
 * ������
 * ���أ�
 * ��ע��
 *
 */
int SelectWrite(int socket_fd, void *buf, int buf_len, long int time_out_ms)
{

	int flags = 0,bak_flag = 0;
	fd_set rfds;
	struct timeval tv;
	int ret;

	if(socket_fd < 0){
		printf("<%s,%d>err: socket_fd:%d \r\n",__func__,__LINE__,socket_fd);
		return -1;
	}

	//��ȡ�������������,��Ϊ������ʽ
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
	tv.tv_usec = (time_out_ms%1000)*1000;
	ret = select(socket_fd + 1, NULL, &rfds, NULL, &tv);
	if (ret > 0) {
		if(FD_ISSET(socket_fd, &rfds)){
			usleep(100000);
			ret = send(socket_fd, buf , buf_len , 0);
//			ret = read(socket_fd,buf,buf_len);
			if(0 == ret){
				ret = -1;
			}
		}
	} else if(0 == ret) {
		//timeout
	} else {
		printf("<%s,%d> select err: errno[%d], %s\n",__func__,__LINE__,errno, strerror(errno));
	}

	//��ԭ����
	if(0 != bak_flag) {
		fcntl(socket_fd, F_SETFL, bak_flag);
	}

	return ret;
}

int EpollRead(int fd,char *buf, int read_len,int timeout_ms)
{
	int len = -1,flags = 0,bak_flag = 0;
	int i,e_num = -1;
	struct epoll_event event = {0};
	struct epoll_event events[MAX_EVENT_NUMBER] = {{0}};
	event.events = EPOLLET | EPOLLIN;
	event.data.fd = fd;

	int epid = epoll_create(MAX_EVENT_NUMBER);  // ����epoll����������ɹ����˳�ʱ�ǵùر�
	if(epid < 0) {
		printf("<%s,%d>epoll_create error,ret[%d]\n", __func__,__LINE__,epid);
		return -1;
	}
	int ret = epoll_ctl(epid, EPOLL_CTL_ADD, fd, &event);
	if (ret < 0) {
		printf("<%s,%d> set epoll error,ret[%d]:%s.\n", __func__,__LINE__,ret,strerror(errno));
		epoll_ctl(epid, EPOLL_CTL_DEL, fd, &event);
		close(epid);
		return -1;
	}

	//��ȡ�������������,��Ϊ������ʽ
	if ((flags = fcntl(fd, F_GETFL, 0)) == -1) {
		flags = 0;
	}
	if(0 == (flags & O_NONBLOCK)) {
		flags &= ~O_NONBLOCK;
		fcntl(fd, F_SETFL, flags);
		bak_flag = flags;
	}

	e_num = epoll_wait(epid, events, MAX_EVENT_NUMBER, timeout_ms);
	if (e_num == 0)  {//|| errno== EAGAIN ) //timeout
		epoll_ctl(epid, EPOLL_CTL_DEL, fd, &event);
		close(epid);
		//��ԭ����
		if(0 != bak_flag) {
			fcntl(fd, F_SETFL, bak_flag);
		}
		return -1;
	}
	for (i = 0; i < e_num; i++) {
		if ((events[i].data.fd == fd)&&(events[i].events & EPOLLIN)) {// fdƥ�䲢�ҿɶ�
			len = read(fd, buf, read_len);
		}
	}

	epoll_ctl(epid, EPOLL_CTL_DEL, fd, &event);
	close(epid);

	//��ԭ����
	if(0 != bak_flag) {
		fcntl(fd, F_SETFL, bak_flag);
	}

	return len;
}

