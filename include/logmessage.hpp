/*
 * @Descripttion: 日志消息类
 * @Author: jev
 * @Date: 2026-04-19
 */
#pragma once
#include <string>
namespace jev {

/**
 * @brief 日志消息类, 配合 sink 使用。也可以直接使用 string
 *
 */
class LogMessage {
    using LogDetailsFunc = std::string (*)(const LogMessage&);

public:
    LogMessage() = default;

private:
};
}  // namespace jev