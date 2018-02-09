// Nguyen Hai Duong
// Date : july 2016
// lib/TCP-IP.h

#ifndef BEU_TCP_IP_H
#define BEU_TCP_IP_H

#include <stdio.h>		// for printf() and fprintf()
#include <sys/socket.h> // for socket(), bind(), and connect()
#include <arpa/inet.h>  // for sockaddr_in and inet_ntoa()
#include <stdlib.h>     // for atoi() and exit()
#include <string.h>		// for memset()
#include <unistd.h>		// for sleep(), close()
#include <sys/types.h>	// for Socket data types
#include <netinet/in.h> // for IP Socket data types
#include <pthread.h>	// for multithread function
#include <sys/wait.h>	// for waitpid()
#include <sys/time.h>   // for struct timeval()
#include <fcntl.h>      // for fcntl()

// for log and error
#include "beU-log.h"

namespace BeU {

#define MAXPENDING 5
#define BUFFSIZE 256

class SocketServer {
public:
	SocketServer();
	~SocketServer();
	void initSocket(unsigned short port);
    // Create tcp server socket
    int createTCPSocketServer(unsigned short port);
    // accept tcp connection
    // Return server socket
    int acceptTCPConnection();
	int sendMessage(const char*msg);
	int readMessage(char *msg);
	bool hasMessage();
	unsigned int listen_port;
	std::string mesg;
    struct sockaddr_in serv_addr;
private:
	int servSock_;
	int clntSock_;
};

class SocketClient {
public:
	SocketClient();
	~SocketClient();
	void initSocket();
	void closeSocket();
    // Create tcp client socket
    int createTCPClientSocket(const char* serverIP, unsigned short port);
	size_t sendMessage(const char*msg);
	size_t sendMessage(std::string mesg);
	std::string serverIP;
	unsigned short port;
    struct sockaddr_in serv_addr;
	bool status;
private:
	int sockfd_;
};


}	// end of name space TREE

#endif