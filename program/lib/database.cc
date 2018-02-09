#include "database.h"

NetWorkDatabase::NetWorkDatabase() {}
NetWorkDatabase::~NetWorkDatabase() {}

void NetWorkDatabase::init() {
	SocketClient::initSocket();
}

size_t NetWorkDatabase::upload(std::string mesg) {
	return SocketClient::sendMessage(mesg);
}

Database::Database() {};
Database::~Database() {};

int Database::increaseElement(const char* COLL, int type, int value) {
	std::string ss;
	switch(type) {
		case TOTAL: ss = "total"; break;
		case SCREW: ss = "screw"; break;
		case FEEDER: ss = "feeder"; break;
		case FIXTURE: ss = "fixture"; break;
	}

	int res = increaseElementLocal(COLL, ss, value);
	int resOnline = increaseElementOnline(machineMAC, std::string(COLL), ss, value);

	if ((res==0)&&(resOnline==0))
		return 0;
	else return 1;
}

int Database::increaseDraftHand(const char* COLL, int pos, int value) {
	int res = increaseDraftHandLocal(COLL, pos, value);
	int resOnline = increaseDraftHandOnline(machineMAC, COLL, pos, value);
	if ((res==0)&&(resOnline==0))
		return 0;
	else return 1;
}

int Database::increaseScrewPosition(const char* COLL, int pos, int value) {
	int res = increaseScrewPositionLocal(COLL, pos, value);
	int resOnline = increaseScrewPositionOnline(machineMAC, COLL, pos, value);
	if ((res==0)&&(resOnline==0))
		return 0;
	else return 1;
}

int Database::insertNewDocument(const char* COLL) {
	int res = insertNewDocumentLocal(COLL);
	int resOnline = insertNewDocumentOnline(machineMAC, COLL);
	if ((res==0)&&(resOnline==0))
		return 0;
	else return 1;
}

int Database::insertPressureVacuum(const char* COLL, float data) {
	int res = insertPressureVacuumLocal(COLL, data);
	int resOnline = insertPressureVacuumOnline(machineMAC, COLL, data);
	if ((res==0)&&(resOnline==0))
		return 0;
	else return 1;
}

int Database::increaseElementLocal(const char* COLL, std::string type, int value) {
	// get last id of last document via "total" field.
	int32_t last = 0;
	MongoDatabase::getValInt(COLL, "_id", "id", "total", last);
	last = (last - 1) < 0 ? 0 : last -1; // the first document indexed from 0, so need to abstract 1
	int32_t curr = 0;
	MongoDatabase::getValInt(COLL, "_id", last, type.c_str(), curr);
	return MongoDatabase::update(COLL, "_id", last, type.c_str(), curr+value);
}

int Database::increaseDraftHandLocal(const char* COLL, int pos, int value) {
	// get last id of last document via "total" field.
	int32_t last = 0;
	MongoDatabase::getValInt(COLL, "_id", "id", "total", last);
	last = (last - 1) < 0 ? 0 : last -1; // the first document indexed from 0, so need to abstract 1
	int32_t curr = 0;
	std::string ss = DRAFT + std::string(".") + std::to_string(pos);
	MongoDatabase::getValInt(COLL, "_id", last, ss.c_str(), curr);
	return MongoDatabase::update(COLL, "_id", last, ss.c_str(), curr+value);
}

int Database::increaseScrewPositionLocal(const char* COLL, int pos, int value) {
	// get last id of last document via "total" field.
	int32_t last = 0;
	MongoDatabase::getValInt(COLL, "_id", "id", "total", last);
	last = (last - 1) < 0 ? 0 : last -1; 	// the first document indexed from 0, so need to abstract 1
	int32_t curr = 0;
	std::string ss = SCREW_POSITION + std::string(".") + std::to_string(pos);
	MongoDatabase::getValInt(COLL, "_id", last, ss.c_str(), curr);
	return MongoDatabase::update(COLL, "_id", last, ss.c_str(), curr+value);
}

int Database::insertNewDocumentLocal(const char* COLL) {
	bson_t *doc = BCON_NEW(NULL);
	int res = MongoDatabase::insertInOrderWithDateTime(COLL, doc);
	bson_destroy(doc);
	return res;
}

int Database::insertPressureVacuumLocal(const char* COLL, float data) {
	int last = 0;
	MongoDatabase::getValInt(COLL, "_id", "id", "total", last);
	last = (last - 1) < 0 ? 0 : last -1; 	// the first document indexed from 0, so need to abstract 1
	return MongoDatabase::update(COLL, "_id", last, VACUUM, data);
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

int Database::increaseElementOnline(std::string MAC, std::string COLL, std::string type, int value) {
	json js = {
		{"MAC", MAC},
		{"colection", COLL},
		{"type", type},
		{"value", value}
	};
	
	return network.upload(js.dump());
}

int Database::increaseDraftHandOnline(std::string MAC, std::string COLL, int pos, int value) {
	json js = {
		{"MAC", MAC},
		{"colection", COLL},
		{"pos", pos},
		{"value", value}
	};
	
	return network.upload(js.dump());
}

int Database::increaseScrewPositionOnline(std::string MAC, std::string COLL, int pos, int value) {
	json js = {
		{"MAC", MAC},
		{"colection", COLL},
		{"pos", pos},
		{"value", value}
	};
	
	return network.upload(js.dump());
}

int Database::insertNewDocumentOnline(std::string MAC, std::string COLL) {
	json js = {
		{"MAC", MAC},
		{"colection", COLL}
	};
	
	return network.upload(js.dump());
}

int Database::insertPressureVacuumOnline(std::string MAC, std::string COLL, float data) {
	json js = {
		{"MAC", MAC},
		{"colection", COLL},
		{"value", data}
	};
	
	return network.upload(js.dump());
}
