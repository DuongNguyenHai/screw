#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#include <stdio.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include "config.h"
#include "TCP-IP.h"
#include "bone-log.h"

namespace BONE {

// Test passed: June 31 2017
class Socket {
public:
	Socket();
	~Socket();
	void creatSocket(unsigned short port);
	int sendMessage(const char*msg);
	int readMessage(char *msg);

private:
	int servSock_;
	int clntSock_;
};

}
#endif

