#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"
#include "Thread.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <sys/eventfd.h>
#include <unistd.h>
#include <vector>

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
    , weakup_fd_(createEventfd())
    , weakup_channel_(new Channel(this, weakup_fd_))
    , current_active_channel_(nullptr)
{
    if (loop_in_this_thread) {
        // has another loop exist
    } else {
        loop_in_this_thread = this;
    }
    weakup_channel_->setReadCallBack(std::bind(&EventLoop::handleRead, this));
    weakup_channel_->enableReading();
}

EventLoop::~EventLoop()
{
    weakup_channel_->disableAll();
    weakup_channel_->remove();
    ::close(weakup_fd_);
    loop_in_this_thread = nullptr;
}

void EventLoop::loop()
{
    assert(!is_looping_);
    assertInLoopThread();
    is_looping_ = true;
    has_quit_ = false;

    while (has_quit_) {
        active_channels_.clear();
        poll_return_time_ = epoll_->poll(kPollTimeMs, &active_channels_);
        ++iteration_;

        is_handling_event_ = true;
        for (const auto& channel : active_channels_) {
            current_active_channel_ = channel;
            current_active_channel_->handleEvent(poll_return_time_);
        }

        current_active_channel_ = nullptr;
        is_handling_event_ = false;
        doPendingFunctors();
    }

    is_looping_ = false;
}

void EventLoop::quit()
{
    has_quit_ = true;
    if (!isInLoopThread()) {
        wakeup();
    }
}

void EventLoop::runInLoop(Functor callback)
{
    if (isInLoopThread()) {
        callback();
    } else {
        queueInLoop(std::move(callback));
    }
}

void EventLoop::queueInLoop(Functor callback)
{
    {
        std::lock_guard<std::mutex> lock_guard_(mutex_);
        pending_functors_.push_back(std::move(callback));
    }
    if (!isInLoopThread() || is_calling_pending_functors) {
        wakeup();
    }
}

void EventLoop::updateChannel(Channel* channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    epoll_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
    assert(channel->ownerLoop());
    assertInLoopThread();
    if (is_handling_event_) {
        assert(current_active_channel_ == channel || std::find(active_channels_.begin(), active_channels_.end(), channel) == active_channels_.end());
    }
    epoll_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel* channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    return epoll_->hasChannel(channel);
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = ::read(weakup_fd_, &one, sizeof(one));
    if (n != sizeof(one)) {
        DEBUG_LOG
    }
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    is_calling_pending_functors = true;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        functors.swap(pending_functors_);
    }

    for (const auto& functor : functors) {
        functor();
    }
    is_calling_pending_functors=false;
}
