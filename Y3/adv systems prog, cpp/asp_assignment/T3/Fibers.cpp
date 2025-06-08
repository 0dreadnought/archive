#pragma once
#include "Fibers.hpp"
#include <iostream>

fiber::fiber(void func(),int* pointer){
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
}

Context& fiber::get_context(){
    return this->c;
}

int* fiber::get_data(){
    return this->dp;
}


scheduler::scheduler(int* pointer){
    this->current_fiber = nullptr;
    this->dp = pointer;
}

void scheduler::spawn(fiber* f){
    fibers.push_back(f);
}

void scheduler::do_it(){
    get_context(&c);
    if(fibers.size()){
        this->current_fiber = fibers.front();//remember the fiber in case of yeild
        fibers.pop_front();
        set_context(&this->current_fiber->get_context());
    }
}

void scheduler::fiber_exit(){
    set_context(&c);
}

int* scheduler::get_data(){
    return this->dp;
}

void scheduler::yeild(){
    fibers.push_back(this->current_fiber);//put the fiber back in the q
    swap_context(&this->current_fiber->get_context(),&c);
}