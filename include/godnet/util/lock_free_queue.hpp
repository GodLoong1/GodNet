#ifndef GODNET_UTIL_LOCK_FREE_QUEUE_HPP
#define GODNET_UTIL_LOCK_FREE_QUEUE_HPP

#include "godnet/util/noncopyable.hpp"

#include <atomic>

namespace godnet
{

template <typename T>
class LockFreeQueue : Noncopyable
{
public:
    LockFreeQueue()
    : head_(new DataNode), tail_(head_.load(std::memory_order_relaxed))
    {
    }

    ~LockFreeQueue()
    {
        T output;
        while (dequeue(output))
        {
        }
        DataNode* front{head_.load(std::memory_order_relaxed)};
        delete front;
    }

    void enqueue(const T& input)
    {
        DataNode* node{new DataNode(input)};
        DataNode* prev{head_.exchange(node, std::memory_order_acq_rel)};
        prev->next_.store(node, std::memory_order_release);
    }

    void enqueue(T&& input)
    {
        DataNode* node{new DataNode(std::move(input))};
        DataNode* prev{head_.exchange(node, std::memory_order_acq_rel)};
        prev->next_.store(node, std::memory_order_release);
    }

    bool dequeue(T& output)
    {
        DataNode* tail{tail_.load(std::memory_order_relaxed)};
        DataNode* next{tail->next_.load(std::memory_order_acquire)};
        if (!next)
        {
            return false;
        }
        output = std::move(*next->data_);
        delete next->data_;
        tail_.store(next, std::memory_order_release);
        delete tail;
        return true;
    }

    bool empty() const
    {
        DataNode* tail{tail_.load(std::memory_order_relaxed)};
        DataNode* next{tail->next_.load(std::memory_order_acquire)};
        return !next;
    }

private:
    struct DataNode
    {
        DataNode() = default;

        DataNode(const T& data)
        : data_(new T(data))
        {
        }

        DataNode(T&& data)
        : data_(new T(std::move(data)))
        {
        }

        T* data_{nullptr};
        std::atomic<DataNode*> next_{nullptr};
    };

    std::atomic<DataNode*> head_;
    std::atomic<DataNode*> tail_;
};

}

#endif
