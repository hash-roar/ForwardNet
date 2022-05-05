#include "Logging.h"
#include "LogStream.h"
#include <assert.h>
#include <bits/types/time_t.h>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace flog;

thread_local char ErrnoBuffer[512];
thread_local char TimeBuffer[64];
thread_local time_t LastSecond;

Logger::LogLevel getEnvLogLevel()
{
  if (::getenv("FLOG_RBUG")) {
    return Logger::DEBUG;
  } else {
    return Logger::INFO;
  }
}

Logger::LogLevel global_loglevel = getEnvLogLevel();

const char* LogLevelToString[FLOG_LEVEL_NUM] = {
  "DEBUG",
  "INFO",
  "WARN",
  "ERROR",
  "FATAL"
};

class GetStrLen {
  public:
  GetStrLen(const char* str, unsigned len)
      : str_(str)
      , len_(len)
  {
    assert(std::strlen(str) == len_);
  }

  const char* str_;
  const unsigned len_;
};

void defaultOutput(const char* msg, int len)
{
std:
  size_t n = std::fwrite(msg, 1, len, stdout);
}

void defaultFlush()
{
  std::fflush(stdout);
}

Logger::OutputFunc global_output = defaultOutput;
Logger::FlushFunc global_flush = defaultFlush;
// std::chrono::system_clock::time_point

Logger::Logger(std::string fileName, int line, LogLevel level, const char* functionName)
    : level_(level)
    , line_(line)
    , filename_(fileName)
    , stream_()
    , time_(std::chrono::system_clock::now())
{
}

void Logger::formatTime()
{
}
