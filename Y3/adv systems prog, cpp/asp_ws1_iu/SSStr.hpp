#include <iostream>
#include <cstring>

using namespace std;


class ssstr {

    private:
        char* stringi;
        int* ref;

    public:
        ssstr();
        ssstr(const char* s) {
            ref = new int; // initializing counter T2
            *ref = 1;
            stringi = new char[strlen(s)+1]; // dynamic malloc T1
            strcpy(stringi,s);

        };
        ssstr(ssstr const& s){
            this->ref = s.ref; //copying address and incrementing value T2
            *ref += 1;            
            stringi = s.stringi; //copying string address T1
            
        };
        ssstr& operator= (ssstr const& s) {
            *ref -=1;   //decrement current reference count before changing pointers
            if(*ref == 0){
                delete ref;
                delete stringi;
            }
            this->ref = s.ref;
            *ref += 1;
            stringi = s.stringi; 
            return *this;
        };
        ~ssstr(){
            *ref -= 1; //decrement once gone T2
            if(*ref==0){
                cout<<"[0]"<<endl; //T3
                delete ref;
                delete stringi;
            }
            };
        char getChar(const int& i) {return stringi[i];};
        void setChar(const int& i, const char& c) {stringi[i]=c;};
        void print() {cout<<stringi<<" ["<<*ref<<"]"<<endl;}; //ref count included T2
};
