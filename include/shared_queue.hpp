/*
 * @Descripttion: 多生产者多消费者的线程安全队列 （MPMC,有锁简单实现
 * @Author: jev
 * @Date: 2026-03-30
 */
#pragma once

#include "base_queue.hpp"
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <queue>

namespace jev {
template <typename T>
class shared_queue : public BaseQueue<T> {
private:
    std::queue<T> queue_;
    mutable std::mutex mtx_;
    std::condition_variable data_cond_;

    shared_queue(const shared_queue& other) = delete;
    shared_queue& operator=(const shared_queue& other) = delete;

public:
    shared_queue() = default;

    /**
     * @brief 插入元素
     *
     * @param item
     */
    bool tryEnqueue(const T& item) override {
        {
            std::lock_guard<std::mutex> lck(mtx_);
            queue_.push(item);
        }
        data_cond_.notify_one();
        return true;
    }

    /**
     * @brief 获取元素（如果失败，直接返回）
     *
     * @param pop_item
     * @return true
     * @return false
     */
    bool tryDequeue(T& pop_item) override {
        std::lock_guard<std::mutex> lck(mtx_);

        if (queue_.empty()) { return false; }

        pop_item = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    /**
     * @brief 阻塞等待获取数据
     *
     * @param pop_item
     */
    void waitAndDequeue(T& pop_item) override {
        std::unique_lock<std::mutex> lck(mtx_);

        data_cond_.wait(lck, [this]() { return !queue_.empty(); });

        pop_item = std::move(queue_.front());

        queue_.pop();
    }

    /**
     * @brief 是否为空
     *
     * @return true
     * @return false
     */
    bool isEmpty() const override {
        std::lock_guard<std::mutex> lck(mtx_);

        return queue_.empty();
    }

    /**
     * @brief 队列元素个数
     *
     * @return unsigned
     */
    std::size_t size() const override {
        std::lock_guard<std::mutex> lck(mtx_);

        return queue_.size();
    }

    void push(T item) {
        (void)tryEnqueue(item);
    }

    bool try_and_pop(T& pop_item) {
        return tryDequeue(pop_item);
    }

    void wait_and_pop(T& pop_item) {
        waitAndDequeue(pop_item);
    }

    bool empty() const {
        return isEmpty();
    }
};
}  // namespace jev