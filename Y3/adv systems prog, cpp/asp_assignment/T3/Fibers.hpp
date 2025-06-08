#pragma once
#include <deque>
#include "../context/context.hpp"

class fiber{
    private:
        Context c;
        char* sb;
        char* st;
        int* dp;
        
    public:
        fiber(void func(),int* pointer = nullptr);
        Context& get_context();
        int* get_data();

};

class scheduler{
    private:
        std::deque<fiber*> fibers;
        Context c;
        fiber* current_fiber; //new var to keep the fiber after deleting it from the q
        int* dp;

    public:
        scheduler(int* pointer = nullptr);
        void spawn(fiber* f);
        void do_it();
        void fiber_exit();
        int* get_data();
        void yeild();

};