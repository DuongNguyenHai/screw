#ifndef DATABASE_H
#define DATABASE_H

#include "base/bone-database.h"
using namespace BONE;

#define DRAFT "draft"
#define SCREW_POSITION "screwPosition"
#define VACUUM "vaccumPressure"
#define TOTAL 0
#define SCREW 1
#define FEEDER 3
#define FIXTURE 4
#define HAND1 1
#define HAND2 2

class Database : public MongoDatabase
{

public:
	Database();
	~Database();
	int increaseElement(const char* COLL, int type, int value);
	int increaseDraftHand(const char* COLL, int pos, int value);
	int increaseScrewPosition(const char* COLL, int pos, int value);
	int insertNewDocument(const char* COLL);
	int insertPressureVacuum(const char* COLL, float data);
	int64_t lastDateTime(const char *COLL);	// unix time by seconds
};

#endif
