#include "Basic.hpp"

#ifndef _MEMPOOL_HPP
#define _MEMPOOL_HPP

// Independent developments collection
namespace idc {

    class _MemoryPool {
        public:
            _MemoryPool();
            _MemoryPool(size_t n, size_t sz);
            void* allocate(size_t sz);
            void deallocate(void *ptr, size_t = 0);
            
            ~_MemoryPool();

        private:
            struct block {
                struct block *prev = nullptr, *next = nullptr;
            };
            typedef struct block* block_ptr;
            
            // address of the whole pool
            void* mpool = nullptr;

            // free memory block linked list
            block_ptr freeBlock = nullptr;

            // alloced memory bleck linked list
            block_ptr allocBlock = nullptr;

            size_t block_size;
            size_t block_num;
            size_t pool_size = block_num * (block_size + sizeof(block));
    };

} // !idc namespace


#endif // !_MEMPOOL_HPP