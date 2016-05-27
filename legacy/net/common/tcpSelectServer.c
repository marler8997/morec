#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "tcpSelectServer.h"

// returns error message if an error occurs
char * tcpSelectServer(
		    int listenPort,
		    int backlog,
		    char *readBuffer,
		    int readBufferLength,
		    unsigned char (*clientNewCallback)(int clientCount, int socket, struct sockaddr_in *addr),
		    unsigned char (*clientCloseCallback)(int clientCount, int socket),
		    unsigned char (*clientDataCallback)(int socket, char *buffer, int size)
) {
  int listenSocket;
  struct sockaddr_in listenSocketAddr;

  // Get Options
  int maxFileDescriptor = FD_SETSIZE-1;

  // Create Listen Socket
  listenSocket = socket(PF_INET, SOCK_STREAM, 0);
  if(listenSocket < 0) return "socket function failed";

  //printf("ListenFileDescriptor %d\n", listenSocket);

  // Create Listen Socket Address and Bind it
  listenSocketAddr.sin_family      = AF_INET;
  listenSocketAddr.sin_port        = htons(listenPort);
  listenSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(listenSocket, (struct sockaddr*)&listenSocketAddr, sizeof(listenSocketAddr)) < 0)
    return "bind function failed";
  
  // Start Listening
  if(listen(listenSocket, backlog) < 0)
    return "listen failed";

  // Initialize set of active sockets
  struct sockaddr_in newClientAddr;

  fd_set activeFDSet;
  FD_ZERO(&activeFDSet);
  FD_SET(listenSocket, &activeFDSet);

  unsigned char callbackResult;
  int clientCount = 0;

  while(1) {
    // Block until a socket pops
    fd_set readFDSet = activeFDSet;
    if(select(FD_SETSIZE, &readFDSet, NULL, NULL, NULL) < 0)
      return "select failed";

    // Server all sockets that popped
    for(int i = 0; i < FD_SETSIZE; i++) {
      if(FD_ISSET(i, &readFDSet)) {

	if(i == listenSocket) {
	  // Accept New Client
	  int addrSize = sizeof(newClientAddr);
	  int newSocket = accept(listenSocket, (struct sockaddr*) &newClientAddr, &addrSize);
	  if(newSocket < 0)
	    return "accept failed"; // TODO: should I return here?


	  clientCount++;
	  callbackResult = clientNewCallback(clientCount, newSocket, &newClientAddr);

	  if(callbackResult & TSS_CLIENT_CLOSE) {
	    clientCount--;
	    shutdown(newSocket, SHUT_RDWR);
	    close(newSocket);
	    
	    unsigned char newCallbackResult = clientCloseCallback(clientCount, newSocket);
	    if(newCallbackResult & TSS_STOP_SERVER) return NULL;
	  } else {
	    FD_SET(newSocket, &activeFDSet);
	  }

	  if(callbackResult & TSS_STOP_SERVER) return NULL;

	} else {
	  int bytesRead = read(i, readBuffer, readBufferLength);

	  if(bytesRead <= 0) {

	    if(bytesRead < 0) {
	      printf("Error \"read returned %d with file descriptor %d\"\n", bytesRead, socket);
	    }

	    FD_CLR(i, &activeFDSet);
	    clientCount--;

	    callbackResult = clientCloseCallback(clientCount, i);
	    if(callbackResult & TSS_STOP_SERVER) return NULL;

	  } else {

	    callbackResult = clientDataCallback(i, readBuffer, bytesRead);
	    if(callbackResult & TSS_CLIENT_CLOSE) {
	      shutdown(i, SHUT_RDWR);
	      close(i);

	      clientCount--;
	      FD_CLR(i, &activeFDSet);

	      unsigned char newCallbackResult = clientCloseCallback(clientCount, i);
	      if(newCallbackResult & TSS_STOP_SERVER) return NULL;
	    }
	    if(callbackResult & TSS_STOP_SERVER) return NULL;
	  }
	}
      }
    }

  }
}
