/*
 * shm.h
 *      function: 
 *      Created on: 2018年3月8日
 *      Author: cjiejie
 *     Mail: cjiejie@outlook.com  
 */

#ifndef INCLUDE_SHM_H_
#define INCLUDE_SHM_H_

#include "../JCL.h"

//传输的消息结构
typedef struct {
	int index; //作为一个标识，预留
    char shm_buf[1024]; //记录写入和读取的内容
} Shm_t;

int ShmInit(key_t key ,  Shm_t * data,InitFlag_t flag);
int ShmDel(key_t key);
int ShmRead(int shm_id ,  Shm_t * data);
int ShmWrite(int shm_id ,  Shm_t * data);

int SemInit(key_t key, InitFlag_t flag);
int SemDel(int sem_id);
int SemSet(int sem_id);
int SemaphoreP(int sem_id);
int SemaphoreV(int sem_id);

#endif
