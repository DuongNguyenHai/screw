// g++ -std=c++11 -Wall test-usb.cc usb-port.cc -o test -lpthread
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include "usb-port.h"

int main(int argc, char const *argv[]) {

    StreamPortUSB port;
    std::vector<std::string> list = port.listPort();
    std::cout << list.size() << std::endl;
    if(list.size()>0) {
        std::cout << list[0] << std::endl;
    } else {
        std::cout << "No port is founded" << std::endl;
        return -1;
    }
    port.init(CHECKING_ALIVE);

    if(port.begin(list[0])) {
        std::cout << "Open port " << port.portName << " successful" << std::endl;
        // port.blocking(false);
    } else {
        std::cout << "Fail" << std::endl;
    }
    while(1) {
        if(port.stillAlive()) {
            // std::cout << "Port is opening" << std::endl;
        }
        else {
            // list.clear();
            // std::cout << "Port has closed" << std::endl;
            // list = port.listPort();
            // if(list.size()>0) {
            //     std::cout << list[0] << std::endl;
            //     if(port.begin(list[0])) {
            //         std::cout << "Open port " << port.portName << " successful" << std::endl;
            //         // port.blocking(false);
            //     } else {
            //         std::cout << "Fail" << std::endl;
            //     }
            // } else {
            //     std::cout << "No port is founded" << std::endl;
            // }

        }
        int avai = port.available();
    	if(avai > 0) {
            char c = port.readByte();
            std::cout << c << std::endl;
        }
        usleep(3000);
        // sleep(3);
    }
    return 0;
}
