#ifndef __FNET_NET_EVENTLOOP_
#define __FNET_NET_EVENTLOOP_

#include "Channel.h"
#include "Thread.h"
//#include "Epoll.h"
#include <any>
#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <sys/types.h>
#include <vector>

namespace net {

class Epoll;
// using ChannelList = std::vector<Channel*>;
typedef std::vector<Channel*> ChannelList;

class EventLoop {
  public:
  using Functor = std::function<void()>;

  EventLoop();
  ~EventLoop();

  void loop();
  void quit();

  void runInLoop(Functor callback);
  void queueInLoop(Functor callback);

  void wakeup();

  bool isInLoopThread() const { return thread_id_ == CurrentThread::tid(); }
  void assertInLoopThread()
  {
    // TODO
  }

  private:
  void handleRead();
  void doPendingFunctors();

  // using ChannelList = std::vector<Channel*>;
  private:
  std::atomic_bool is_looping_;
  std::atomic_bool has_quit_;
  std::atomic_bool is_handling_event_;
  std::atomic_bool is_calling_pending_functors;
  int64_t iteration_;
  const pid_t thread_id_;

  Timestamp poll_return_time_;
  std::unique_ptr<Epoll> epoll_;
  // std::unique_ptr<TimeQueue> time_queue_;
  int weakup_fd_;
  std::unique_ptr<Channel> weakup_channel_;
  std::any context_;

  ChannelList active_channels_;
  Channel* current_active_channel_;
  mutable std::mutex mutex_;
  std::vector<Functor> pending_functors_;
};
}

#endif
