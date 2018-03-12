/*
 * fifo.c
 *      function: 
 *      Created on: 2018��3��7��
 *      Author: cjiejie
 *     Mail: cjiejie@outlook.com  
 */
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "fifo.h"

/**
 * ��������FifoReadInit
 * ���ܣ�
 * ������
 * ���أ�
 * ��ע��
 *
 */
int FifoInit(const char *filename,mode_t mode)
{
	int ret = -1;
	if(-1 == access(filename, F_OK)) {
		ret = mkfifo(filename, 0777);
		if(-1 == ret) {
			printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
			return ret;
		}
	}

	ret = open(filename,mode);
	if(-1 == ret) {
		printf("<%s,%d> errno: %d %s\n",__func__,__LINE__,errno, strerror(errno));
		return ret;
	}
	return ret;
}











