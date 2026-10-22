#pragma once
/*!****************************************************************************
 * file  : MonolithicAllocator.h
 * brief : Allocater that allocates a large block of memory at once
 * when remove clean all of it
 * usecase: on setup scene graph, level data, etc.
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#include <vector>
#include <cassert>

template <typename T>
class MonolithicAllocator
{
public:
    explicit MonolithicAllocator(std::size_t capacity)
        : m_capacity(capacity), m_offset(0)
    {
        m_buffer.reserve(capacity); // Preallocate memory to avoid reallocation
    }

    T* allocate()
    {
        assert(m_offset < m_capacity && "Out of memory!");
        m_buffer.emplace_back(); // Construct default T
        return &m_buffer[m_offset++];
    }

    template <typename... Args>
    T* allocate(Args&&... args)
    {
        assert(m_offset < m_capacity && "Out of memory!");
        m_buffer.emplace_back(std::forward<Args>(args)...);
        return &m_buffer[m_offset++];
    }

    void reset()
    {
        m_buffer.clear(); // Destroy all objects
        m_offset = 0;
    }

private:
    std::vector<T> m_buffer;
    std::size_t m_capacity;
    std::size_t m_offset;
};

