/*
 * msg.h
 *      function: 
 *      Created on: 2018��3��7��
 *      Author: cjiejie
 *     Mail: cjiejie@outlook.com  
 */

#include "../JCL.h"

#ifndef INCLUDE_MSG_H_
#define INCLUDE_MSG_H_

#define MSG_BUFSIZE 		1024
#define QUEUE_SIZE			4096

//�Զ������ݽṹ
typedef struct {
    int type;
    char buf[MSG_BUFSIZE];
} MyMsg_t;

//�������Ϣ�ṹ
typedef struct {
    long int msg_type;
    MyMsg_t msg_buff;
} Msg_t;

typedef struct {
	int in;
	int out;
	int size;
	char buf[QUEUE_SIZE];
} CircularQueue_t;

int MsgInit(key_t key,InitFlag_t my_flag);

int MsgDeinit(key_t key);

int MsgRcv(int msg_id, Msg_t *msg_stu, long int read_type, int msg_flag );

int MsgSnd(int msg_id, Msg_t *msg_stu, int msg_flag );

#endif
