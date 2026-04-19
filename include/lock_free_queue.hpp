/*
 * @Descripttion:  无锁队列
 * @Author: jev
 * @Date: 2026-03-30
 */
#pragma once

#include "base_queue.hpp"
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <thread>
namespace jev {

/**
 * @brief  无锁环形缓冲队列
 *
 * @tparam T 数据类型
 * @tparam Size  缓冲区大小
 */
template <typename T, std::size_t Size>
class LockFreeQueue : public BaseQueue<T> {
public:
    LockFreeQueue()
        : enqueue_pos_(0)
        , dequeue_pos_(0) {
        // 初始化，当 buffer[i] == i 时，意味着可写
        for (size_t i = 0; i < Size; ++i) buffer_[i].seq.store(i);
    }

    /**
     * @brief 插入元素
     *
     * @param item
     */
    void push(T item) override { tryEnqueue(item); }

    /**
     * @brief 尝试出队
     *
     * @param item
     */
    void try_and_pop(T& item) override { tryDequeue(item); }

    /**
     * @brief 阻塞等待出队
     *
     * @param item
     */
    void wait_and_pop(T& item) override { waitAndDequeue(item); }

    /**
     * @brief 尝试入队
     *
     * @param data
     * @return true
     * @return false
     */
    bool tryEnqueue(const T& data) override {
        size_t pos = enqueue_pos_.load(std::memory_order_relaxed);
        for (;;) {
            // 环形slot定位逻辑， 期望位置 % 队列大小。
            // （由于我们的Size是2的幂次，所以这里可以优化成位运算）
            Node& node = buffer_[pos & (Size - 1)];

            // 获取当前slot数据块的序列号，用于检查是否能够使用
            size_t seq = node.seq.load(std::memory_order_acquire);

            // size_t 作差比较有负数变极大正数的风险，所以硬转成int
            intptr_t diff = (intptr_t)seq - (intptr_t)pos;

            if (diff == 0) {
                // 有空余位置，slot里的序列号是当前生产者游标所在位置，生产者游标+1
                if (enqueue_pos_.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed)) {
                    // 存放数据，序列号占用（使其+1）
                    node.data = data;
                    node.seq.store(pos + 1, std::memory_order_release);
                    return true;
                }
            } else if (diff < 0) {
                // 队列满了
                return false;
            } else {
                // 被抢占，重试
                pos = enqueue_pos_.load(std::memory_order_relaxed);
            }
        }
    }

    /**
     * @brief 尝试出队
     *
     * @param data  传出参数，保存队列中出队的数据
     * @return true
     * @return false
     */
    bool tryDequeue(T& data) override {
        size_t pos = dequeue_pos_.load(std::memory_order_relaxed);

        for (;;) {
            Node& node = buffer_[pos & (Size - 1)];

            size_t seq = node.seq.load(std::memory_order_acquire);

            intptr_t diff = (intptr_t)seq - (intptr_t)(pos + 1);

            if (diff == 0) {
                // slot 可读
                if (dequeue_pos_.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed)) {
                    data = node.data;
                    // size_t 会自然回绕（不用担心溢出）
                    node.seq.store(pos + Size, std::memory_order_release);
                    return true;
                }
            } else if (diff < 0) {
                return false;
            } else {
                pos = dequeue_pos_.load(std::memory_order_relaxed);
            }
        }
    }

    void waitAndDequeue(T& data) override {
        while (!tryDequeue(data)) { std::this_thread::yield(); }
    }

    bool isEmpty() const override { return size() == 0; }

    std::size_t size() const override {
        const std::size_t enqueuePos = enqueue_pos_.load(std::memory_order_acquire);
        const std::size_t dequeuePos = dequeue_pos_.load(std::memory_order_acquire);
        return enqueuePos - dequeuePos;
    }

    bool try_enqueue(const T& data) { return tryEnqueue(data); }

    bool try_dequeue(T& data) { return tryDequeue(data); }

private:
    /**
     * @brief 环形缓冲中的节点数据结构
     *
     */
    struct Node {
        std::atomic<size_t> seq;  // 序列号（原子变量，注意是size_t，无符号）
        T data;                   // 实际数据
    };

    /**
     * @brief 编译时断言检查，如果不满足直接报错
     *
     */
    static_assert((Size & (Size - 1)) == 0, "Size must be power of 2");

    alignas(64) Node buffer_[Size];                // 环形缓冲区 slot
    alignas(64) std::atomic<size_t> enqueue_pos_;  // 生产者全局游标
    alignas(64) std::atomic<size_t> dequeue_pos_;  // 消费者全局游标
};
}  // namespace jev