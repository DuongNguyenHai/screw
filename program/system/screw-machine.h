// Nguyen Hai Duong
// November 2017

#ifndef SCREW_MACHINE_H
#define SCREW_MACHINE_H

#include <iostream>
#include <string.h>
#include <vector>
#include <time.h>
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <mutex>          // std::mutex
#include <chrono>         // std::chrono::seconds
#include <atomic>		  // atomic variable
#include <stdio.h>
#include <math.h>
#include "database.h"
#include "usb-port.h"
using namespace BONE;

class ScrewMachine
{

public:
	ScrewMachine();
	~ScrewMachine();
	bool begin();					// start working
	bool restore();
	// Functions work with database
	void cycleNewDocument();		// creat new a ducument in LogRealTime after every 15 minutes and in Product after every 24 hour,
	void checkOutOfDateDocument();	// check the last document in LogRealTime and Product is old or not. If old -> create a new one
	// Functions work with PLC
	void sayOk();					// send 'O' to PLC to say system had received data
	static size_t indentifyPLC();		// indentify PLC by id
	static void checkingAlivePLC();
	// variables
	static std::vector<std::string> listPort;		// port connect to plc
	static StreamPortUSB port;						// object to check port opened or closed.
	StreamPortUSB plc;								// object connect to plc
	std::atomic<bool> stateConnected;				// state discribe status of connection to PLC
	std::atomic<bool> working;						// state discribe status of object is working or not
	std::atomic<bool> started;						// state discribe status of object has started or not. it will be used to check to call restore() function
	static std::string dbName;		// database name
	std::vector<std::string> collection;
	std::string machineName;
	char plcID;						// id of PLC
	static uint32_t timeIndentifyPLC;
	static std::vector<ScrewMachine *> screws;
	static std::vector<ScrewMachine *> screwsDisconnect;
private:
	// Functions work with PLC
	void waitDataPLC();
	static void isAlivePLC();		// checking connection to PLC is still alive.
	Database dtbase;				// object connect to database
	struct tm currTime_;
	int lastDay_;
	std::thread *cycle_;
	static std::thread *alivePLCThread_;
	std::thread *plcWaitDataThread_;
};


#endif
