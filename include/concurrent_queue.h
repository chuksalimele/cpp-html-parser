
#ifndef CONCURRENT_QUERY_H
#define CONCURRENT_QUERY_H


#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <semaphore>
#include <condition_variable>
#include <functional>

namespace chtml{
    class ConcurrentQueue {
    private:
        using Task = std::function<void()>;
        std::queue<Task> queue_;
        mutable std::mutex mutex_;
        std::binary_semaphore sem; // Semaphore with initial count 1

    public:
        explicit ConcurrentQueue();

        ~ConcurrentQueue();

        void push(Task task);

        Task pop();

        bool empty() const;
    };
}

#endif // CONCURRENT_QUERY_H