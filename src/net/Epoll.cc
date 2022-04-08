#include "Epoll.h"
#include "../base/cast.h"
#include "../base/tool.h"
#include "Channel.h"
#include "EventLoop.h"
#include <asm-generic/errno-base.h>
#include <cassert>
#include <cerrno>
#include <cstddef>
#include <sys/epoll.h>
#include <unistd.h>

using namespace base;
using namespace net;

namespace {
const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;
}

Epoll::Epoll(EventLoop* loop)
    : owner_loop_(loop)
    , epollfd_(::epoll_create1(EPOLL_CLOEXEC))
    , events_(kInitEventListSize)
{
  if (epollfd_ < 0) {
    // DEBUG //TODO
  }
}

Epoll::~Epoll()
{
  ::close(epollfd_);
}

Timestamp Epoll::poll(int timeutMs, ChannelList* activeChannels)
{
  int num_events = ::epoll_wait(epollfd_,
      &*events_.begin(),
      static_cast<int>(events_.size()),
      timeutMs);

  int save_errono = errno;

  Timestamp now(Timestamp::now());
  if (num_events > 0) {
    fillActiveChannels(num_events, activeChannels);

    if (implicit_cast<size_t>(num_events) == events_.size()) { // be care for every cast
      events_.resize(events_.size() * 2);
    }
  } else if (num_events == 0) {
    // DEBUG //TODO
  } else {
    if (save_errono != EINTR) {
      // DEBUG //TODO
    }
  }

  return now;
}

void Epoll::fillActiveChannels(int numEvents, ChannelList* activeChannels) const
{
  assert(implicit_cast<size_t>(numEvents) <= events_.size());
  for (int i = 0; i < numEvents; ++i) {
    Channel* channel = static_cast<Channel*>(events_[i].data.ptr);

    channel->setReceiveEvents(events_[i].events);
    activeChannels->push_back(channel);
  }
}

void Epoll::updateChannel(Channel* channel)
{
  assertInLoopThread();
  const int index = channel->index();
  // DEBUG //TODO
  if (index == kNew || index == kDeleted) {
    int fd = channel->fd();
    if (index == kNew) {
      assert(channels_.find(fd) == channels_.end());
      channels_[fd] = channel;
    } else {
      assert(channels_.find(fd) != channels_.end());
      assert(channels_[fd] == channel);
    }
    channel->setIndex(kAdded);
    update(EPOLL_CTL_ADD, channel);

  } else {
    int fd = channel->fd();
    (void)fd;
    assert(channels_.find(fd) != channels_.end());
    assert(channels_[fd] == channel);
    assert(index == kAdded);
    if (channel->isNoneEvent()) {
      update(EPOLL_CTL_DEL, channel);
      channel->setIndex(kDeleted);
    } else {
      update(EPOLL_CTL_DEL, channel);
    }
  }
}

void Epoll::removeChannel(Channel* channel)
{
  assertInLoopThread();
  int fd = channel->fd();
  assert(channels_.find(fd) != channels_.end());
  assert(channels_[fd] == channel);
  assert(channel->isNoneEvent());

  int index = channel->index();
  assert(index == kAdded || index == kDeleted);
  size_t n = channels_.erase(fd);
  assert(n == 1);
  if (index == kAdded) {
    update(EPOLL_CTL_DEL, channel);
  }
  channel->setIndex(kNew);
}

void Epoll::update(int operation, Channel* channel)
{
  struct epoll_event event;
  memZero(&event, sizeof(event));
  event.events = channel->events();
  event.data.ptr = channel;

  int fd = channel->fd();
  if (::epoll_ctl(epollfd_, operation, fd, &event) < 0) {
    // DEBUG_LOG //TODO
  }
}

bool Epoll::hasChannel(Channel* channel)
{
  assertInLoopThread();
  auto iter = channels_.find(channel->fd());
  return iter != channels_.end() && iter->second == channel;
}

const char* Epoll::operationToString(int op)
{
  // TODO
  //
  return "";
}
