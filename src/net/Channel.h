#ifndef __FNET_NET_CHANNEL_
#define __FNET_NET_CHANNEL_

#include <functional>
#include <memory>
#include "../base/Timestamp.h"

namespace net {

using namespace base;

class EventLoop;

class Channel {
  public:
  using EventCallback = std::function<void()>;
  using ReadEventCallback = std::function<void(Timestamp)>;

  Channel(EventLoop* loop, int fd);

  private:
  static std::string eventToString(int fd, int ev);

  EventLoop* loop_;
  const int fd_;

  std::weak_ptr<void> tie_;
  
  ReadEventCallback read_callcak_;
  EventCallback wirte_callback_;
  EventCallback close_callbcak_;
  EventCallback error_callbcak_;
};
}

#endif
