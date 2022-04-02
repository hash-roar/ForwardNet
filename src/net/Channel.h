#ifndef __FNET_NET_CHANNEL_
#define __FNET_NET_CHANNEL_

#include "../base/Timestamp.h"
#include <algorithm>
#include <functional>
#include <memory>

namespace net {

using namespace base;

class EventLoop;

class Channel {
  public:
  using EventCallback = std::function<void()>;
  using ReadEventCallback = std::function<void(Timestamp)>;

  Channel(EventLoop* loop, int fd);
  ~Channel();

  void handleEvent(Timestamp receiveTime);

  void setReadCallBack(ReadEventCallback callbcak)
  {
    read_callcak_ = std::move(callbcak);
  }

  void setWriteCallback(EventCallback callbcak)
  {
    wirte_callback_ = std::move(callbcak);
  }
  void setCloseCallback(EventCallback callbcak)
  {
    close_callbcak_ = std::move(callbcak);
  }

  void setErrorCallbcak(EventCallback callbcak)
  {
    error_callbcak_ = std::move(callbcak);
  }

  void tie(const std::shared_ptr<void>&); // prolang lifetime of this channel to avoid core dump

  void setReceiveEvents(int receiveEvents) { receive_events_ = receiveEvents; }

  bool isNoneEvent() const { return register_events_ == kNoneEvent; }

  // set events

  void enableReading()
  {
    register_events_ |= kReadEvent;
    update();
  }
  void disableReading()
  {
    register_events_ &= ~kReadEvent;
    update();
  }
  void enableWriting()
  {
    receive_events_ |= kWriteEvent;
    update();
  }
  void disbaleWriting()
  {
    receive_events_ &= ~kWriteEvent;
    update();
  }
  void disableAll()
  {
    receive_events_ = kNoneEvent;
    update();
  }
  bool isWriting() const { return register_events_ & kWriteEvent; }
  bool isReading() const { return register_events_ & kReadEvent; }
  // get and set channel status
  int index() { return poll_index_; }
  void setIndex(int index) { poll_index_ = index; }
  int fd() const { return fd_; }
  int events() const { return register_events_; }

  // void doNotLogHup(){log_hup_ =false;} // never log socket close
  //
  EventLoop* ownerLoop() { return loop_; }

  void remove();

  private:
  void update();

  static std::string eventToString(int fd, int ev);
  void handleEventWithGuard(Timestamp receiveTime);

  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;

  EventLoop* loop_;
  const int fd_;

  int register_events_;
  int receive_events_;
  int poll_index_;
  bool log_hup_; // from linux2.6.17 ,when another side close socket return

  std::weak_ptr<void> tie_;
  bool tied_;
  bool is_handling_event_;
  bool added_to_loop_;
  ReadEventCallback read_callcak_;
  EventCallback write_callback_;
  EventCallback close_callbcak_;
  EventCallback error_callbcak_;
};
}

#endif
