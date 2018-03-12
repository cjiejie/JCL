/*
 * ini_rw.h
 *      function: 
 *      Created on: 2017��10��19��
 *      Author: cjiejie
 *     Mail: cjiejie@outlook.com  
 */

#ifndef _INI_H_
#ifdef __cplusplus
extern "C" {
#endif
#define  _INI_H_

/* ˵����ini�����ļ���д ini_rw
 * 1��֧��;��#ע�ͷ��ţ�֧����βע�͡�
 * 2��֧�ִ�����'��"�ɶ�ƥ����ַ�������ȡʱ�Զ�ȥ���š������пɴ��������Ż�;#ע�ͷ���
 * 3��֧����section���section(����Ϊ��)��
 * 4��֧��10��16��8��������0x��ͷΪ16��������0��ͷΪ8���ơ�
 * 5��֧��section��key��=��ǰ����ո�
 * 6��֧��\n��\r��\r\n��\n\r���и�ʽ��
 * 7��������section��key��Сд����д��ʱ���´�Ϊ׼�����������Сд��
 * 8����������ʱ����section�������ڸý����һ����Ч���ݺ����ӣ��������ļ�β�����ӡ�
 * 9��֧��ָ��key��������ɾ������ɾ���ü�ֵ������ע�͡�
 * 10�����Զ�������ʽ�����У��޸�ʱ��Ȼ������
 * 11���޸�ʱ����ԭע�ͣ���������ע�͡���βע��(����ǰ��ո�)��
 * 12���޸�ʱ����ԭ���С�����������Ҫ�Ǿ�������ԭ��ʽ��
 */
//����ini�ļ����ڴ�
int iniFileLoad(const char *filename);
//�ͷ�ini�ļ���ռ��Դ
void iniFileFree();
//��ȡ�ַ�������������
int iniGetString(const char *section, const char *key, char *value, int size, const char *defvalue);
//��ȡ����ֵ
int iniGetInt(const char *section, const char *key, int defvalue);
//��ȡ������
double iniGetDouble(const char *section, const char *key, double defvalue);
//�����ַ�������valueΪNULL����ɾ����key�����У�����ע��
int iniSetString(const char *section, const char *key, const char *value);
//��������ֵ��baseȡֵ10��16��8���ֱ��ʾ10��16��8���ƣ�ȱʡΪ10����
int iniSetInt(const char *section, const char *key, int value, int base);


#ifdef __cplusplus
}
#endif
#endif