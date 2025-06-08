#include <iostream>
#include "../context/context.hpp"

Context c;

int main(){
    volatile int x = 0;
    get_context(&c);
    std::cout<<"M"<<std::endl;
    if(!x){
        x++;
        set_context(&c);
    }
    return 0;
}