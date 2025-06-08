#include "Fibers.hpp"
#include <iostream>

scheduler s;

void foo(){
    int* dp = s.get_data();
    std::cout<<"FOO pt1: "<<*dp<<std::endl;
    *dp +=1;
    s.yeild();
    std::cout<<"FOO pt2: "<<*dp<<std::endl;
    s.fiber_exit();
}

void goo(){
    int* dp = s.get_data();
    std::cout<<"GOO pt1: "<<*dp<<std::endl;
    *dp +=1;
    s.yeild();
    std::cout<<"GOO pt2: "<<*dp<<std::endl;
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