
#include "parser_executor.h"

namespace chtml{

    ParserExecutor parserExecutor(4); //define the parser executor for use

    ParserExecutor::ParserExecutor() 
        :ParserExecutor::ParserExecutor(std::thread::hardware_concurrency()) {

        //if hardware_concurrency fails to get the number 
        //of processors on this system then set to 2
        if (this->pool_size == 0) {
            // Unable to detect the number of hardware processors
            std::cout << "Unable to detect the number of hardware processors. Defaulting to 1." << std::endl;
            this->pool_size = 2; // Default to 2 thread - the second thread prevents the recursive stack to improve performance
        } else {
            std::cout << "Detected " << this->pool_size << " hardware processors." << std::endl;
        }
    }

    ParserExecutor::ParserExecutor(size_t pool_size) 
            : pool_size(pool_size), is_stop(false) {}

    ParserExecutor::~ParserExecutor() {
        this->stop();    
    }

    void ParserExecutor::enqueue(std::function<void()> f){  
         this->taskAddedCount++;         
         this->concurQueue.push(f);
    }

    void ParserExecutor::printProccessorUsedByThread(int threadId) {
        #ifdef _WIN32
            int processorId = GetCurrentProcessorNumber();
        #else
            int processorId = sched_getcpu();
        #endif
        
        // Lock coutMutex before writing to std::cout
        std::lock_guard<std::mutex> lock(this->coutMutex);
       
        std::cout << "Thread " << threadId << " is running on processor " << processorId << std::endl;
    }

    void ParserExecutor::tryUtilizeAllProcessors(int i){
            #ifdef _WIN32
                SetThreadAffinityMask(GetCurrentThread(), 1 << i);
            #else
                cpu_set_t cpuset;
                CPU_ZERO(&cpuset);
                CPU_SET(i, &cpuset);
                pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
            #endif

            this->printProccessorUsedByThread(i);
    }

    void ParserExecutor::run(int thread_id) {

        while (!this->is_stop) {                        

            if(this->taskCompletedCount ==  this->taskAddedCount){
                
                {
                    std::lock_guard<std::mutex> lock(this->coutMutex);
                    std::cout << "FINISHED PARSING - COMPLETED TASK = " << this->taskCompletedCount <<std::endl;
                }
                
                this->stop();
                return;
            }

            auto task = this->concurQueue.pop(); //Pop a task from the queue. if empty return null
            if(task != nullptr){
                task(); // Execute the lambda function
                this->taskCompletedCount++;
            }
            
            /*if(this->concurQueue.empty()){//TESTING!!!
                //{
                    //std::lock_guard<std::mutex> lock(this->coutMutex);
                    std::cout << "FINISHED PARSING - COMPLETED TASK = " << this->taskCompletedCount <<std::endl;
                //}
                this->stop();
                return;
            }*/

        }        
    }

    bool ParserExecutor::isRunning() {
        {
            std::lock_guard<std::mutex> lock(this->exectrMutex);
            return this->stopped_threads.size() < this->threadPool.size();
        }
    }

    void ParserExecutor::start() {
        // Initialize the thread pool with the specified number of threads
        if(this->isRunning()){
            std::cout << "Not Allowed - can not restart when one or more threads are still running" << std::endl;
        }

        this->stopped_threads.clear();
        this->threadPool.clear();

        for (int i = 0; i < this->pool_size; ++i) {

            this->tryUtilizeAllProcessors(i);        

            std::thread mthread(&ParserExecutor::run, this, i);

            this->threadPool.push_back(std::move(mthread)); // do not copy just move the thread in
        }

        condition.notify_all(); 
        
        this->joinAll(); //main thread block until all thread finish executing
    }

    void ParserExecutor::stop() {
        // Stop the execution of tasks by setting the is_stop flag to true
        {
            std::lock_guard<std::mutex> lock(this->exectrMutex);
            this->is_stop = true;
        }
        this->condition.notify_all();
    }

    void ParserExecutor::joinAll(){
        
        // Join all threads - main thread block until all thread finish executing
        for (std::thread& thread : this->threadPool) {
            thread.join();
        }
        
    }
}