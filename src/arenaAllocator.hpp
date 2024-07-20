#pragma once

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

class ArenaAllocator {
private:
    size_t m_size;
    size_t total_used = 0;
    std::vector<std::byte*> m_buffers;
    std::byte* m_used;

public:
    explicit ArenaAllocator(const size_t bytes)
        : m_size(bytes)
    {
        m_buffers.push_back(new std::byte[m_size]);
        m_used = m_buffers.back();
        std::cout << "Allocated " << m_size << " bytes of memory" << std::endl;
    }

    ArenaAllocator(ArenaAllocator&& other) noexcept
        : m_size { std::exchange(other.m_size, 0) }
        , m_buffers { std::exchange(other.m_buffers, std::vector<std::byte*>{nullptr}) }
        , m_used { std::exchange(other.m_used, nullptr) }
    {}

    ArenaAllocator& operator=(ArenaAllocator&& other) noexcept
    {
        std::swap(m_size, other.m_size);
        std::swap(m_buffers, other.m_buffers);
        std::swap(m_used, other.m_used);
        return *this;
    }

    template<typename T, typename... Args> [[nodiscard]]
    T* allocate(Args&&... args) noexcept
    {
        size_t remaining_num_bytes = m_size - static_cast<size_t>(m_used - m_buffers.back());
        void* ptr = static_cast<void*>(m_used);
        void* aligned_address = std::align(alignof(T), sizeof(T), ptr, remaining_num_bytes);
        m_used = static_cast<std::byte*>(aligned_address) + sizeof(T);

        if (aligned_address == nullptr) {
            std::cout << "Allocating more memory {Total: " + std::to_string((m_buffers.size() + 1) * m_size) + " bytes}" << std::endl;
            m_buffers.push_back(new std::byte[m_size]);
            m_used = m_buffers.back();
            aligned_address = m_used;
            m_used += sizeof(T);
            total_used += sizeof(T);
        } else {
            total_used += static_cast<size_t>(m_used - static_cast<std::byte*>(ptr));
        }

        return new (static_cast<T*>(aligned_address)) T {std::forward<Args>(args)...}; // construct
    }

    // strings not aligned
    [[nodiscard]]
    char* allocate_c_string(size_t len) {
        char* loc = (char*) m_used;
        m_used += len;
        total_used += len;

        //std::cout << "Allocating " << len << " bytes for var name" << std::endl;

        if ((m_used - m_buffers.back()) > m_size) {
            std::cout << "Allocating more memory {Total: " + std::to_string((m_buffers.size() + 1) * m_size) + " bytes}" << std::endl;
            m_buffers.push_back(new std::byte[m_size]);
            m_used = m_buffers.back();
            loc = (char*) m_used;
            m_used += len;
        }

        return loc;
    }

    ArenaAllocator(const ArenaAllocator& other) = delete;

    ArenaAllocator operator=(const ArenaAllocator& other) = delete;

    ~ArenaAllocator()
    {
        for(std::byte* memory : m_buffers) {
            std::cout << "Freeing " << memory << std::endl;
            delete[] memory;
        }
        std::cout << "Mem used: " << total_used << " bytes" << std::endl;
    }
};