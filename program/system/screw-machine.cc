#include "screw-machine.h"

float convertKpa(float x);

ScrewMachine::ScrewMachine() {
	stateConnected = false;
	working = false;
	started = false;
	screws.push_back(this);
};
ScrewMachine::~ScrewMachine() {};

StreamPortUSB ScrewMachine::port;
std::string ScrewMachine::dbName;
std::vector<ScrewMachine *> ScrewMachine::screws;
std::vector<std::string> ScrewMachine::listPort;
std::thread *ScrewMachine::thread_alive_;
uint32_t ScrewMachine::timeIndentifyPLC = 5;	// unit seconds

bool ScrewMachine::begin() {
	dtbase.begin(dbName.c_str());
	plc.init(CHECKING_ALIVE);
	plc.begin(plc.portName);
	if(vacuum_enable_) {
		vacuum_.addModule(ADDRESS_ADC);    // add the first module. index of this module will be 0
	    vacuum_.begin();
	}
	// cycle_ = new std::thread(&ScrewMachine::cycleNewDocument, this);
	thread_waitData_PLC_ = new std::thread(&ScrewMachine::waitDataPLC, this);
	thread_schedule_ = new std::thread(&ScrewMachine::scheduleWorking, this);
	stateConnected = true;
	working = true;
	started = true;
	readyToMeasure_ = false;
	return true;
}

bool ScrewMachine::restore() {
	plc.begin(plc.portName);
	stateConnected = true;
	working = true;
	return true;
}

void ScrewMachine::cycleNewDocument(struct tm curr) {

	uint32_t hour = curr.tm_hour;
	uint32_t min = curr.tm_min;
	uint32_t secs = curr.tm_sec;

	if( hour==0 && min==0 && secs==0 ) {
		if(curr.tm_yday > lastDay_) {
			LOG_VERB << machineName << " -> " << "Create a new document on " << collection[0].c_str();
			dtbase.insertNewDocument(collection[0].c_str());
			lastDay_ = curr.tm_yday;
		}
	}

	if( min==0 && secs==0 ) {
		LOG_VERB << machineName << " -> " << "Create a new document on " << collection[1].c_str();
		dtbase.insertNewDocument(collection[1].c_str());
	}
	if( min%15==0 && secs==0 ) {
		LOG_VERB << machineName << " -> " << "Create a new document on " << collection[2].c_str();
		dtbase.insertNewDocument(collection[2].c_str());
	}
}

void ScrewMachine::checkOutOfDateDocument() {
	time_t tm = time(NULL);
	localtime_r(&tm, &currTime_);
	lastDay_ = currTime_.tm_yday;
	currTime_.tm_hour = 0;
	currTime_.tm_min = 0;
	currTime_.tm_sec = 0;

	time_t today = mktime(&currTime_);	// GMT is 7 (Ho Chi Minh, Viet Nam)

	int64_t lastTimeDay = dtbase.lastDateTime(collection[0].c_str());
	int lastTimeHour = dtbase.lastDateTime(collection[1].c_str());
	int64_t lastTimeMinute = dtbase.lastDateTime(collection[2].c_str());

	if(lastTimeDay < today) {
		LOG_VERB << machineName << " -> " << "(old document) Create a new doucment on " << collection[0].c_str();
		dtbase.insertNewDocument(collection[0].c_str());
	}

	if(lastTimeHour <= tm - 60*60) {
		LOG_VERB << machineName << " -> " << "(old document) Create a new document on " << collection[1].c_str();
		dtbase.insertNewDocument(collection[1].c_str());
	}

	if(lastTimeMinute <= tm - 15*60) {
		LOG_VERB << machineName << " -> " << "(old document) Create a new document on " << collection[2].c_str();
		dtbase.insertNewDocument(collection[2].c_str());
	}
}

void ScrewMachine::handleLogInfor(char c) {
	switch(c) {
		// Draft failed
		case 'B': {
			for (size_t i = 0; i < collection.size(); i++)
				dtbase.increaseDraftHand(collection[i].c_str(), HAND1, 1);
			LOG << machineName << " -> " << "Draft 1 failed !";
		} break;
		case 'E': {
			for (size_t i = 0; i < collection.size(); i++)
				dtbase.increaseDraftHand(collection[i].c_str(), HAND2, 1);
			LOG << machineName << " -> " << "Draft 2 failed !";
		} break;
		// Feeder failed
		case 'C': {
			for (size_t i = 0; i < collection.size(); i++)
				dtbase.increaseElement(collection[i].c_str(), FEEDER, 1);
			LOG << machineName << " -> " << "Feeder failed !";
		} break;
		// Fixture failed
		case 'D': {
			for (size_t i = 0; i < collection.size(); i++)
				dtbase.increaseElement(collection[i].c_str(), FIXTURE, 1);
			LOG << machineName << " -> " << "Fixture failed !";
		} break;
		// Total of screw times.
		case 'T': {
			for (size_t i = 0; i < collection.size(); i++)
				dtbase.increaseElement(collection[i].c_str(), TOTAL, 16);
			LOG_VERB << machineName << " -> " << "Screwed 16 times !";
		} break;
		default: {
			// PLC send c in range [41, 56] and we have to convert to range [1, 16]
			if(c>=41 && c<=56) {
				for (size_t i = 0; i < collection.size(); i++) {
					dtbase.increaseElement(collection[i].c_str(), SCREW, 1);
					dtbase.increaseScrewPosition(collection[i].c_str(), (int)c - 40, 1);
				}
				LOG_VERB << machineName << " -> " << "Screw position failed: " << (int)c - 40 << " !";
			}
		}
	}
}

void ScrewMachine::waitDataPLC() {
    while(true)
    {
		if(stateConnected) {
			int avai = plc.available();
	    	if(avai > 0) {
	    		char c = plc.readByte();
		        LOG_VERB << machineName << ":" << c;
				// check self-test
				if(c=='S') {
					readyToMeasure_ = true;
				} else {
					handleLogInfor(c);
				}
				// switch (mode) {
				// 	case modeVacuum: readyToMeasure_ = true; break;
				// 	case modeNormal: handleLogInfor(c); break;
				// }
	    	}
		}
    	usleep(10000); // 10ms. longer time will reduce consumption of cpu in pi
    }
}

void ScrewMachine::sayOk() {
	plc.writeData('O');
}

size_t ScrewMachine::indentifyPLC() {
	bool brk = false;
	int count = 0;

	for (size_t i = 0; i < listPort.size(); i++) {
		bool foundPLC = false;
		bool portExist = false;
		// Checking port is in used
		for(auto & screw: screws) {
			if(screw->stateConnected) {
				if(listPort[i].compare(screw->plc.portName)==0)
					portExist = true;
			}
		}
		if(portExist==false) {
			port.begin(listPort[i]);
			{
				LOG << "Indentifying PLC on port: " << listPort[i];
				for (size_t j = 0; j < 15; j++) {
					port.writeData('w');
					sleep(1);
					if(port.available()) {
						char c = port.readByte();
						for(auto & screw: screws) {
							if(screw->stateConnected)
								continue;
							if(c == screw->plcID) {
								screw->plc.portName = listPort[i];
								LOG << "Detected PLC with id=\""<< screw->plcID <<"\" on \"" << listPort[i] << "\" port";
								port.writeData('o');
								screw->stateConnected = true;
								brk = true;
								foundPLC = true;
								count++;
								break;
							}
						}
					} else {
						port.writeData('o');
					}
					if(brk) {brk=false; break;}
				}
				if(!foundPLC) {
					LOG_WARN << "Indentifying PLC on port: " << listPort[i] << " FAIL";
				}
			}
			port.end();
		}
    }
	return count;
}

void ScrewMachine::checkingAlivePLC() {
	thread_alive_ = new std::thread(ScrewMachine::isAlivePLC);
}

void ScrewMachine::isAlivePLC() {
	while(true) {
		bool somePLCfail = false;
		for(auto & screw: screws) {
			if(screw->plc.stillAlive()==false) {
				somePLCfail = true;
				screw->stateConnected = false;
				screw->working = false;
				LOG_WARN << "PLC (" << screw->plcID <<") is not working";
			}
		}
		if(somePLCfail) {
			LOG_VERB << "Cheking all port again to identify PLC";
			listPort = StreamPortUSB::listPort();
			size_t count = indentifyPLC();
			if(count>0) {
				for(auto & screw: screws) {
					if(screw->stateConnected==true && screw->working==false) {
						if(screw->started==false) {
							LOG << "Starting work with PLC (" << screw->plcID <<") on port " << screw->plc.portName;
							screw->begin();
							screw->checkOutOfDateDocument();
						}
						else {
							LOG << "Restoring work with PLC (" << screw->plcID <<") on port " << screw->plc.portName;
							screw->restore();
						}
					}
				}
			}
		}
		sleep(timeIndentifyPLC);
	}
}

void ScrewMachine::appPressureVacuum(bool state) {
	if(state) {
		vacuum_enable_ = true;
	} else {
		vacuum_enable_ = false;
	}
}

void ScrewMachine::handlePressureVacuum(struct tm curr) {
	// ready to measure
	if(readyToMeasure_) {
		float avr = 0;
		float data[5];
		for (size_t k = 0; k < 5; k++) {
			data[k] = convertKpa(vacuum_.readVoltage(MODULE_1, CHANEL_0));
			avr += data[k];
		}
		avr = (float)(avr/5);
		LOG << machineName << ": Pressure of vacuum on avarage: " << avr;
		dtbase.insertPressureVacuum(collection[0].c_str(), avr);
		usleep(100000);
		plc.writeDate3Times('o');
		readyToMeasure_ = false;
	}
	// check schedule
	else {
		int tm_seconds = curr.tm_hour*3600 + curr.tm_min*60 + curr.tm_sec;
		for (size_t i = 0; i < pressureSchedule.size(); i++) {
			if(pressureSchedule[i] == tm_seconds) {
				LOG << machineName << ": Request self-test mode";
				for (size_t k = 0; k < 3; k++) {
					if(readyToMeasure_) break;
					plc.writeData('s');
					usleep(300000);
				}
				break;
			}
		}
	}
}

void ScrewMachine::scheduleWorking() {
	time_t now;
	struct tm curr;

	for(;;) {
		now = time(NULL);
		localtime_r(&now, &curr);

		// check time to create new document for log data
		cycleNewDocument(curr);

		// application: measure pressure of vacuum
		// if(vacuum_enable_)
		// 	handlePressureVacuum(curr);

		sleep(1);
	}
}

float convertKpa(float x) {
  return (float)((x-min_val) * (pressure_max - pressure_min) / (max_val-min_val) + pressure_min);
}
