#include "EventLoop.h"
#include "Epoll.h"
#include "Thread.h"
#include <memory>
#include <sys/eventfd.h>

using namespace base;
using namespace net;
using namespace net::CurrentThread;

namespace {
thread_local EventLoop* loop_in_this_thread = 0;
const int kPollTimeMs = 10000;

int createEventfd()
{
  int event_fd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (event_fd < 0) {
    // DEBUG_LOG
  }

  return event_fd;
}
}

EventLoop::EventLoop()
    : is_looping_(false)
    , has_quit_(false)
    , is_handling_event_(false)
    , is_calling_pending_functors(false)
    , iteration_(0)
    , thread_id_(tid())
    , epoll_(std::make_unique<Epoll>(Epoll(this)))
    , weakup_fd_()
