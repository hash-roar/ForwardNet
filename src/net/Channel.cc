#include "Channel.h"
#include <assert.h>
#include <memory>
#include <sys/poll.h>

using namespace net;
using namespace base;

const int Channel::kNoneEvent = 0;
const int kReadEvent = POLLIN | POLLPRI;
const int kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop)
    , fd_(fd)
    , register_events_(0)
    , receive_events_(0)
    , poll_index_(-1)
    , log_hup_(false)
    , tied_(false)
    , is_handling_event_(false)
    , added_to_loop_(false)
{
}

Channel::~Channel()
{
  assert(!is_handling_event_);
  assert(!added_to_loop_);
  // TODO
}

void Channel::tie(const std::shared_ptr<void>& obj)
{
  tie_ = obj;
  tied_ = true;
}

void Channel::update()
{
  added_to_loop_ = true;
  // TODO
}

void Channel::remove()
{
  assert(isNoneEvent());
  added_to_loop_ = false;
  // TODO
}

void Channel::handleEvent(Timestamp receiveTime)
{
  std::shared_ptr<void> guard;
  if (tied_) {
    guard = tie_.lock();
    if (guard) {
      handleEventWithGuard(receiveTime);
    }
  } else {
    handleEventWithGuard(receiveTime);
  }
}

void Channel::handleEventWithGuard(Timestamp receiveTime)
{
  is_handling_event_ = true;
  if ((receive_events_ & POLLHUP) && !(receive_events_ & POLLIN)) {
    if (log_hup_) {
      // TODO
    }
    if (close_callbcak_) {
      close_callbcak_();
    }
  }

  if (receive_events_ & POLLNVAL) {
    // TODO
  }

  if (receive_events_ & (POLLERR | POLLNVAL)) {
    if (error_callbcak_) {
      error_callbcak_();
    }
  }
}
