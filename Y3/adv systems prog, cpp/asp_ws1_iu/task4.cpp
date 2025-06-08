#include "SSStrT4.hpp"
#include "ref_c.hpp"

int main() {
    ssstr s("Hello world");
    ref_counter<ssstr> c(s); //string wraped into reference counter
    c.cont_class->print();
    c.print();
    {
        ref_counter<ssstr> t(c);
        ref_counter<ssstr> d = t;
        c.cont_class->print();
        c.print();
        t.cont_class->print();
        t.print();
        d.cont_class->print();
        d.print();
        std::cout << c.cont_class->getChar(1) << std::endl;
        t.cont_class->setChar(4,'O');
        c.cont_class->print();
        c.print();
        t.cont_class->print();
        t.print();
        d.cont_class->print();
        d.print();
    }
    c.cont_class->setChar(1,'E');
    c.cont_class->print();
    c.print();
}
