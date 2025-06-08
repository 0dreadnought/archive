#include "FiberSchedulerExtended.hpp"
#include <iostream>

scheduler s;

void foo(){
    std::cout<<"FOO"<<std::endl;
    int* dp = s.get_data();
    std::cout<<"FOO: "<<*dp<<std::endl;
    *dp +=1;
    s.fiber_exit();
}

void goo(){
    int* dp = s.get_data();
    std::cout<<"GOO: "<<*dp<<std::endl;
    s.fiber_exit();
}

int main(){

    int d = 10;
    int* dp = &d;

    s = scheduler(dp);

    fiber f1(foo,dp);
    fiber f2(goo,dp);

    s.spawn(&f1);
    s.spawn(&f2);

    s.do_it();

    return 0;
}