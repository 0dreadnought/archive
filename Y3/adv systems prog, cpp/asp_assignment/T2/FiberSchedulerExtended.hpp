#pragma once
#include "../context/context.hpp"
#include <iostream>
#include <deque>

class fiber{
    private:
        Context c;
        char* sb;
        char* st;
        int* dp;
    public:
        fiber(void func(),int* pointer = nullptr){
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
            this->dp = pointer;
        };
        
        Context get_context(){
            return this->c;
        };

        int* get_data(){
            return dp;
        }
};

class scheduler{
    private:
        std::deque<fiber*> fibers;
        Context c;
        int* dp;
    public:
        scheduler(int* pointer = nullptr){
            this->dp = pointer;
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

        int* get_data(){
            return dp;
        };
};