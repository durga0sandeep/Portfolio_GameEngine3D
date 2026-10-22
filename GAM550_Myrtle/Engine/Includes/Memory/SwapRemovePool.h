#pragma once
#include <vector>
#include <cassert>
#include <cstdint>

/*!****************************************************************************
 * file  : SwapRemovePool.h
 * brief : This is used for a pool that allows fast removal by swapping.
 * It keeps a compact vector(for iteration) and provides stable handles for safe access.
 * Usecase: rendering objects, game entities(maybe?), etc.
 * For particles, bullets, etc., use PoolAllocator instead.
 *
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 *****************************************************************************/

template <typename T>
class SwapRemovePool {
public:
    struct Handle {
        uint32_t index;
        uint32_t generation;

        bool operator==(const Handle& other) const
        {
            return index == other.index && generation == other.generation;
        }
    };

    Handle add(const T& item)
    {
        size_t index = m_items.size();
        m_items.push_back(item);
        m_generations.push_back(0);
        return Handle{ static_cast<uint32_t>(index), 0 };
    }

    void remove(const Handle& handle)
    {
        assert(isValid(handle));

        size_t index = handle.index;
        size_t lastIndex = m_items.size() - 1;

        if (index != lastIndex) {
            m_items[index] = std::move(m_items[lastIndex]);
            m_generations[index] = m_generations[lastIndex];
        }

        m_items.pop_back();
        m_generations.pop_back();
    }

    T* get(const Handle& handle)
    {
        if (!isValid(handle)) return nullptr;
        return &m_items[handle.index];
    }

    const T* get(const Handle& handle) const
    {
        if (!isValid(handle)) return nullptr;
        return &m_items[handle.index];
    }

    bool isValid(const Handle& handle) const
    {
        return handle.index < m_items.size() &&
            m_generations[handle.index] == handle.generation;
    }

    size_t size() const
    {
        return m_items.size();
    }
    bool empty() const
    {
        return m_items.empty();
    }

    auto begin()
    {
        return m_items.begin();
    }
    auto end()
    {
        return m_items.end();
    }

    auto begin() const
    {
        return m_items.begin();
    }
    auto end()   const
    {
        return m_items.end();
    }

private:
    std::vector<T> m_items;
    std::vector<uint32_t> m_generations;
};
