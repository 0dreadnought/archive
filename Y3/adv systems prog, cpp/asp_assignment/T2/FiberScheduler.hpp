#pragma once
#include "../context/context.hpp"
#include <iostream>
#include <deque>

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

class scheduler{
    private:
        std::deque<fiber*> fibers;
        Context c;
    public:
        scheduler(){

        };
        
        void spawn(fiber* f){
            fibers.push_back(f);
        };
        void do_it(){
            get_context(&c);
            if(fibers.size()){
                Context tmp;
                tmp = fibers.front()->get_context();
                fibers.pop_front();
                set_context(&tmp);
            }

        };
        void fiber_exit(){
            set_context(&c);
        };
};