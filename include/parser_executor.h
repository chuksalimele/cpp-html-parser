
#ifndef PARSER_EXECUTOR_H
#define PARSER_EXECUTOR_H

#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "concurrent_queue.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace chtml{

    class ParserExecutor {
        
    private:
        std::vector<std::thread> threadPool;
        ConcurrentQueue concurQueue;
        std::vector<int> stopped_threads;

        std::atomic<int> taskAddedCount = 0;
        std::atomic<int> taskCompletedCount = 0;

        std::mutex exectrMutex;
        std::condition_variable condition;
        std::mutex coutMutex; // Mutex for synchronizing std::cout

        bool is_stop;

        int pool_size;

        void joinAll();

        void run(int thread_id);

        void printProccessorUsedByThread(int threadId);

        void tryUtilizeAllProcessors(int threadId);

    public:
        explicit ParserExecutor();

        explicit ParserExecutor(size_t pool_size);

        ~ParserExecutor();
        
        void enqueue(std::function<void()> f);

        void start();

        void stop();

        bool isRunning();
    };


    // Declare the ParserExecutor using extern to be used by more than one files
    extern ParserExecutor parserExecutor;

}

#endif // PARSER_EXECUTOR_H