/*
 * @Descripttion:  日至等级模块
 * @Author: jev
 * @Date: 2026-03-28
 */
#pragma once
#include <string>
namespace jev_log {
namespace lot_levels {
/**
 * @brief 日至等级模块，需要能够方便的改变，删除和添加日至等级
 *
 */
class Levels {
public:
    /**
     * @brief 拷贝构造函数
     *
     * @param other
     */
    Levels(const Levels& other)
        : level_(other.level_)
        , text_(other.text_) {}

    /**
     * @brief 构造函数
     *
     * @param id
     * @param id_text
     */
    Levels(int id, std::string& id_text)
        : level_(id)
        , text_(id_text) {}

    bool operator==(const Levels& other) const {
        return (other.level_ == level_) && (other.text_ == text_);
    }

    bool operator!=(const Levels& other) const {
        return (other.level_ != level_) || (other.text_ != text_);
    }

private:
    int level_;
    std::string text_;
};

}  // namespace lot_levels
}  // namespace jev_log