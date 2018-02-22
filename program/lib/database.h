#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <sstream>
#include "base/beU-database.h"
#include "base/beU-TCP.h"
#include "base/json.hpp"
using namespace BeU;
using namespace nlohmann;

#define DRAFT "draft"
#define SCREW_POSITION "screwPosition"
#define VACUUM "vaccumPressure"
#define TOTAL 0
#define SCREW 1
#define FEEDER 3
#define FIXTURE 4
#define HAND1 1
#define HAND2 2

class NetWorkDatabase: public SocketClient {
public:
	NetWorkDatabase();
	~NetWorkDatabase();
	void init();
	std::string combineJson(std::string key, std::string value);
	size_t upload(std::string str);
	size_t upload(std::string key, std::string value);
};

class Database : public MongoDatabase
{

public:
	Database();
	~Database();
	// Overlap for save data in database
	int increaseElement(const char* COLL, int type, int value);
	int increaseDraftHand(const char* COLL, int pos, int value);
	int increaseScrewPosition(const char* COLL, int pos, int value);
	int insertNewDocument(const char* COLL);
	int insertPressureVacuum(const char* COLL, float data);
	// Local database
	int increaseElementLocal(const char* COLL, int last, std::string type, int value);
	int increaseDraftHandLocal(const char* COLL, int last, std::string ss, int pos, int value);
	int increaseScrewPositionLocal(const char* COLL, int last, std::string ss, int pos, int value);
	int insertNewDocumentLocal(const char* COLL);
	int insertPressureVacuumLocal(const char* COLL, float data);
	// Online database
	int increaseElementOnline(std::string MAC, std::string COLL, std::string type, int value);
	int increaseDraftHandOnline(std::string MAC, std::string COLL, int pos, int value);
	int increaseScrewPositionOnline(std::string MAC, std::string COLL, int pos, int value);
	int insertNewDocumentOnline(std::string MAC, std::string COLL);
	int insertPressureVacuumOnline(std::string MAC, std::string COLL, float data);

	int64_t lastDateTime(const char *COLL);	// unix time by seconds
	NetWorkDatabase network;
	std::string machineMAC;
};

#endif
