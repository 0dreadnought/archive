#include <iostream>
#include <string>
#include "log.hpp"

int main() {
    log Log;
    Log.create_log("log.in");
    Log.next();
    std::cout << Log.line() << std::endl;
    std::cout << Log.error() << std::endl;
    Log.next();
    std::cout << Log.reformat() << std::endl;
    return 0;
}
