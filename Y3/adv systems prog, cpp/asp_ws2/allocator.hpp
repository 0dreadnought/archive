#include <cstddef>

template <std::size_t U>

class allocator{

    private:

    public:
        int allocount;
        int* heapend;
        int* next;
        int* start;

        allocator(){
            start = new int[U]; //allocating memory block
            next = start;
            heapend = start + U; // defining boundaries
            allocount=0;
        };

        //~allocator();
        template<typename T>
        T* alloc(int n){
            if(this->next+n*sizeof(T)<=this->heapend){ //check if enough memory is available
                T* tmp = (T*)this->next;
                this->next+=n*sizeof(T); //moving pointer by the size of allocated block
                this->allocount++;
                return tmp;
                }
            else{
                return nullptr;
            }
            };
        template<typename K>
        void dealloc(K* s){
            allocount-=1;
            if(allocount==0){
                this->next = this->start; //reseting memory block
            }
        };

};