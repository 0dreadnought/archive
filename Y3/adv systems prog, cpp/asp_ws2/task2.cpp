#include "allocator.hpp"
#include "simpletest/simpletest.h"

char const * groups[] = {
"al",
};


DEFINE_TEST_G(AlTest, al)
{
    allocator<80 * sizeof(char)> al; //allocator defined
    int * x = al.alloc<int>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");
    TEST_MESSAGE((al.next-al.start) == (sizeof(int)*10),"Wrong memory size allocated!!!!");
    char * y = al.alloc<char>(39); //allocation tested with different data types
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");
    int * z = al.alloc<int>(1);
    TEST_MESSAGE(z == nullptr, "Should have failed to allocate!!!!");
    al.dealloc(y);
    TEST_MESSAGE(al.allocount==1, "Failed to dealocate!!!!");
    al.dealloc(x);
    TEST_MESSAGE(al.next==al.start, "Failed to clear!!!!");

    allocator<200 * sizeof(int)> all; //allocator definition tested with different data types
    TEST_MESSAGE(all.start>al.heapend, "Clashing memory blocks!!!!"); //allocator tested for correctly defined boundaries
    int * q = all.alloc<int>(100);
    TEST_MESSAGE(q != nullptr, "Failed to allocate!!!!");
    char * w = all.alloc<char>(400);
    TEST_MESSAGE(w != nullptr, "Failed to allocate!!!!");
    int * e = all.alloc<int>(1);
    TEST_MESSAGE(e == nullptr, "Should have failed to allocate!!!!");
    all.dealloc(w);
    TEST_MESSAGE(all.allocount==1, "Failed to dealocate!!!!");
    all.dealloc(e);
    TEST_MESSAGE(all.next==all.start, "Failed to clear!!!!");
}

int main() {
bool pass = true;
for (auto group : groups) {
pass &= TestFixture::ExecuteTestGroup(group, TestFixture::Verbose);
}
return pass ? 0 : 1;
}
