/*
 * @Descripttion:  Active Object
 * @Author: jev
 * @Date: 2026-04-09
 */
#include "lock_free_queue.hpp"
#include "shared_queue.hpp"

namespace jev {

template <typename T>
class ActiveObject {
private:
    LockFreeQueue<T, 1024> queue_;
};
}  // namespace jev