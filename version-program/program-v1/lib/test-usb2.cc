// g++ -std=c++11 -Wall test-usb2.cc usb-port.cc -o test -lpthread
#include <iostream>
#include <unistd.h>
#include "usb-port.h"

int main(int argc, char const *argv[]) {
    StreamPortUSB port;
    while(true) {
        std::vector<std::string> list = port.listPort();
        for (size_t i = 0; i < list.size(); i++) {
            port.begin(list[i]);
            std::cout << "Open port: " << port.port << std::endl;
            sleep(2);
            port.end();
        }
    }
    sleep(100);
    pause();
    return 0;
}
