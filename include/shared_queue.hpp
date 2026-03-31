/*
 * @Descripttion: 多生产者多消费者的线程安全队列 （MPMC,有锁简单实现
 * @Author: jev
 * @Date: 2026-03-30
 */
#include <condition_variable>
#include <mutex>
#include <queue>

namespace jev {
template <typename T>
class shared_queue {
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
    void push(T item) {
        {
            std::lock_guard<std::mutex> lck(mtx_);
            queue_.push(item);
        }
        data_cond_.notify_one();
    }

    /**
     * @brief 获取元素（如果失败，直接返回）
     *
     * @param pop_item
     * @return true
     * @return false
     */
    bool try_and_pop(T& pop_item) {
        std::lock_guard<std::mutex> lck(mtx_);

        if (queue_.empty()) { return false; }

        pop_item = std::move(queue_.front());
        queue_.pop();
    }

    /**
     * @brief 阻塞等待获取数据
     *
     * @param pop_item
     */
    void wait_and_pop(T& pop_item) {
        std::lock_guard<std::mutex> lck(mtx_);

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
    bool empty() const {
        std::lock_guard<std::mutex> lck(mtx_);

        return queue_.empty();
    }

    /**
     * @brief 队列元素个数
     *
     * @return unsigned
     */
    unsigned size() const {
        std::lock_guard<std::mutex> lck(mtx_);

        return queue_.size();
    }
};
}  // namespace jev