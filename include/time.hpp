/*
 * @Descripttion:
 * @Author: jev
 * @Date: 2026-04-19
 */
#pragma once
#include <chrono>
#include <ctime>
#include <string>

namespace jev_log {
using system_time_point = std::chrono::time_point<std::chrono::system_clock>;
using high_resolution_time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;
using milliseconds = std::chrono::milliseconds;
using microseconds = std::chrono::microseconds;
using seconds = std::chrono::seconds;
using minutes = std::chrono::minutes;

namespace internal {
/**
 * @brief 获取时间格式化字符串中的小数部分
 *
 */
enum class Fractional { Millisecond, Microsecond, Nanosecond, NanosecondDefault };

/**
 * @brief 获取时间格式化字符串中的小数部分
 *
 * @param format_buffer 时间格式化字符串
 * @param pos 小数部分的位置
 * @return Fractional 小数部分
 */
Fractional getFractional(const std::string& format_buffer, size_t pos);

/**
 * @brief 将时间点转换为字符串
 *
 * @param time_point 时间点
 * @param fractional 小数部分
 * @return std::string 时间字符串
 */
std::string to_string(const jev_log::system_time_point& time_point,
                      Fractional fractional = Fractional::NanosecondDefault);

/**
 * @brief 将时间点转换为本地时间字符串
 *
 * @param time_point 时间点
 * @param format_buffer 时间格式化字符串
 * @return std::string 本地时间字符串
 */
std::string localtime_formatted_fractional(const jev_log::system_time_point& time_point,
                                           std::string format_buffer);

static const std::string date_formatted = "%Y/%m/%d";

// %f: 秒的小数部分 (%f 是纳秒)
// %f3: 毫秒，3 位数字：001
// %f6: 微秒，6 位数字：000001  --- time_format 默认格式
// %f9, %f: 纳秒，9 位数字：000000001
static const std::string time_formatted = "%H:%M:%S %f6";
}  // namespace internal

std::string put_time(const struct tm* tmb, const char* c_time_format);

tm localtime(std::time_t time);

std::string localtime_formatted(const system_time_point& ts, const std::string& time_format);

/**
 * @brief 将高精度时间点转换为系统时间点
 *
 * @param ts 高精度时间点
 * @return system_time_point
 */
inline system_time_point to_system_time(const high_resolution_time_point& ts) {
    // 在某些 (Windows) 系统上，system_clock 不提供最高的时间分辨率。
    // 因此 g3log 使用 high_resolution_clock 来记录消息时间戳。但是，
    // 与 system_clock 不同，high_resolution_clock 不能转换为时间和日期，
    // 因为它通常测量的是自系统启动以来的时间。
    // 因此，hrs_now 和 sys_now 在程序启动时记录一次，以便能够使用
    // to_system_time() 将时间戳转换为时间和日期。绝对时间的精度当然是
    // system_clock() 的精度，加上由于两个静态变量非同步初始化而增加的误差，
    // 但同一日志内的相对时间将与 high_resolution_clock 一样精确。
    using namespace std::chrono;
    static const auto hrs_now = high_resolution_clock::now();
    static const auto sys_now = system_clock::now();

    return time_point_cast<system_clock::duration>(sys_now + (ts - hrs_now));
}
}  // namespace jev_log