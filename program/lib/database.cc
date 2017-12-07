#include "database.h"

Database::Database() {};
Database::~Database() {};

int Database::increaseElement(const char* COLL, int type, int value) {
	// get last id of last document via "total" field.
	int32_t last = 0;
	MongoDatabase::getValInt(COLL, "_id", "id", "total", last);
	last = (last - 1) < 0 ? 0 : last -1; // the first document indexed from 0, so need to abstract 1
	std::string ss;
	switch(type) {
		case TOTAL: ss = "total"; break;
		case SCREW: ss = "screw"; break;
		case FEEDER: ss = "feeder"; break;
		case FIXTURE: ss = "fixture"; break;
	}

	int32_t curr = 0;
	MongoDatabase::getValInt(COLL, "_id", last, ss.c_str(), curr);
	return MongoDatabase::update(COLL, "_id", last, ss.c_str(), curr+value);
}

int Database::increaseDraftHand(const char* COLL, int pos, int value) {
	// get last id of last document via "total" field.
	int32_t last = 0;
	MongoDatabase::getValInt(COLL, "_id", "id", "total", last);
	last = (last - 1) < 0 ? 0 : last -1; // the first document indexed from 0, so need to abstract 1
	int32_t curr = 0;
	std::string ss = DRAFT + std::string(".") + std::to_string(pos);
	MongoDatabase::getValInt(COLL, "_id", last, ss.c_str(), curr);
	return MongoDatabase::update(COLL, "_id", last, ss.c_str(), curr+value);
}

int Database::increaseScrewPosition(const char* COLL, int pos, int value) {
	// get last id of last document via "total" field.
	int32_t last = 0;
	MongoDatabase::getValInt(COLL, "_id", "id", "total", last);
	last = (last - 1) < 0 ? 0 : last -1; 	// the first document indexed from 0, so need to abstract 1
	int32_t curr = 0;
	std::string ss = SCREW_POSITION + std::string(".") + std::to_string(pos);
	MongoDatabase::getValInt(COLL, "_id", last, ss.c_str(), curr);
	return MongoDatabase::update(COLL, "_id", last, ss.c_str(), curr+value);
}

int Database::insertNewDocument(const char* COLL) {
	bson_t *doc = BCON_NEW(NULL);
	int res = MongoDatabase::insertInOrderWithDateTime(COLL, doc);
	bson_destroy(doc);
	return res;
}

int64_t Database::lastDateTime(const char *COLL) {
	// get last id of last document via "total" field.
	int32_t last = 0;
	int64_t lastTime = 0;
	if(MongoDatabase::getValInt(COLL, "_id", "id", "total", last)) {
		last = last - 1;	// the first document indexed from 0, so need to abstract 1
		MongoDatabase::getValDatetime(COLL, "_id", last, "date", lastTime);
		lastTime /= 1000;
	}
	return lastTime;
}