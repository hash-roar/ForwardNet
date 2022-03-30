#ifndef __FNET_BASE_TIMER_
#define __FNET_BASE_TIMER_

#include "base.h"
#include <atomic>
#include <chrono>
#include <cstdint>

namespace base {

class Timer {
  using time_pt_type = std::chrono::time_point<std::chrono::system_clock>;

  public:
  Timer(TimerCallback callBack, time_pt_type time_pt,
      double interval)
      : call_back_(callBack)
      , expiration_(time_pt)
      , interval_(interval)
      , repeat_(interval > 0.0)
      , sequence_(num_created_.fetch_add(1, std::memory_order_relaxed))
  {
  }

  void run() { call_back_(); }

  time_pt_type expiration() { return expiration_; }

  bool repeat() { return repeat_; }
  int64_t sequence() { return sequence_; }
  void restart(time_pt_type now);

  static int64_t numCreated() { return num_created_.load(std::memory_order_relaxed); }

  private:
  const TimerCallback call_back_;
  time_pt_type expiration_;
  const double interval_;
  const bool repeat_;
  const int64_t sequence_;

  static std::atomic_int64_t num_created_;
};
} // namespace base

#endif
