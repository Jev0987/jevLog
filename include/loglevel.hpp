/*
 * @Descripttion:  日至等级模块
 * @Author: jev
 * @Date: 2026-03-28
 */
#pragma once
#include "atomicbool.hpp"
#include <map>
#include <string>
namespace jev_log {

namespace log_levels {
/**
 * @brief 日志等级，数值型
 *
 */
inline constexpr int DebugValue = 100;
inline constexpr int InfoValue = 200;
inline constexpr int WarningValue = 500;
inline constexpr int FatalValue = 1000;  // 普通致命类型 -> 用户使用
inline constexpr int InternalFatalValue = 2000;  // 程序内部致命类型 -> 系统用（例如崩溃了）
}  // namespace log_levels

/**
 * @brief 日至等级类
 *
 */
class LEVELS {
public:
    /**
     * @brief 拷贝构造函数
     *
     * @param other
     */
    LEVELS(const LEVELS& other)
        : value_(other.value_)
        , text_(other.text_) {}

    /**
     * @brief 构造函数
     *
     * @param id
     * @param id_text
     */
    LEVELS(int id, const std::string& id_text)
        : value_(id)
        , text_(id_text) {}

    bool operator==(const LEVELS& other) const {
        return (other.value_ == value_) && (other.text_ == text_);
    }

    bool operator!=(const LEVELS& other) const {
        return (other.value_ != value_) || (other.text_ != text_);
    }

    friend void swap(LEVELS& a, LEVELS& b) {
        using std::swap;
        swap(a.value_, b.value_);
        swap(a.text_, b.text_);
    }

    LEVELS& operator=(LEVELS other) {
        swap(*this, other);
        return *this;
    }

    int value_;
    std::string text_;
};

/**
 * @brief 内置的日志等级
 *
 */
inline const LEVELS JEVLOG_DEBUG{log_levels::DebugValue, "DEBUG"};
inline const LEVELS INFO{log_levels::InfoValue, "INFO"};
inline const LEVELS WARNING{log_levels::WarningValue, "WARNING"};
inline const LEVELS FATAL{log_levels::FatalValue, "FATAL"};

/**
 * @brief 框架底层崩溃被动输出的致命日志等级
 *
 */
inline const LEVELS CONTRACT{log_levels::InternalFatalValue, "CONTRACT"};
inline const LEVELS FATAL_SIGNAL{log_levels::InternalFatalValue + 1, "FATAL_SIGNAL"};
inline const LEVELS FATAL_EXCEPTION{log_levels::InternalFatalValue + 2, "FATAL_EXCEPTION"};

/**
 * @brief 日志等级实体类，确认是否使用该等级
 *
 */
class LoggingLevel {
public:
    LoggingLevel()
        : level_(INFO){};
    LoggingLevel(const LoggingLevel& other)
        : level_(other.level_) {
        status_ = other.status_;
    };
    LoggingLevel(const LEVELS& other_level)
        : level_(other_level) {
        status_ = true;
    }
    LoggingLevel(const LEVELS& other_level, const bool enabled)
        : level_(other_level) {
        status_ = enabled;
    }
    ~LoggingLevel() = default;

    LoggingLevel& operator=(const LoggingLevel& other) {
        status_ = other.status_;
        level_ = other.level_;
        return *this;
    }

    bool operator==(const LoggingLevel& other) const {
        return (other.level_ == level_) && (other.status_.value() == status_.value());
    }

    jev::atomicbool status_{false};
    LEVELS level_;
};  // namespace intrclass LoggingLevel

namespace internal {
inline const std::map<int, LoggingLevel> j_log_level_defaults = {
    {JEVLOG_DEBUG.value_, {JEVLOG_DEBUG}},
    {INFO.value_, {INFO}},
    {WARNING.value_, {WARNING}},
    {FATAL.value_, {FATAL}}};
}
}  // namespace jev_log