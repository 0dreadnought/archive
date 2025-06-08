#include <iostream>
#include <cstring>

using namespace std;


class ssstr {

    private:
        char* stringi;
        //reference counting turned into class

    public:
        ssstr();
        ssstr(const char* s) {
            stringi = new char[strlen(s)+1];
            strcpy(stringi,s);

        };
        ssstr(ssstr const& s){         
            stringi = s.stringi;
        };
        ssstr& operator= (ssstr const& s) {
            stringi = s.stringi; 
            return *this;
        };
        //~ssstr();
        char getChar(const int& i) {return stringi[i];};
        void setChar(const int& i, const char& c) {stringi[i]=c;};
        void print() {cout<<stringi;};
};
