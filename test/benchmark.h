
#include <chrono>
#include <functional>


void benchmark(std::function<void()> f, int interations){
    auto start = std::chrono::steady_clock::now();

    for(int i=0; i < interations; ++i){
        f();
    }    

    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "\nElapsed time: " << elapsed_seconds.count() << "s\n";

}