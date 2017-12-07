#include "communicate.h"

namespace BONE {

// int Socket::sendMessage(const char* msg) {}
static unsigned char CRC8(const unsigned char *data, unsigned char len) {
  unsigned char crc = 0x00;
  while (len--) {
    unsigned char extract = *data++;
    for (unsigned char tempI = 8; tempI; tempI--) {
      unsigned char sum = (crc ^ extract) & 0x01;
      crc >>= 1;
      if (sum) {
        crc ^= 0x8C;
      }
      extract >>= 1;
    }
  }
  return crc;
}

Socket::Socket() {}
Socket::~Socket() {}

void Socket::creatSocket(unsigned short port) {
	servSock_ = createTCPServerSocket(port);
	clntSock_ = acceptTCPConnection(servSock_);
}

int Socket::readMessage(char* msg) {
	int num = recv(clntSock_ , msg, BUFFSIZE, 0);
	if ( num == -1 ) 
        BONE_WARN << "ERROR reading from socket";
    else if( num > 0 ){
    	// success
    	msg[num] = '\0';
    }
    else {
        // BONE_VLOG << "Client disconnect !\n";
        clntSock_ = acceptTCPConnection(servSock_);
    }
    return num;
}

}