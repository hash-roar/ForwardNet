#ifndef __FNET_BASE__BASE_
#define __FNET_BASE__BASE_

#include <exception>
#include <functional>
#include <memory>
#include <system_error>

#define DEBUG_LOG

namespace base {
typedef std::function<void()> TimerCallback;

class noncopyable {
  public:
  noncopyable(const noncopyable&) = delete;
  void operator=(const noncopyable&) = delete;

  protected:
  noncopyable() = default;
  ~noncopyable() = default;
};

class TcpConnection;
class Buffer;
class Timestamp;

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using ConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
using CloseCallback = std::function<void(const TcpConnectionPtr&)>;
using WriteCompelteCallback = std::function<void(const TcpConnectionPtr&)>;
using HighWaterCallback = std::function<void(const TcpConnectionPtr&)>;
using MessageCallback = std::function<void(const TcpConnectionPtr&, Buffer*, base::Timestamp)>;

// some utils class
class FnetException : public std::exception {
  public:
  explicit FnetException(std::string msg)
      : message_(std::move(msg))
  {
  }

  FnetException(const std::string& msg, int last_errno)
  {
    message_ = std::system_error(std::error_code(last_errno, std::generic_category()), msg).what();
  }
  const char* what() const noexcept override
  {
    return message_.c_str();
  }

  private:
  std::string message_;
};

} // namespace base

void throw_fnet_ex(const std::string& msg, int last_errno);
void throw_fnet_ex(std::string msg);

#endif
