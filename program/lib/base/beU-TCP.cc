// Nguyen Hai Duong
// Date : July 2016
// lib/TCP-IP.cc

#include "beU-TCP.h"

namespace BeU { 

SocketServer::SocketServer() {}
SocketServer::~SocketServer() {}

void SocketServer::initSocket(unsigned short port) {
	createTCPSocketServer(port);
	acceptTCPConnection();
}

int SocketServer::createTCPSocketServer(unsigned short port) {
    // Create socket for incoming connections 
    if ((servSock_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
        LOG_WARN << "ERROR opening socket";
    
    int reuse = 1;
    if (setsockopt(servSock_, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
        LOG_WARN << "setsockopt(SO_REUSEADDR) failed";

    #ifdef SO_REUSEPORT
        if (setsockopt(servSock_, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0) 
            LOG_WARN << "setsockopt(SO_REUSEPORT) failed";
    #endif

    // Construct local address structure 
    memset(&serv_addr, 0, sizeof(serv_addr));       /* Zero out structure */
    serv_addr.sin_family = PF_INET;                 /* Internet address family */
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  /* Any incoming interface */
    serv_addr.sin_port = htons(port);               /* Local port */

    // Bind to the local address
    if (bind(servSock_, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
        LOG_WARN << "ERROR on binding";

    // Mark the socket so it will listen for incoming connections 
    if (listen(servSock_, MAXPENDING) < 0)
        LOG_WARN << "ERROR on listening";

    return servSock_;
}

int SocketServer::acceptTCPConnection(){
    struct sockaddr_in cli_addr;        /* Client address */
    unsigned int clntLen;               /* Length of client address data structure */

    // Set the size of the in-out parameter 
    clntLen = sizeof(cli_addr);
    
    // Wait for a client to connect 
    if ((clntSock_ = accept(servSock_, (struct sockaddr *) &cli_addr, &clntLen)) < 0)
        LOG_WARN << "accept() failed";
    
    // LOG_VLOG   <<"Got a connection from " << inet_ntoa(cli_addr.sin_addr)
    //             <<" on port " 
    //             << htons(cli_addr.sin_port);

    return clntSock_;
}

int SocketServer::readMessage(char* msg) {
	int num = recv(clntSock_ , msg, BUFFSIZE, 0);
	if ( num == -1 ) 
        LOG_WARN << "ERROR reading from socket";
    else if( num > 0 ){
    	// success
    	msg[num] = '\0';
    }
    else {
        LOG_VERB << "Client disconnect !\n";
        acceptTCPConnection();
    }
    return num;
}

bool SocketServer::hasMessage() {
    char buff[BUFFSIZE];
    if(readMessage(buff)>1) {
        mesg = std::string(buff);
        return true;
    } else 
        return false;
    return "";
}

SocketClient::SocketClient() {
    sockfd_ = -1;
    status = false;
}
SocketClient::~SocketClient() {}

void SocketClient::initSocket() {
    status = createTCPClientSocket(serverIP.c_str(), port);
}

void SocketClient::closeSocket() {
    if(status)
        close(sockfd_);
}

int SocketClient::createTCPClientSocket(const char* serverIP, unsigned short port) {
    if ((sockfd_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        LOG_WARN << "socket() failed";
        return false;
    }
    /* set infor for socket */
    memset(&serv_addr, 0, sizeof(serv_addr));               /* Zero out structure */
    serv_addr.sin_family      = PF_INET;                    /* Internet address family */
    serv_addr.sin_addr.s_addr = inet_addr(serverIP);        /* Server IP address */
    serv_addr.sin_port        = htons(port);                /* Server port */

    if (connect(sockfd_,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        LOG_WARN << "ERROR connecting";
        return false;
    }
    return true;
}

size_t SocketClient::sendMessage(const char*msg) {
    status = createTCPClientSocket(serverIP.c_str(), port);
    if(status) {
        int n = send(sockfd_, msg, strlen(msg),0);
        closeSocket();
        if(n==-1)
            return 0;
        else
            return n;
    }
}

size_t SocketClient::sendMessage(std::string mesg) {
    return sendMessage(mesg.c_str());
}

}   // end of namespace TREE
