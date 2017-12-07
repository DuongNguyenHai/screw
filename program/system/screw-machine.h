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
	// Functions work with database
	void cycleNewDocument();		// creat new a ducument in LogRealTime after every 15 minutes and in Product after every 24 hour,
	void checkOutOfDateDocument();	// check the last document in LogRealTime and Product is old or not. If old -> create a new one
	// Functions work with PLC
	void sayOk();					// send 'O' to PLC to say system had received data
	// variables
	static std::vector<std::string> listPort;		// port connect to plc
	std::string collection[3];		// collection name of database
	StreamPortUSB plc;				// object connect to plc
	char plcID;
	static std::string dbName;		// database name
	std::string machineName;
	static std::vector<ScrewMachine *> screws;
	static void indentifyPLC();
private:
	// Functions work with PLC
	void waitDataPLC();
	Database dtbase;				// object connect to database
	struct tm currTime_;
	int lastDay_;
	std::thread *cycle_;
	std::thread *plcWaitDataThread_;
	std::atomic<bool> flagTotal_;
};


#endif
