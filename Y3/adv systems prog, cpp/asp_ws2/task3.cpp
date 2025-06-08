#include <iostream>
#include "allocator.hpp"
#include "allocatordown.hpp"
#include "timer.hpp"

void tell(void){ //function of type void f(void)
    std::cout<<"meow"<<std::endl;
}

int sqr(int x){
    return x*x;
}

char getChar(const char* s,int n){
    return s[n];
}

int main() {
    double elapsed_time=0.0, elapsedd=0.0;
    {
    for(int j = 0;j<100;j++){ //all test run 100 times
    allocator<200 * sizeof(int)> al; //defining bumpup allocator
    allocatordown<200 * sizeof(int)> ald; //defining bumpdown allocator
    auto alalloc = [&al](int size) { return al.alloc<int>(size); };
    auto aldalloc = [&ald](int size) { return ald.alloc<int>(size); };
    //member functions must be properly linked before using in benchmark function
    for (int i =0; i<200;i++){ //allocators tested for many small allocations
        elapsedd +=  benchmark(aldalloc, 1).second.count(); 
        elapsed_time +=  benchmark(alalloc, 1).second.count();
        //result of allocation is dismissed and only time is taken into account
    }
    }
    std::cout<<elapsed_time/20000<<" "<<elapsedd/20000<<" "<<std::endl;//avg time taken for 1 allocation
    }

    elapsed_time=0.0;
    elapsedd=0.0;//reseting timers

    {
    for(int j =0;j<100;j++){
    allocator<200 * sizeof(int)> al;
    allocatordown<200 * sizeof(int)> ald;
    auto alalloc = [&al](int size) { return al.alloc<int>(size); };
    auto aldalloc = [&ald](int size) { return ald.alloc<int>(size); };
    for (int i =0; i<20;i++){ //allocators tested for fewer bigger allocations
        elapsed_time += benchmark(alalloc, 10).second.count();
        elapsedd += benchmark(aldalloc, 10).second.count();
    }
    }
    std::cout<<elapsed_time/2000<<" "<<elapsedd/2000<<std::endl;//avg time taken for 1 allocation
    }

    auto y = benchmark(tell).second;//test of function type void f(void)
    std::cout <<y.count()<<std::endl;
    //test of function with other function types
    auto [f,g] = benchmark(sqr,5); 
    std::cout <<f<<" "<<g.count()<<std::endl;
    auto [j,h] = benchmark(getChar,"Soup",1); 
    std::cout <<j<<" "<<h.count()<<std::endl;   

    return 0;
}