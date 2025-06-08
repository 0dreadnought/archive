#include "../context/context.hpp"
#include <iostream>

class fiber{
    private:
        Context c;
        char* sb;
        char* st;
    public:
        fiber(void func()){
            char* dta = new char[4096];

            char* sp;
            uintptr_t tmp = (uintptr_t) sp;
            tmp = tmp& -16L;
            sp = (char*) tmp;
            sp -= 128;
            sp = dta + 4096*sizeof(char);  

            this->c.rip = (void*)func;
            this->c.rsp = sp;
            this->st = sp;
            this->sb = dta;
        };
        
        Context get_context(){
            return this->c;
        };
};

Context a;

void foo(){
    std::cout<<"FOO"<<std::endl;
    set_context(&a);
}

int main(){

    fiber f(foo);
    Context c;
    c = f.get_context();

    volatile int x = 1;
    get_context(&a);
    if(x){
        x=0;
        set_context(&c);
        }

    std::cout<<"FIN"<<std::endl;
    return 0;
}