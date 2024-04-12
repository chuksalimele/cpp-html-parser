#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include "include/concurrent_queue.h"

namespace chtml{
    
    ConcurrentQueue::ConcurrentQueue() : sem(1) {} // Initialize sem with initial count of 1

    ConcurrentQueue::~ConcurrentQueue() {
    }

    void ConcurrentQueue::push(ConcurrentQueue::Task task) {
            sem.acquire(); // Acquire the semaphore 
            
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(std::move(task));

            sem.release(); // Release the semaphore
        
    }

    ConcurrentQueue::Task ConcurrentQueue::pop() {
        
        sem.acquire(); // Acquire the semaphore 
                    
        std::lock_guard<std::mutex> lock(mutex_);
        if(queue_.empty()){
            sem.release(); // Release the semaphore
            return nullptr;
        }

        Task task = std::move(queue_.front());
        queue_.pop();
        
        sem.release(); // Release the semaphore        
        
        return task;
    }

    bool ConcurrentQueue::empty() const {        
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

}