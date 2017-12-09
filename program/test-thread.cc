// g++ -std=c++11 -Wall test-thread.cc -o test -lpthread

#include <thread>    // std::thread, std::this_thread::sleep_for
#include <iostream>
#include <vector>
#include <atomic>		  // atomic variable
#include <unistd.h>

class bar {

public:
    bar();
    ~bar();
    void init();
    void begin();
    void end();
    void foo();
    void foo2();
    std::thread th;
    std::thread th2;
    volatile std::atomic<bool> working;
};

bar::bar() {}
bar::~bar() {}

void bar::init() {

}

void bar::begin() {
    working = true;
    th = std::move(std::thread(&bar::foo,this));
    th.detach();

    th2 = std::move(std::thread(&bar::foo2,this));
    th2.detach();
}

void bar::end() {
    working = false;
}

void bar::foo() {
    while (working) {
        if(true) {
            std::cout << "thread 1 is working !" << std::endl;
            sleep(1);
        }
    }
}

void bar::foo2() {
    while (working) {
        if(true) {
            std::cout << "thread 2 is working !" << std::endl;
            sleep(1);
        }
    }
}

int main(int argc, char const *argv[]) {

    bar test;
    test.init();
    while(1) {
        test.begin();
        sleep(1);
        test.end();
    }
    pause();
    return 0;
}
