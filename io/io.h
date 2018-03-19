/*
 * io.h
 *      function: 
 *      Created on: 2018Äê3ÔÂ14ÈÕ
 *      Author: cjiejie
 *     Mail: cjiejie@outlook.com  
 */


int SelectRead(int socket_fd, void *buf, int buf_len, long int time_out_ms);
int SelectWrite(int socket_fd, void *buf, int buf_len, long int time_out_ms);
int EpollRead(int fd,char *buf, int read_len,int timeout_ms);
