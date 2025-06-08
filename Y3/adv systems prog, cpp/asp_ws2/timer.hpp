#include <chrono>

//use of variadic template allows function to support varying argument number and types
template <typename... Args>
auto benchmark(void func(Args...), Args... args) { //support for functions of type void f(void)
    auto t1 = std::chrono::high_resolution_clock::now();
    func(args...); //function is executed
    auto t2 = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1); //time taken for function to run
    return std::make_pair("void",time);
}

template <typename K, typename... Args>
auto benchmark(K func, Args... args) { 
    auto t1 = std::chrono::high_resolution_clock::now();
    auto result = func(args...); //function is executed
    auto t2 = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1); //time taken for function to run
    return std::make_pair(result,time); //result of function and time taken passed as pair
}

// time is passed as std::chrono::duration<std::chrono::nanoseconds>,

// example use of benchmark function:

// int sqr(int a){
//     return a*a;
// }
//
// int main(){
//
//     auto [result,time] = benchmark(sqr,4);
//     std::cout<<result<<" "<<time.count()<<std::endl;
//
//     return 0;
// }
// will print 16 and time in nanoseconds

// to be used with member functions, lambda expression must be used to access member function:
// auto funclink = [&obj](argsType args) {return obj.func(args);};