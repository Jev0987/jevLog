/*
 * @Descripttion: 队列抽象基类
 * @Author: jev
 * @Date: 2026-04-09
 */
#pragma once

#include <cstddef>

namespace jev {

/**
 * @brief 统一队列接口，便于不同队列实现按同一抽象使用
 *
 * @tparam T 队列元素类型
 */
template <typename T>
class BaseQueue {
public:
    virtual ~BaseQueue() = default;

    /**
     * @brief 尝试入队（非阻塞）
     *
     * @param item 待入队元素
     * @return true 入队成功
     * @return false 入队失败（例如队列已满）
     */
    virtual bool tryEnqueue(const T& item) = 0;

    /**
     * @brief 尝试出队（非阻塞）
     *
     * @param item 出队元素写入位置
     * @return true 出队成功
     * @return false 出队失败（例如队列为空）
     */
    virtual bool tryDequeue(T& item) = 0;

    /**
     * @brief 阻塞等待并出队一个元素
     *
     * @param item 出队元素写入位置
     */
    virtual void waitAndDequeue(T& item) = 0;

    /**
     * @brief 判断队列是否为空
     *
     * @return true 队列为空
     * @return false 队列非空
     */
    virtual bool isEmpty() const = 0;

    /**
     * @brief 返回当前队列元素数量
     *
     * @return std::size_t 元素数量
     */
    virtual std::size_t size() const = 0;
};
}  // namespace jev