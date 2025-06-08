#include <iostream>

template <class C>

class ref_counter{
    private:
        int* ref;

    public:
        C* cont_class;
        ref_counter();
        ref_counter(C& s){
            cont_class = &s;
            ref = new int;
            *ref=1;
        };
        ref_counter(ref_counter& s){
            cont_class = s.cont_class;
            this->ref = s.ref;
            *ref = *s.ref+1;
        };
        ~ref_counter(){
            *ref=*ref-1;
            if(*ref==0){
                delete ref;
                std::cout<<"[0]"<<std::endl;
            }
        };

        void print() {std::cout<<" ["<<*ref<<"]"<<std::endl;};
};