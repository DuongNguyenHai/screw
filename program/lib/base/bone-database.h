// Nguyen Hai Duong
// Date : Oct 2016

#ifndef BONE_DATABASE_H
#define BONE_DATABASE_H

#include <stdio.h>
#include <vector>
#include <string.h>
#include <inttypes.h>			// int64_t
#include <stdarg.h>
#include <mongoc.h>
#include "bone-log.h"

#ifndef RET_MISS
	#define RET_MISS -1
#endif
#ifndef RET_SUCCESS
	#define RET_SUCCESS 0
#endif
#ifndef RET_FAILURE
	#define RET_FAILURE 1
#endif

namespace BONE {

#define GMT 0		// Ho Chi Minh, Viet Nam

typedef mongoc_collection_t db_collection;

typedef struct {
   int started;		// The total of query command
   int succeeded;	// number of succession
   int failed;		// number of failure
   int missed;		// number of query had not implemented its purpose
} stats_t;

class MongoDatabase {

public:
	MongoDatabase();
	MongoDatabase(const char *databaseName);
	~MongoDatabase();
	void begin(const char *databaseName);

	int initOrder(const char *COLL_NAME);			// create an index document.
	int insert(const char *COLL_NAME, char* json); // insert only one data (document) to a collection
	int insertInOrder(const char *COLL_NAME, char* json); // insert a data which has "_id" auto increment
	int insert(const char* COLL_NAME, const bson_t *insert);
	int insertInOrder(const char* COLL_NAME, const bson_t *insert);
	int insertInOrderWithDateTime(const char* COLL_NAME, const bson_t *insert);
	int insertInOrderWithDateTime(const char* COLL_NAME, const bson_t *insert, time_t tm);

	int update(const char *COLL_NAME, char *json, char *jsonSelector); // update only one data to collection
	// key_select also can be in nest, ex: foo.baz
	int update(const char *COLL_NAME, const char *key_select, const char *val_select, const char *key_update, bool val_update);
	int update(const char *COLL_NAME, const char *key_select, const char *val_select, const char *key_update, int32_t val_update);
	int update(const char *COLL_NAME, const char *key_select, const char *val_select, const char *key_update, int64_t val_update);
	int update(const char *COLL_NAME, const char *key_select, const char *val_select, const char *key_update, double val_update);
	int update(const char *COLL_NAME, const char *key_select, const char *val_select, const char *key_update, int val_update[], unsigned int length_arr);
	int update(const char *COLL_NAME, const char *key_select, const char *val_select, const char *key_update, std::vector<int> val_update);
	int update(const char *COLL_NAME, const char *key_select, int val_select, const char *key_update, std::vector<int> val_update);

	int update(const char *COLL_NAME, const char *key_select, int val_select, const char *key_update, int32_t val_update);
	int update(const char *COLL_NAME, const char *key_select, int val_select, const char *key_update, int64_t val_update);
	int update(const char *COLL_NAME, const char *key_select, int val_select, const char *key_update, double val_update);

	int remove(const char *COLL_NAME, const bson_t *query); // delete only one data
	int32_t totalDocuments(const char *COLL_NAME); // count total documents in a specific colletion
	int32_t countDocuments(const char *COLL_NAME);				// Display stats
	std::string getJsonDocument(const char *COLL_NAME, char *jsonSelecColl); // return json document
	std::string getJsonOneDocument(const char *COLL_NAME, bson_t *query); // // return json document
	bson_t getDocument(const char *COLL_NAME, bson_t *query);	// return bson_t document

	bool getValInt(const char *COLL_NAME, const char *field, const char *val_field, const char * key, int32_t &res);
	bool getValInt(const char *COLL_NAME, const char *field, const int val_field, const char * key, int32_t &res);
	bool getValDatetime(const char *COLL_NAME, const char *field, const int val_field, const char * key, int64_t &res);

	// Append array field to bson
	template <size_t rows, size_t cols>
	bool appendBsonArray(bson_t *doc, const char *key, int (&array)[rows][cols]);

private:
	mongoc_database_t 	*dtb;		// var for database
	mongoc_client_t     *client;	// var for client connect to database
	const char *dbName;				// the name of database.
	int insertQuick(const char *COLL_NAME, const bson_t *insert);
	int updateQuick(const char *COLL_NAME, bson_t *query, bson_t *update);
	int updateQuick(const char *COLL_NAME, const char *key_select, const char *val_select, bson_t *update);
	int updateQuick(const char *COLL_NAME, const char *key_select, int32_t val_select, bson_t *update);
	int getNextSequence(db_collection *colt);
};

} // end of namspace TREE


#endif
