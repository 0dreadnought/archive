#include "allocator.hpp"
#include <iostream>

int main(){

    //testing work of allocator
    
    allocator<20* sizeof(int)> al;

    std::cout<<al.next<<" "<<al.heapend<<" "<<al.allocount<< std::endl;

    int * x = al.alloc<int>(10); //check if allocates

    std::cout<<al.next<<" "<<al.allocount<< std::endl;

    int * y = al.alloc<int>(5);

    int * z = al.alloc<int>(10);
    int f = z==nullptr; //check if refuses to allocate when out of boundaries
    std::cout<<al.next<<" "<<al.allocount<<" "<<f<< std::endl;

    al.dealloc<int>(x);
    std::cout<<al.next<<" "<<al.allocount<< std::endl;
    al.dealloc<int>(y);
    std::cout<<al.next<<" "<<al.allocount<< std::endl; // check if resets
    int * c = al.alloc<int>(10);
    std::cout<<al.next<<" "<<al.allocount<< std::endl; // check if allocates again

    return 0;
}