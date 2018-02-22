// Nguyen Hai Duong
// November 2017

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include "beU-log.h"
#include "database.h"
#include "beU-TCP.h"
#include "usb-port.h"
#include "json.hpp"
#include "screw-machine.h"

using namespace nlohmann;
using namespace BeU;

int main(int argc, char const *argv[])
{
    json conf;              // configurattion

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

    // step 2: setup PLC and collection of database will be saved data of PLC
    size_t numPLC = (conf["PLC"]["setting"].is_array()) ? (int32_t)(conf["PLC"]["setting"].size()) : 0;
    if(numPLC == 0) {
        LOG_ERR << "\"number\" of plc is 0 !";
    }
    // create list PLC
    ScrewMachine listMachine[numPLC];

    for (size_t i = 0; i < numPLC; i++) {
        // Set id of PLC
        if(conf["PLC"]["setting"][i]["id"].is_string()) {
            std::string ss = conf["PLC"]["setting"][i]["id"];
            listMachine[i].plcID = ss[0];
        } else {
            LOG_ERR << "Set up fail PLC/setting[" << i << "]/id";
        }

        // Set MAC address of machine, its will be identify machine in database
        if(conf["PLC"]["setting"][i]["MAC"].is_string()) {
            listMachine[i].dtbase.machineMAC = conf["PLC"]["setting"][i]["MAC"];
        } else {
            LOG_ERR << "Set up fail PLC/setting[" << i << "]/MAC";
        }

        // set machine name
        if(conf["PLC"]["setting"][i]["machineName"].is_string()) {
            listMachine[i].machineName = conf["PLC"]["setting"][i]["machineName"];
        }

        // Set collection name
        for (size_t j = 0; j < conf["PLC"]["setting"][i]["collection"].size(); j++) {
            if(conf["PLC"]["setting"][i]["collection"][j].is_string()) {
                listMachine[i].collection.push_back(conf["PLC"]["setting"][i]["collection"][j]);
            } else {
                LOG_ERR << "Set up fail PLC/setting/collection[" << j << "]";
            }
        }

        // Setup application
        // enable/disable measuring pressure of vacuum
        if(conf["PLC"]["setting"][i]["appPressureVacuum"].is_boolean()) {
            listMachine[i].appPressureVacuum(conf["PLC"]["setting"][i]["appPressureVacuum"]);
            if(conf["PLC"]["setting"][i]["pressureVacuumSchedule"].is_array()) {
                std::vector<int> v = conf["PLC"]["setting"][i]["pressureVacuumSchedule"];
                listMachine[i].pressureSchedule = v;
            }
        }
    }

    // setup port for tcp/ip network
    // SocketServer mailBox;
    // mailBox.listen_port = (conf["system"]["listen_port"].is_number()) ? (int32_t)(conf["system"]["listen_port"]) : 8888;
    
    // setup pathway to server (TCP-IP method)
    // SocketClient mailman;
    // mailman.serverIP = (conf["system"]["serverIP"].is_string()) ? (conf["system"]["serverIP"]) : "127.0.0.1";
    // mailman.port = (conf["system"]["server_port"].is_number()) ? (int32_t)(conf["system"]["server_port"]) : 8880;
    // mailman.initSocket();

    // Indentifying plc in network
    // Indentifying what plc is connecting to usb port.
    ScrewMachine::listPort = StreamPortUSB::listPort();     // get list of usb what is opening
    ScrewMachine::port.init();                              // setup mode to indentifyPLC
    ScrewMachine::indentifyPLC();

    // Start working
    for(auto & screw: ScrewMachine::screws) {
        if(screw->stateConnected) { // assurance screw has connected successful.
            LOG << "Starting work with PLC (" << screw->plcID <<") on port " << screw->plc.portName;
            screw->begin();
            // check out what document in database is old. we will create a new doct with the right time (system time)
            screw->checkOutOfDateDocument();
        }
    }
    // this function will maintain port which is connected to plc.
    ScrewMachine::checkingAlivePLC();
    
    listMachine[0].dtbase.begin("ScrewMachine");
    listMachine[0].dtbase.network.serverIP = "127.0.0.1";
    listMachine[0].dtbase.network.port = 5152;
    listMachine[0].dtbase.network.init();
    // LOG << "Why";
    while(true) {
        // mailman.sendMessage("what the hell");
        // listMachine[0].dtbase.network.upload("whats about now !");
        listMachine[0].dtbase.increaseElement("ScrewByDay_1", FEEDER, 1);
        sleep(1);
        listMachine[0].dtbase.increaseDraftHand("ScrewByDay_1", 2, 1);
        sleep(1);
        listMachine[0].dtbase.increaseScrewPosition("ScrewByDay_1", 15, 1);
        // sleep(1);
        // listMachine[0].dtbase.insertNewDocument("ScrewByDay_1");
        // sleep(1);
        // listMachine[0].dtbase.insertPressureVacuum("ScrewByDay_1", 40);
        sleep(5);
    }
    
    /*
    // Waitting message from server.
    mailBox.initSocket(mailBox.listen_port);
    while(true) {
        if(mailBox.hasMessage()) {
            LOG << mailBox.mesg;
        }
    }
    */
    // pause this main thread. Everything work in screw-machine.cc
    pause();

	return 0;
}
