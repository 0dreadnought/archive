#include "../context/context.hpp"
#include <iostream>

Context a,b,c;

void foo(){
    volatile int x = 1;
    std::cout<<"FOOfty %"<<std::endl;
    get_context(&c);
    if(x){
        x=0;
        set_context(&b);
    }
    std::cout<<"FOO"<<std::endl;
    set_context(&a);
}

void goo(){
    std::cout<<"GOO"<<std::endl;
    set_context(&c);
}

int main(){

    char dtb[4096];
    char dtc[4096];

    char* sp;
    {
    uintptr_t tmp = (uintptr_t) sp;
    tmp = tmp& -16L;
    sp = (char*) tmp;
    }
    sp -= 128;
    sp = dtc + 4096;  

    c.rip = (void*) foo;
    c.rsp = sp;

    sp =dtb + 4069;
    b.rip = (void*) goo;
    b.rsp = sp;

    volatile int x = 1;
    get_context(&a);
    if(x){
        x=0;
        set_context(&c);
        }

    std::cout<<"FIN"<<std::endl;
    return 0;
}