#include "screw-machine.h"

ScrewMachine::ScrewMachine() {
	screws.push_back(this);
};
ScrewMachine::~ScrewMachine() {};

std::string ScrewMachine::dbName;
std::vector<ScrewMachine *> ScrewMachine::screws;
std::vector<std::string> ScrewMachine::listPort;

bool ScrewMachine::begin() {

	dtbase.begin(dbName.c_str());
	if(plc.begin()==false) {
		LOG_WARN << machineName << " -> " << "Cant not open \"" << plc.port << " \"port";
	}
	cycle_ = new std::thread(&ScrewMachine::cycleNewDocument, this);
	plcWaitDataThread_ = new std::thread(&ScrewMachine::waitDataPLC, this);
	return true;
}

void ScrewMachine::cycleNewDocument() {
	time_t now;
	struct tm curr;

	while(1)
	{
		now = time(NULL);
		localtime_r(&now, &curr);
		uint32_t hour = (curr.tm_hour);
		uint32_t min = (curr.tm_min);
		uint32_t secs = curr.tm_sec;

		if(hour==0) {
			if(curr.tm_yday > lastDay_) {
				LOG_VERB << machineName << " -> " << "Create a new document on " << collection[0].c_str();
				dtbase.insertNewDocument(collection[0].c_str());
				lastDay_ = curr.tm_yday;
			}
		}

		uint32_t timer = min <= 45 ? ((min/15 + 1)*15*60 - min*60 - secs) : (60*60 - min*60 - secs); // unit second
		sleep(timer);

		if(min>=45) {
			LOG_VERB << machineName << " -> " << "Create a new document on " << collection[1].c_str();
			dtbase.insertNewDocument(collection[1].c_str());
		}

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

void ScrewMachine::waitDataPLC() {
    while(1)
    {
    	int avai = plc.available();

    	if(avai > 0) {
    		char c = plc.readByte();
	        LOG_VERB << c;

	        switch(c) {
	        	// Draft failed
	        	case 'B': {
	        		dtbase.increaseDraftHand(collection[2].c_str(), HAND1, 1);
	        		dtbase.increaseDraftHand(collection[1].c_str(), HAND1, 1);
	        		dtbase.increaseDraftHand(collection[0].c_str(), HAND1, 1);
	        		LOG << machineName << " -> " << "Draft 1 failed !";
	        	} break;
	        	case 'E': {
	        		dtbase.increaseDraftHand(collection[2].c_str(), HAND2, 1);
	        		dtbase.increaseDraftHand(collection[1].c_str(), HAND2, 1);
	        		dtbase.increaseDraftHand(collection[0].c_str(), HAND2, 1);
	        		LOG << machineName << " -> " << "Draft 2 failed !";
	        	} break;
	        	// Feeder failed
	        	case 'C': {
	        		dtbase.increaseElement(collection[2].c_str(), FEEDER, 1);
	        		dtbase.increaseElement(collection[1].c_str(), FEEDER, 1);
	        		dtbase.increaseElement(collection[0].c_str(), FEEDER, 1);
	        		LOG << machineName << " -> " << "Feeder failed !";
	        	} break;
	        	// Fixture failed
	        	case 'D': {
	        		dtbase.increaseElement(collection[2].c_str(), FIXTURE, 1);
	        		dtbase.increaseElement(collection[1].c_str(), FIXTURE, 1);
	        		dtbase.increaseElement(collection[0].c_str(), FIXTURE, 1);
	        		LOG << machineName << " -> " << "Fixture failed !";
	        	} break;
	        	// Total of screw times.
	        	case 'T': {
	        		dtbase.increaseElement(collection[2].c_str(), TOTAL, 16);
	        		dtbase.increaseElement(collection[1].c_str(), TOTAL, 16);
	        		dtbase.increaseElement(collection[0].c_str(), TOTAL, 16);
	        		LOG_VERB << machineName << " -> " << "Screwed 16 times !";
	        	} break;
	        	default: {
	        		// PLC send c in range [41, 56] and we have to convert to range [1, 16]
	        		if(c>=41 && c<=56) {
	        			dtbase.increaseElement(collection[2].c_str(), SCREW, 1);
	        			dtbase.increaseElement(collection[1].c_str(), SCREW, 1);
	        			dtbase.increaseElement(collection[0].c_str(), SCREW, 1);
	        			dtbase.increaseScrewPosition(collection[2].c_str(), (int)c - 40, 1);
	        			dtbase.increaseScrewPosition(collection[1].c_str(), (int)c - 40, 1);
	        			dtbase.increaseScrewPosition(collection[0].c_str(), (int)c - 40, 1);
	        			LOG_VERB << machineName << " -> " << "Screw position failed: " << (int)c - 40 << " !";
	        		}
	        	}
	        }
    	}
    	usleep(10000); // 10ms. longer time will reduce consumption of cpu in pi
    }
}

void ScrewMachine::sayOk() {
	plc.writeData('O');
}

void ScrewMachine::indentifyPLC() {
	for (size_t i = 0; i < listPort.size(); i++) {
		StreamPortUSB port;
		port.begin(listPort[i]);
		LOG << listPort[i];
		for (size_t j = 0; j < 10; j++) {
			port.writeData('W');
			usleep(30000);
			if(port.available()) {
				
			}
		}
    }
}
