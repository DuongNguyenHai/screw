// Nguyen Hai Duong
// November 2017

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include "bone-log.h"
#include "database.h"
#include "screw-machine.h"
#include "usb-port.h"
#include "json.hpp"

using namespace nlohmann;
using namespace BONE;


int main(int argc, char const *argv[])
{
    json conf;              // configurattion
    ScrewMachine screw1;    // screw machine 1
    ScrewMachine screw2;    // screw machine 2

    try {
        conf = json::parse(std::ifstream("config.json"));
    }

    catch(json::parse_error &e) {
        std::cerr << "String json is not validate syntax : " << e.what() << std::endl;
        return -1;
    }

    // step 0: setup system
    PRINT_MONITOR = (conf["system"]["PRINT_MONITOR"].is_boolean()) ? (bool)(conf["system"]["PRINT_MONITOR"]) : true;
    PRINT_FILE = (conf["system"]["PRINT_FILE"].is_boolean()) ? (bool)(conf["system"]["PRINT_FILE"]) : true;
    PRINT_FILE_MONTH = (conf["system"]["PRINT_FILE_MONTH"].is_boolean()) ? (bool)(conf["system"]["PRINT_FILE_MONTH"]) : true;
    DEBUG_LEVEL = (conf["system"]["DEBUG_LEVEL"].is_number()) ? (int32_t)(conf["system"]["DEBUG_LEVEL"]) : 3;

    // step 1: setup database
    if(conf["system"]["database"] != NULL) {
        ScrewMachine::dbName = "ScrewMachine";
    } else {
        LOG_ERR << "You have not set the database name !";
    }

    // Set collection name
    if( conf["plc_1"]["collection_day"].is_string() && conf["plc_1"]["collection_hour"].is_string() && conf["plc_1"]["collection_minute"].is_string() ) {
        screw1.collection[0] = conf["plc_1"]["collection_day"];
        screw1.collection[1] = conf["plc_1"]["collection_hour"];
        screw1.collection[2] = conf["plc_1"]["collection_minute"];
    } else {
        LOG_ERR << "Set up fail collections for plc_1 !";
    }

    if( conf["plc_2"]["collection_day"].is_string() && conf["plc_2"]["collection_hour"].is_string() && conf["plc_2"]["collection_minute"].is_string() ) {
        screw2.collection[0] = conf["plc_2"]["collection_day"];
        screw2.collection[1] = conf["plc_2"]["collection_hour"];
        screw2.collection[2] = conf["plc_2"]["collection_minute"];
    } else {
        LOG_ERR << "Set up fail collections for plc_2 !";
    }
    // Set id of PLC
    if(conf["plc_1"]["id"].is_string()) {
        std::string ss = conf["plc_1"]["id"];
        screw1.plcID = ss[0];
        LOG << screw1.plcID;
    } else {
        LOG_ERR << "Set up fail id for plc_1";
    }

    if(conf["plc_2"]["id"].is_string()) {
        std::string ss = conf["plc_2"]["id"];
        screw2.plcID = ss[0];
        LOG << screw2.plcID;
    } else {
        LOG_ERR << "Set up fail id for plc_2";
    }
    // Indentifying what plc is connecting to usb port.
    ScrewMachine::listPort = StreamPortUSB::listPort();     // get list of usb what is opening

    LOG << "Indentifying the plc ...";
    ScrewMachine::indentifyPLC();

    for (auto & obj : ScrewMachine::screws) {
        LOG << obj->collection[0];
        LOG << obj->collection[1];
        LOG << obj->collection[2];
    }

    // step 2: setup port whats connecting to plc
    // screw1.plc.port = "/dev/ttyUSB0";
    // screw2.plc.port = "/dev/ttyUSB1";
    // // step 3: start working
    // screw1.begin();
    // screw2.begin();
    //
    // // step 4: check out what document in database is old. we will create a new doct with the right time (system time)
    // screw1.checkOutOfDateDocument();
    // screw2.checkOutOfDateDocument();

    // pause this main thread. Everything work in screw-machine.cc
    pause();

	return 0;
}
