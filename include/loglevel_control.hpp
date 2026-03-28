/*
 * @Descripttion:  和日志等级控制相关的函数
 * @Author: jev
 * @Date: 2026-03-28
 */
#pragma once

#include "loglevel.hpp"
namespace jev_log {

namespace internal {
/**
 * @brief  检查当前等级是否为致命等级
 *
 * @param level
 * @return true
 * @return false
 */
bool wasFatal(const LEVELS& level);
}  // namespace internal

/**
 * @brief 日志等级相关的函数
 *
 */
namespace log_levels {
/**
 * @brief 日志等级的状态
 *
 * 1. Absent 不存在该日志等级
 * 2. Enabled 正在使用
 * 3. Disabled 存在，但是未使用
 *
 */
enum class status { Absent, Enabled, Disabled };
status getStatus(LEVELS level);

/**
 * @brief 将小于 level 的日志等级全部disable，大于 level日志等级的全都enable
 *
 * @param level
 */
void setHightest(LEVELS level);

/**
 * @brief 将 level 放入当前日志系统中
 *
 * @param level
 * @param enabled
 */
void set(LEVELS level, bool enabled);

/**
 * @brief 使 level 失效
 *
 * @param level
 */
void disable(LEVELS level);

/**
 * @brief 使 level 生效
 *
 * @param level
 */
void enable(LEVELS level);

/**
 * @brief 使所有日志等级失效
 *
 * @warning 这会使得 FATAL 失效
 *
 */
void disableAll();

/**
 * @brief 使所有日志等级生效
 *
 */
void enableAll();

/**
 * @brief 打印所有日志等级及其状态信息
 *
 * @param levels2Print
 * @return std::string
 */
std::string to_string(std::map<int, LoggingLevel> levels2Print);

/**
 * @brief 打印当前日志系统中的日志等级及其状态信息
 *
 * @return std::string
 */
std::string to_string();

/**
 * @brief 获取当前日志系统的等级信息
 *
 * @return std::map<int, LoggingLevel>
 */
std::map<int, LoggingLevel> getAll();

/**
 * @brief 使 level 生效
 *
 * @param level
 * @return true
 * @return false
 */
bool logLevel(const LEVELS& level);
}  // namespace log_levels

/**
 * @brief 自定义日志等级
 *
 * @warning 仅在单线程上下文中初始化时才安全
 */
namespace only_change_at_initialization {

/**
 * @brief 添加自定义日志等级，自定义 status
 *
 * @param level
 * @param enabled
 */
void addLogLevel(LEVELS level, bool enabled);

/**
 * @brief 添加自定义日志等级， status 默认为 enable
 *
 * @param level
 */
void addLogLevel(LEVELS level);

/**
 * @brief 重置所有默认日志等级，使其为enabled状态，并且清理所有自定义的日志等级
 *
 */
void reset();
}  // namespace only_change_at_initialization

}  // namespace jev_log