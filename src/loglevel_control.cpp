/*
 * @Descripttion:
 * @Author: jev
 * @Date: 2026-03-28
 */
#include "loglevel.hpp"
#include "loglevel_control.hpp"
#include <string>

namespace jev_log {

namespace internal {
/**
 * @brief 当前日志系统中的日志等级map
 *
 */
std::map<int, LoggingLevel> j_log_levels = j_log_level_defaults;
bool wasFatal(const LEVELS& level) { return level.value_ >= FATAL.value_; }
}  // namespace internal

namespace log_levels {

status getStatus(LEVELS level) {
    const auto it = internal::j_log_levels.find(level.value_);

    if (it == internal::j_log_levels.end()) { return status::Absent; }

    return it->second.status_.value() ? status::Enabled : status::Disabled;
}

void setHightest(LEVELS level) {
    auto it = internal::j_log_levels.find(level.value_);

    if (it != internal::j_log_levels.end()) {
        for (auto& v : internal::j_log_levels) {
            if (v.first < level.value_) {
                disable(v.second.level_);
            } else {
                enable(v.second.level_);
            }
        }
    }
}

void set(LEVELS level, bool enabled) {
    auto it = internal::j_log_levels.find(level.value_);

    if (it != internal::j_log_levels.end()) {
        internal::j_log_levels[level.value_] = {level, enabled};
    }
}

void disable(LEVELS level) { set(level, false); }

void enable(LEVELS level) { set(level, true); }

void disableAll() {
    for (auto& v : internal::j_log_levels) { v.second.status_ = false; }
}

void enableAll() {
    for (auto& v : internal::j_log_levels) { v.second.status_ = true; }
}

std::string to_string(std::map<int, LoggingLevel> levels2Print) {
    std::string result;
    for (auto& v : internal::j_log_levels) {
        result += "name: " + v.second.level_.text_ + " levels: " + std::to_string(v.first) +
                  " status: " + std::to_string(v.second.status_.value()) + "\n";
    }
    return result;
}

std::string to_string() { return to_string(internal::j_log_levels); }

std::map<int, LoggingLevel> getAll() { return internal::j_log_levels; }

bool logLevel(const LEVELS& log_level) {
    int level = log_level.value_;
    bool status = internal::j_log_levels[log_level.value_].status_.value();
    return status;
}

}  // namespace log_levels

}  // namespace jev_log