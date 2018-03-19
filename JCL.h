/*
 * JCL.h
 *      function: 个人库头文件
 *      Created on: 2017年10月19日
 *      Author: cjiejie
 *     Mail: cjiejie@outlook.com  
 */

#ifndef _JCL_H_
#ifdef __cplusplus
extern "C" {
#endif
#define _JCL_H_

typedef enum {
	I_AUTO 			= 0x01 << 0, // 自动创建
	I_FOCUS 		= 0x01 << 1, // 如果资源存在则强制删除再创建
} InitFlag_t;

void LibBuiltTime(void);

#ifdef __cplusplus
}
#endif
#endif
