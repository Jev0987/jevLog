/*
 * @Descripttion:  封装bool类型的原子变量
 * @Author: jev
 * @Date: 2026-04-09
 */
#include <atomic>

namespace jev {
/**
 * @brief 封装原子变量，使其能够在 vector 等容器中使用
 *
 * http://stackoverflow.com/questions/13193484/how-to-declare-a-vector-of-atomic-in-c
 *
 */
class atomicbool {
public:
    atomicbool()
        : value_(false) {}
    atomicbool(bool value)
        : value_(value) {}
    atomicbool(std::atomic<bool>& other)
        : value_{other.load(std::memory_order_acquire)} {}
    atomicbool(const atomicbool& other)
        : value_{other.value_.load(std::memory_order_acquire)} {}

    atomicbool& operator=(bool value) {
        value_.store(value, std::memory_order_release);
        return *this;
    }
    atomicbool& operator=(const atomicbool& other) {
        value_.store(other.value_.load(std::memory_order_acquire), std::memory_order_release);
        return *this;
    }
    atomicbool& operator=(std::atomic<bool>& other) {
        value_.store(other.load(std::memory_order_acquire), std::memory_order_release);
        return *this;
    }
    atomicbool& operator=(const std::atomic<bool>& other) {
        value_.store(other.load(std::memory_order_acquire), std::memory_order_release);
        return *this;
    }

    bool operator==(const atomicbool& other) const {
        return value_.load(std::memory_order_acquire) ==
               other.value_.load(std::memory_order_acquire);
    }

    bool value() const { return value_.load(std::memory_order_acquire); }

    std::atomic<bool>& get() { return value_; }

private:
    std::atomic<bool> value_;
};
}  // namespace jev