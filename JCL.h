/*
 * JCL.h
 *      function: ���˿�ͷ�ļ�
 *      Created on: 2017��10��19��
 *      Author: cjiejie
 *     Mail: cjiejie@outlook.com  
 */

#ifndef _JCL_H_
#ifdef __cplusplus
extern "C" {
#endif
#define _JCL_H_

typedef enum {
	I_AUTO 			= 0x01 << 0, // �Զ�����
	I_FOCUS 		= 0x01 << 1, // �����Դ������ǿ��ɾ���ٴ���
} InitFlag_t;

void LibBuiltTime(void);

#ifdef __cplusplus
}
#endif
#endif
