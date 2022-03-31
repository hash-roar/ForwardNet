#ifndef __FNET_NET_EVENTLOOP_
#define __FNET_NET_EVENTLOOP_

#include <atomic>
#include <cstdint>

namespace net {

class EventLoop {
  private:
  std::atomic_bool is_looping_;
  std::atomic_bool has_quit_;
  std::atomic_bool is_handling_event_;
  std::atomic_bool is_calling_pending_functors;
  int64_t iteration_;
  const pid_t 

};
}

#endif
