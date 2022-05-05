#pragma once

#include "LogStream.h"
#include <chrono>
#include <string>

#define FLOG_LEVEL_NUM 5

namespace flog {

class Logger {
  public:
  enum LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
  };

  Logger(std::string fileName, int line);
  Logger(std::string fileName, int line, LogLevel level);
  Logger(std::string fileName, int line, LogLevel level, const char* functionName);

  static LogLevel logLevel();
  static void setLogLevel(LogLevel level);

  using OutputFunc = void (*)(const char*, int len);
  using FlushFunc = void (*)();
  

  private:
  void formatTime();
  void finish();

  private:

  std::chrono::system_clock::time_point time_;
  LogStream stream_;
  LogLevel level_;
  int line_;
  std::string filename_;
};

extern Logger::LogLevel global_loglevel;

inline Logger::LogLevel Logger::logLevel()
{
  return global_loglevel;
}

} // namespace flog
