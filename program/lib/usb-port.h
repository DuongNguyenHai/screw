// Nguyen Hai Duong
// November 2017

// reference :
// http://mirror.datenwolf.net/serial/#basics

#ifndef USB_PORT_H
#define USB_PORT_H

#include <stdio.h>   /* Standard input/output defbeginions */
#include <string.h>  /* String function defbeginions */
#include <unistd.h>  /* UNIX standard function defbeginions */
#include <fcntl.h>   /* File control defbeginions */
#include <errno.h>   /* Error number defbeginions */
#include <termios.h> /* POSIX terminal control defbeginions */
#include <thread>    // std::thread, std::this_thread::sleep_for
#include <iostream>
#include <vector>
#include <atomic>		  // atomic variable

#define BLOCKING true
#define NON_BLOCKING false
#define NONE 0
// baudrate : B9600 | B19200 | B57600 | B115200
#define BUFF_RX_MAX 32
#define CHECKING_ALIVE 1

class StreamPortUSB
{
public:
	StreamPortUSB();
	~StreamPortUSB();
	void begin();
	void begin(int fd);
	bool begin(const char dev[]);
	bool begin(std::string dev);
	bool beginPort();
	void setFD(int fd);
	int getFD();
	void init(int mode = CHECKING_ALIVE);
	void checkingAlive(bool state);
	void end();
	// set config: speed - baudrate, parity - 0/1 (0 mean is none, 1 is yes)
	int config(int speed, int parity);
	void blocking(bool block);
	// check if data is available. In asynchronous mode, the function will return number of bytes exist.
	// In synchronous mode. Function will return 1.
	int available();
	// read data from the port
	char readByte();
	int readBytes(char buff[], int nbytes);
	// write data to the port
	int writeData(char c);
	int writeData(char buff[], int nbytes);
	int writeData(char buff[]);
	int writeData(std::string buff);

	std::atomic<bool> alive;
	std::atomic<bool> working;
	bool stillAlive();
	static int timeCheckingAlive;
	static std::vector<std::string> listPort();
	std::string portName;
	bool asynchronous; // true -> set work in asynchronous mode. false -> synchronous
private:
	std::thread *rx_thread;
	std::thread *alive_thread;
	std::atomic<bool> alive_state_;
	void autoReceive();
	void howAlive();
	char buff_rx[BUFF_RX_MAX];
	std::atomic<int> rx_head;
	std::atomic<int> rx_tail;
	int fd_;
};

#endif
