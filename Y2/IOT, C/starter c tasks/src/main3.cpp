#include "log.hpp"
#include "../simpletest/simpletest.h"
#include <string>

using namespace std;

char const * groups[]={
    "log",
};

DEFINE_TEST_G(logTest,log){
    log Log;
    //checks if the log file is opened
    bool success = Log.create_log("../log.in");
    TEST_MESSAGE(success != true,"Failed to open log file");

    //tries to read next line
    success=Log.next();
    TEST_MESSAGE(success != true,"Failed to read log file");

    std::string line =Log.line();
    //Checks if line() function works properly
    TEST_MESSAGE(line.compare("Invalid operation")!= 0, "Expeсting: Invalid operation");

    line = Log.error();
    //Checks if error() function works properly
    TEST_MESSAGE(line.compare("ERROR")!= 0, "Expecting: ERROR");

    //tries to read next line
    success=Log.next();
    TEST_MESSAGE(success != true,"Failed to read log file");

    line = Log.reformat();
    //Checks if reformat() function works properly
    TEST_MESSAGE(line.compare("Operation completed (INFO)")!= 0, "Expecting: Operation completed (INFO)");
}

int main(){
    bool pass =true;

    for(auto group : groups){
        pass &= TestFixture::ExecuteTestGroup(group,TestFixture::Verbose);
    }

    return pass ? 0 : 1;
}