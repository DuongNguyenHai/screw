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
#include <map>
#include <stdio.h>
#include <math.h>
#include "database.h"
#include "usb-port.h"
#include "ADC-converter.h"
using namespace BONE;

#define TURN_ON true
#define TURN_OFF false

#define FAILURE_CYCLE 0
#define PRESSURE_VACUUM 1

enum ModeWorking {
	modeNormal = 1,
	modeVacuum = 2
};

class ScrewMachine
{

public:
	ScrewMachine();
	~ScrewMachine();
	bool begin();					// start working
	bool restore();
	// Functions work with database
	void checkOutOfDateDocument();	// check the last document in LogRealTime and Product is old or not. If old -> create a new one
	// application
	void appPressureVacuum(bool state);
	std::vector<int> pressureSchedule;
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
	enum ModeWorking mode;
private:
	// Functions work with PLC
	void waitDataPLC();
	static void isAlivePLC();		// checking connection to PLC is still alive.
	Database dtbase;				// object connect to database
	struct tm currTime_;
	int lastDay_;
	std::thread *cycle_;
	static std::thread *thread_alive_;
	std::thread *thread_waitData_PLC_;
	std::thread *thread_schedule_;
	void scheduleWorking();

	// Functions work with database
	void cycleNewDocument(struct tm curr);		// creat new a ducument in LogRealTime after every 15 minutes and in Product after every 24 hour

	// application log total screws and failure times.
	void handleLogInfor(char c);
	// application Measure pressure of vacuum
	void handlePressureVacuum(struct tm curr);	// function to request selftest plc and measure vaccum
	ADC_Converter vacuum_;				// object vaccum
	std::atomic<bool> readyToMeasure_;	// waitting to measure pressure of vacuum
	std::atomic<bool> vacuum_enable_;	// enable / disable vaccum app

};


#endif
