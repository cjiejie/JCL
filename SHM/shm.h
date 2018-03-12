/*
 * shm.h
 *      function: 
 *      Created on: 2018��3��8��
 *      Author: cjiejie
 *     Mail: cjiejie@outlook.com  
 */

#ifndef INCLUDE_SHM_H_
#define INCLUDE_SHM_H_

#include "../JCL.h"

//�������Ϣ�ṹ
typedef struct {
	int index; //��Ϊһ����ʶ��Ԥ��
    char shm_buf[1024]; //��¼д��Ͷ�ȡ������
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
