/*
 * shm.c
 *      function: 
 *      Created on: 2018年3月8日
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


#define READ_TO 1000 //读超时1000ms
#define WRITE_TO 1000  //写超时1000ms


//联合类型semun定义
union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};


/**
 * 函数名：ShmInit
 * 功能：
 * 参数：
 * 返回：
 * 备注：
 *
 */
int ShmInit(key_t key ,  Shm_t * data,InitFlag_t flag)
{
	int ret = -1;
	void *shm = NULL;//分配的共享内存的原始首地址
	if(key <= 0) {
		printf("<%s,%d> key must bigger than 0.\n",__func__,__LINE__);
		return ret;
	}
	//创建共享内存
	ret = shmget(key,sizeof(Shm_t),0777|IPC_CREAT);
	if(-1 == ret) {
		printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
		return ret;
	}
    switch(flag) {
		case I_FOCUS:
		    //将共享内存连接到当前进程的地址空间
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
	    //删除共享内存
	    if(shmctl(ret, IPC_RMID, 0) == -1)  {
	    	printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
	    }
	}
	return 0;
}

/**
 * 函数名：ShmRead
 * 功能：
 * 参数：
 * 返回：
 * 备注：
 *
 */
int ShmRead(int shm_id ,  Shm_t * data)
{
	void *shm = NULL;//分配的共享内存的原始首地址
    //将共享内存连接到当前进程的地址空间
	shm = shmat(shm_id, 0, 0);
    if(shm == (void*)-1) {
		printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
		return -1;
    }
    data = (Shm_t *)shm;
	return 0;
}

/**
 * 函数名：ShmWrite
 * 功能：
 * 参数：
 * 返回：
 * 备注：
 *
 */
int ShmWrite(int shm_id ,  Shm_t * data)
{
	void *shm = NULL;//分配的共享内存的原始首地址
    //将共享内存连接到当前进程的地址空间
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
		    //将共享内存连接到当前进程的地址空间
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

//函数：信号量P操作：对信号量进行减一操作
int SemaphoreP(int sem_id)
{
	struct sembuf sem_b;

	sem_b.sem_num = 0;//信号量编号
	sem_b.sem_op = -1;//P操作
	sem_b.sem_flg = SEM_UNDO;

	if(semop(sem_id,&sem_b,1) == -1) {
    	printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
		return 0;
	}

	return 1;
}


//函数：信号量V操作：对信号量进行加一操作
int SemaphoreV(int sem_id)
{
	struct sembuf sem_b;

	sem_b.sem_num = 0;//信号量编号
	sem_b.sem_op = 1;//V操作
	sem_b.sem_flg = SEM_UNDO;

	if(semop(sem_id,&sem_b,1) == -1)  {
		printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
		return -1;
	}

	return 0;
}

