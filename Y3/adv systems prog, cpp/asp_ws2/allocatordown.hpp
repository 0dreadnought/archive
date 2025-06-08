#include <cstddef>

template <std::size_t U>

class allocatordown{

    private:

    public:
        int allocount;
        int* heapend;
        int* next;
        int* start;

        allocatordown(){
            start = new int[U];
            heapend = start + U;
            next = heapend;
            allocount=0;
        };

        //~allocator();
        template<typename T>
        T* alloc(int n){
            if(this->next-n*sizeof(T)>=this->start){ //check if enough memory is available
                this->next-=n*sizeof(T); //moving pointer down by the size of allocated block
                this->allocount++;
                return (T*)this->next;;
                }
            else{
                return nullptr;
            }
            };
        template<typename K>
        void dealloc(K* s){
            allocount-=1;
            if(allocount==0){
                this->next = this->heapend;
            }
        };

};