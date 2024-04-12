#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <stdexcept>
#include <cmath>
#include "benchmark.h"

#ifdef _WIN32
#include <malloc.h> // For _aligned_malloc and _aligned_free on Windows
#else
#include <cstdlib> // For posix_memalign on POSIX systems
#endif

class MemoryPool {

    private:
        std::vector<void*> memoryBlocks;
        size_t blockSize;
        size_t numBlocks;
        std::mutex mtx;

    public:
        MemoryPool(size_t block_size, size_t num_blocks) : blockSize(block_size), numBlocks(num_blocks) {
            // Allocate memory for the memory pool
            memoryBlocks.reserve(this->numBlocks);

            allocateMemory(this->numBlocks);
        }

        ~MemoryPool() {
            // Deallocate memory when the memory pool is destroyed
            for (void* block : memoryBlocks) {
                #ifdef _WIN32
                _aligned_free(block);
                #else
                free(block);
                #endif
            }
        }

        void* allocate(size_t size) {
            std::lock_guard<std::mutex> lock(this->mtx);
            
            if (size > this->blockSize) {
                throw std::runtime_error("Requested size exceeds block size of memory pool");
            }

            if (this->memoryBlocks.size() == 0) {
                // Allocate additional memory blocks
                size_t additionalBlocks = (size_t) ceil(this->numBlocks / 2.f); //Increase by half                
                allocateMemory(additionalBlocks);     
            }

            void* ptr = memoryBlocks.back();
            memoryBlocks.pop_back();
            return ptr;
        }

        void allocateMemory(size_t num_blocks){
            for (size_t i = 0; i < num_blocks; ++i) {
                void* block;
                #ifdef _WIN32
                block = _aligned_malloc(this->blockSize, alignof(std::max_align_t));
                if (!block) {
                    // Error handling: throw an exception if memory allocation fails
                    throw std::runtime_error("Memory allocation failed");
                }
                #else
                if (posix_memalign(&block, alignof(std::max_align_t), block_size) != 0) {
                    // Error handling: throw an exception if memory allocation fails
                    throw std::runtime_error("Memory allocation failed");
                }
                #endif
                memoryBlocks.push_back(block);
            }
        }

        void deallocate(void* ptr) {
            std::lock_guard<std::mutex> lock(mtx);
            //push back memory block for reuse                    
            memoryBlocks.push_back(ptr);
        }
};

MemoryPool MyClass::memoryPool(sizeof(MyClass), 2000000); // Memory pool for MyClass objects
