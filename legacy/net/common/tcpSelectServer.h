#ifndef TCP_SELECT_SERVER_H
#define TCP_SELECT_SERVER_H

#include <netinet/in.h>


#define TSS_CLIENT_CLOSE 0x01
#define TSS_STOP_SERVER  0x02

char * tcpSelectServer(
		    int listenPort,
		    int backlog,
		    char *readBuffer,
		    int readBufferLength,
		    unsigned char (*clientNewCallback)(int clientCount, int socket, struct sockaddr_in *addr),
		    unsigned char (*clientCloseCallback)(int clientCount, int socket),
		    unsigned char (*clientDataCallback)(int socket, char *buffer, int size)
		    );

#endif
