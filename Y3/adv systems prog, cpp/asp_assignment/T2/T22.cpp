#include "FiberScheduler.hpp"
#include <iostream>

scheduler s;

void foo(){
    std::cout<<"FOO"<<std::endl;
    s.fiber_exit();
}

void goo(){
    std::cout<<"GOO"<<std::endl;
    s.fiber_exit();
}

int main(){

    fiber f1(foo);
    fiber f2(goo);

    s.spawn(&f1);
    s.spawn(&f2);

    s.do_it();

    return 0;
}