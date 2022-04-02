#ifndef __FNET_NET_EVENTLOOP_
#define __FNET_NET_EVENTLOOP_

#include "Channel.h"
#include <atomic>
#include <cstdint>
#include <sys/types.h>
#include <vector>

namespace net {

//using ChannelList = std::vector<Channel*>;
 typedef   std::vector<Channel*> ChannelList;

class EventLoop {
  public:

    void assertInLoopThread()
    {
    }

  private:
  // using ChannelList = std::vector<Channel*>;
  private:
  std::atomic_bool is_looping_;
  std::atomic_bool has_quit_;
  std::atomic_bool is_handling_event_;
  std::atomic_bool is_calling_pending_functors;
  int64_t iteration_;
  const pid_t thread_id_;

  Timestamp pool_return_time_;
  // std::unique_ptr
};
}

#endif
