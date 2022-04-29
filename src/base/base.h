#ifndef __FNET_BASE__BASE_
#define __FNET_BASE__BASE_

#include "Timestamp.h"
#include <functional>
#include <memory>

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

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using ConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
using CloseCallback = std::function<void(const TcpConnectionPtr&)>;
using WriteCompelteCallback = std::function<void(const TcpConnectionPtr&)>;
using HighWaterCallback = std::function<void(const TcpConnectionPtr&)>;
using MessageCallback = std::function<void(const TcpConnectionPtr&, Buffer*, base::Timestamp)>;
} //namespace base

#endif
