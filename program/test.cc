#include <iostream>
#include <vector>
#include <atomic>		  // atomic variable

class foo {

public:
    foo();
    ~foo();
    int get();
    int val;
    // std::atomic<int> k;  // issue here
};

foo::foo() {}
foo::~foo() {}
int foo::get() { return val;}

int main(int argc, char const *argv[]) {

    std::vector<foo> vec;

    for (size_t i = 0; i < 5; i++) {
        foo a;
        a.val = i;
        vec.push_back(a);
    }

    for (size_t i = 0; i < vec.size(); i++) {
        std::cout << vec[i].get() << std::endl;
    }

    return 0;
}
