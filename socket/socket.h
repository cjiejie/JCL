/*
 * socket.h
 *      function: 
 *      Created on: 2018Äê3ÔÂ10ÈÕ
 *      Author: cjiejie
 *     Mail: cjiejie@outlook.com  
 */

int CreatLocalSocketServer(const  char *socket_name, int block_flag);
int ConnectLocalSocket(const  char *socket_name);

int CreatTCPSocketServer(char* ip_name, int port);
int ConnectTCPSocketServer( char* ip_name, int port);

int CreatUDPSocketServer(char* ip_name, int port, long int ms);
int ConnectUDPSocketServer(char* ip_name, int port,struct sockaddr *ser_addr);

int ReadSocket(int socket_fd, void *buf, int buf_len, long int time_out_ms);
int WriteSocket(int socket_fd, void *buf, int buf_len, long int time_out_ms);


