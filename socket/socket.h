/*
 * socket.h
 *      function: 
 *      Created on: 2018Äê3ÔÂ10ÈÕ
 *      Author: cjiejie
 *     Mail: cjiejie@outlook.com  
 */

int CreatLocalSocketServer(const  char *socket_name, int block_flag);
int ConnectLocalSocket(const  char *socket_name);

int CreatTCPSocketServer(SocketTypeEnum_t ip_type, char *ip_addr, int port, int block_flag);
int ConnectTCPSocketServer(SocketTypeEnum_t ip_type,char *ip_addr, int port);

int CreatUDPSocketServer(SocketTypeEnum_t ip_type, char *ip_addr, int port, long int ms);
int ConnectUDPSocketServer(SocketTypeEnum_t ip_type, char *ip_addr, int port);

int ReadSocket(int socket_fd, void *buf, int buf_len, long int time_out_ms);
int WriteSocket(int socket_fd, void *buf, int buf_len, long int time_out_ms);


