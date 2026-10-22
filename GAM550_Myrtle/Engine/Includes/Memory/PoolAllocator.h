/*!****************************************************************************
 * file  : PoolAllocator.h
 * brief : 
 * allocator that reuses memory slots for objects of type T
 * recommend usecase: game objects, particles, etc.
 * like a stack allocator but can free in any order
 * using a free list to track available slots instead of remove
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#pragma once

template<typename T>
class PoolAllocator {
public:
    PoolAllocator(int size)
    {
        pool.reserve(size);
    }
    T& allocate()
    {
        if (!freeList.empty()) {
            size_t index = freeList.back();
            freeList.pop_back();
            return pool[index]; // reuse slot
        }
        else {
            pool.emplace_back();
            return pool.back(); // new slot
        }
    }

    void deallocate(T& obj)
    {
        size_t index = &obj - &pool[0]; // get index from pointer
        freeList.push_back(index);
    }

private:
    std::vector<T> pool;
    std::vector<size_t> freeList;
};


