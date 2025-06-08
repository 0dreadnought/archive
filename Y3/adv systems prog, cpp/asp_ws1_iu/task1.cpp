#include "SSStr.hpp"

int main() {
    ssstr s("Hello world");
    s.print();
    {
        ssstr t = s;
        s.print();
        t.print();
        s.setChar(4,'O');
        std::cout << s.getChar(1) << std::endl;
        s.print();
        t.print();
    }
    s.setChar(1,'E');
    s.print();
}
