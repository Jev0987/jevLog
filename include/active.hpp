/*
 * @Descripttion:  Active Object
 * @Author: jev
 * @Date: 2026-04-09
 */
#include "base_queue.hpp"
#include "lock_free_queue.hpp"
#include "shared_queue.hpp"
#include <functional>
#include <memory>

namespace jev {
using Callback = std::function<void()>;

/**
 * @brief 主动对象模式
 *
 * @tparam T
 */
template <typename T>
class ActiveObject {
public:
    /**
     * @brief 创建主动对象
     *
     * @param q 队列对象
     * @return std::unique_ptr<ActiveObject<T>> 主动对象
     */
    static std::unique_ptr<ActiveObject<T>> create(std::unique_ptr<BaseQueue<T>> q) {
        std::unique_ptr<ActiveObject<T>> obj = std::make_unique<ActiveObject<T>>();
        obj->queue_ = std::move(q);
        obj->thr_ = std::thread(&ActiveObject<T>::run, obj.get());
        return obj;
    }

    /**
     * @brief 发送回调函数
     *
     * @param cb 回调函数
     */
    void send(Callback cb) { queue_->push(cb); }

    virtual ~ActiveObject() {
        send([this]() noexcept { done_ = true; });
        thr_.join();
    }

private:
    /**
     * @brief 构造函数隐藏
     *
     */
    ActiveObject<T>()
        : done_(false){};

    /**
     * @brief 显示构造函数
     *
     * @param q 显示构造队列对象
     */
    explicit ActiveObject(std::unique_ptr<BaseQueue<T>> q)
        : queue_(std::move(q))
        , done_(false) {}

    /**
     * @brief 禁止拷贝构造
     *
     */
    ActiveObject<T>(const ActiveObject<T>&) = delete;
    ActiveObject<T>& operator=(const ActiveObject<T>&) = delete;

    void run() {
        while (!done_) {
            Callback cb;
            queue_->wait_and_pop(cb);
            cb();
        }
    }

private:
    std::unique_ptr<BaseQueue<T>> queue_;
    std::thread thr_;
    bool done_;
};
}  // namespace jev