#pragma once
#include <chrono>
#include <exception>
#include <string>

namespace flog {
class noncopyable {
  public:
  noncopyable(const noncopyable&) = delete;
  void operator=(const noncopyable&) = delete;

  protected:
  noncopyable() = default;
  ~noncopyable() = default;
};

// this exception can be catched by ```catch(std::exception ex)```
class flog_exception : std::exception {
  public:
  explicit flog_exception(std::string message);
  flog_exception(const std::string& message,int last_errno);
  const char*what() const noexcept override;

  private:
  std::string message_;
};

[[noreturn]] void throw_flog_ex(const std::string &message,int last_errno);
[[noreturn]] void throw_flog_ex(std::string message);

using flog_lock = std::chrono::system_clock;
} // namespace flog

#define FLOG_LEVEL_DEBUG 0
#define FLOG_LEVEL_INFO 1
#define FLOG_LEVEL_WARN 2
#define FLOG_LEVEL_ERROR 3
#define FLOG_LEVEL_FATAL 4

namespace level {

enum level_enum : int {
  debug = FLOG_LEVEL_DEBUG,
  info = FLOG_LEVEL_INFO,
  warn = FLOG_LEVEL_WARN,
  error = FLOG_LEVEL_ERROR,
  fatal = FLOG_LEVEL_FATAL
};
} // namespace level
