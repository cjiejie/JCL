/*
 * msg.c
 *      function: 
 *      Created on: 2018年3月7日
 *      Author: cjiejie
 *     Mail: cjiejie@outlook.com  
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/msg.h>

#include "msg.h"

/**
 * 函数名：MsgInit
 * 功能：
 * 参数：
 * 返回：
 * 备注：
 *
 */
int MsgInit(key_t key,InitFlag_t my_flag)
{
	int ret = -1;
	if(key <= 0) {
		printf("<%s,%d> key must bigger than 0.\n",__func__,__LINE__);
		return ret;
	}
	switch(my_flag) {
	case I_FOCUS:
		ret = msgget(key,IPC_CREAT|IPC_EXCL);
		if(-1 == ret) { //消息队列存在，先获取到将其删除，再创建
			ret = msgget(key,IPC_CREAT);
			msgctl(ret,IPC_RMID,0);
			ret = msgget(key,IPC_CREAT);
		}
		break;
	case I_AUTO:
	default:
		ret = msgget(key,IPC_CREAT);
		break;
	}
	if(-1 == ret) {
		printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
		return ret;
	}

	return ret;
}

/**
 * 函数名：MsgDeinit
 * 功能：
 * 参数：
 * 返回：
 * 备注：
 *
 */
int MsgDeinit(key_t key)
{
	return msgctl(key,IPC_RMID,0);
}

/**
 * 函数名：MsgRcv
 * 功能：
 * 参数：
 * 返回：
 * 备注：
 *
 */
int MsgRcv(int msg_id, Msg_t *msg_stu, long int read_type, int msg_flag )
{
	int ret = -1;
	ret = msgrcv(msg_id,msg_stu,sizeof(Msg_t)-sizeof(long int),read_type,msg_flag);
	if(ret < 0) {
		printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
		return ret;
	} else {
		ret = read_type;
	}

	return ret;
}

/**
 * 函数名：MsgSnd
 * 功能：
 * 参数：
 * 返回：
 * 备注：
 *
 */
int MsgSnd(int msg_id, Msg_t *msg_stu, int msg_flag )
{
	int ret = -1;
	ret = msgsnd(msg_id,msg_stu,sizeof(Msg_t)-sizeof(long int),msg_flag);
	if(ret < 0) {
		printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
		return ret;
	}

	return ret;
}

/**
 * 函数名：QueueInit
 * 功能：
 * 参数：
 * 返回：
 * 备注：
 *
 */
int QueueInit(CircularQueue_t *q)
{
	q->in = q->out = 0;
	q->size = sizeof(q->buf);
	memset(q->buf,0,sizeof(q->buf));
	return 0;
}

/**
 * 函数名：QueueLen
 * 功能：查询队列内容长度
 * 参数：
 * 返回：
 * 备注：
 *
 */
int QueueLen(CircularQueue_t *q)
{
	if(q->in >= q->out) {
		return (q->in - q->out);
	} else {
		return (q->size - q->out + q->in);
	}
}

/**
 * 函数名：QueueLenAvail
 * 功能：查询队列剩余长度
 * 参数：
 * 返回：
 * 备注：
 *
 */
int QueueLenAvail(CircularQueue_t *q)
{
	if((q->in + 1) % (q->size) == q->out) {
		return 0;
	} else {
		return (q->size - QueueLen(q) - 1);
	}
}

/**
 * 函数名：QueueIn
 * 功能：插入队列
 * 参数：
 * 返回：
 * 备注：
 *
 */
int QueueIn(CircularQueue_t *q,char *data, int len)
{
	if((len <=0) || (len > sizeof(q->buf))) {
		printf("<%s,%d> err: length out of rand.\r\n",__func__,__LINE__);
	}
	int cnt = 0;
	for(;len>0;len--) {
		q->buf[q->in] = data[cnt++];
		q->in = (q->in+1)%(q->size);
	}
	return 0;
}


/**
 * 函数名：QueueOut
 * 功能：从读列中取数据
 * 参数：
 * 返回：
 * 备注：
 *
 */
int QueueOut(CircularQueue_t *q,char *buf, int len)
{
	int cnt = 0,data_len = 0;
	data_len = QueueLen(q);
	if(0 == data_len) {
		printf("<%s,%d> queue empty. \r\n",__func__,__LINE__);
		return 0;
	} else {
		printf("<%s,%d> err: queue data len[%d]. \r\n",__func__,__LINE__,data_len);
		return -1;
	}
	if(data_len > len) {
		data_len = len;
	}
	for(;data_len > 0;data_len--) {
		buf[cnt++] = q->buf[q->out];
		q->out = (q->out + 1) % (q->size);
	}
	return data_len;
}

/**
 * 函数名：QueuePeek
 * 功能：从循环队列里面拷贝数据出来
 * 参数：
 * 返回：
 * 备注：
 *
 */
int QueuePeek(CircularQueue_t *q,char *buf, int len)
{
	int cnt = 0, out = 0;
	int data_len = QueueLen(q);
	if(data_len <= 0) {
		printf("<%s,%d>err: queue is empty. \r\n",__func__,__LINE__);
		return 0;
	}
	if(data_len >= len) {
		data_len = len;
	}
	out = q->out;
	for(;data_len > 0;data_len--) {
		buf[cnt++] = q->buf[out];
		out = (out+1) % (q->size);
	}
	return cnt;
}



