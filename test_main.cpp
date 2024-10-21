#include <iostream>
#include <random>
#include <unistd.h>
#include <sstream>

#include "BaseFrame/Log.h"

class Derived {
public:

};

int main(int argc, char **argv) {
    std::printf("%zu\n", sizeof("HelloWorld"));

    std::cout << std::vector<int>().capacity() << std::endl;


    return 0;
}