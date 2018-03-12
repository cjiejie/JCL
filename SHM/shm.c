/*
 * shm.c
 *      function: 
 *      Created on: 2018��3��8��
 *      Author: cjiejie
 *     Mail: cjiejie@outlook.com  
 */

#include <stdio.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <sys/sem.h>

#include "shm.h"
#include "../JCL.h"


#define READ_TO 1000 //����ʱ1000ms
#define WRITE_TO 1000  //д��ʱ1000ms


//��������semun����
union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};


/**
 * ��������ShmInit
 * ���ܣ�
 * ������
 * ���أ�
 * ��ע��
 *
 */
int ShmInit(key_t key ,  Shm_t * data,InitFlag_t flag)
{
	int ret = -1;
	void *shm = NULL;//����Ĺ����ڴ��ԭʼ�׵�ַ
	if(key <= 0) {
		printf("<%s,%d> key must bigger than 0.\n",__func__,__LINE__);
		return ret;
	}
	//���������ڴ�
	ret = shmget(key,sizeof(Shm_t),0777|IPC_CREAT);
	if(-1 == ret) {
		printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
		return ret;
	}
    switch(flag) {
		case I_FOCUS:
		    //�������ڴ����ӵ���ǰ���̵ĵ�ַ�ռ�
		    shm = shmat(ret, 0, 0);
		    if(shm == (void*)-1) {
				printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
				return ret;
		    }
		    data = (Shm_t *)shm;
			memset(data->shm_buf,0,sizeof(data->shm_buf));
			break;
		case I_AUTO:
		default:
			break;
    }
	return ret;
}

int ShmDel(key_t key)
{
	int ret = -1;
	ret = shmget((key_t)1234, sizeof(Shm_t), 0777);
	if(ret >= 0) {
	    //ɾ�������ڴ�
	    if(shmctl(ret, IPC_RMID, 0) == -1)  {
	    	printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
	    }
	}
	return 0;
}

/**
 * ��������ShmRead
 * ���ܣ�
 * ������
 * ���أ�
 * ��ע��
 *
 */
int ShmRead(int shm_id ,  Shm_t * data)
{
	void *shm = NULL;//����Ĺ����ڴ��ԭʼ�׵�ַ
    //�������ڴ����ӵ���ǰ���̵ĵ�ַ�ռ�
	shm = shmat(shm_id, 0, 0);
    if(shm == (void*)-1) {
		printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
		return -1;
    }
    data = (Shm_t *)shm;
	return 0;
}

/**
 * ��������ShmWrite
 * ���ܣ�
 * ������
 * ���أ�
 * ��ע��
 *
 */
int ShmWrite(int shm_id ,  Shm_t * data)
{
	void *shm = NULL;//����Ĺ����ڴ��ԭʼ�׵�ַ
    //�������ڴ����ӵ���ǰ���̵ĵ�ַ�ռ�
	shm = shmat(shm_id, 0, 0);
    if(shm == (void*)-1) {
		printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
		return -1;
    }
    memcpy(shm,data,sizeof(Shm_t));
	return 0;
}

int SemInit(key_t key, InitFlag_t flag)
{
	int ret = -1;
    switch(flag) {
		case I_FOCUS:
		    //�������ڴ����ӵ���ǰ���̵ĵ�ַ�ռ�
			ret = semget(key, 1, 0777 | IPC_CREAT | IPC_EXCL);
		    if(ret <= 0) {
				printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
				return ret;
		    }
			break;
		case I_AUTO:
		default:
			ret = semget(key, 1, 0777 | IPC_CREAT);
			break;
    }

	return 0;
}

int SemDel(int sem_id)
{
	union semun sem_union;
	if(semctl(sem_id,0,IPC_RMID,sem_union)) {
		printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
		return -1;
	}
	return 0;
}

int SemSet(int sem_id)
{
	union semun sem_union;
	sem_union.val = 1;

	if(semctl(sem_id,0,SETVAL,sem_union)) {
		printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
		return -1;
	}
	return 0;
}

//�������ź���P���������ź������м�һ����
int SemaphoreP(int sem_id)
{
	struct sembuf sem_b;

	sem_b.sem_num = 0;//�ź������
	sem_b.sem_op = -1;//P����
	sem_b.sem_flg = SEM_UNDO;

	if(semop(sem_id,&sem_b,1) == -1) {
    	printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
		return 0;
	}

	return 1;
}


//�������ź���V���������ź������м�һ����
int SemaphoreV(int sem_id)
{
	struct sembuf sem_b;

	sem_b.sem_num = 0;//�ź������
	sem_b.sem_op = 1;//V����
	sem_b.sem_flg = SEM_UNDO;

	if(semop(sem_id,&sem_b,1) == -1)  {
		printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
		return -1;
	}

	return 0;
}

