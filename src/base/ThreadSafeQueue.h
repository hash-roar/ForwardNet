#ifndef __FNET_BASE_TSQ_
#define __FNET_BASE_TSQ_
#include <mutex>
#include <queue>
#include <utility>

namespace base {

template <typename T>
class ThreadSafeQueue {
  public:
  ThreadSafeQueue() = default;
  ThreadSafeQueue(ThreadSafeQueue&&) = default;
  ThreadSafeQueue& operator=(ThreadSafeQueue&&) = default;

  ThreadSafeQueue(const ThreadSafeQueue&) = delete;
  ThreadSafeQueue operator=(const ThreadSafeQueue&) = delete;

  void enqueue(T&&t)
  {
    std::unique_lock<std::mutex> lock_gurd(mutex_);
    queue_.emplace(t);
  }
  void enqueue(const T& t)
  {
    std::unique_lock<std::mutex> lock_gurd(mutex_);
    queue_.push(t);
  }

  bool dequeue(T&t)
  {
    std::unique_lock<std::mutex> lock_gurd(mutex_);
    if (queue_.empty()) {
      return false;
    }
    t = std::move(queue_.front());
    queue_.pop();
    return true;
  }

  bool empty()
  {
    std::unique_lock<std::mutex> lock_gurd(mutex_);
    return queue_.empty();
  }

  int size()
  {
    std::unique_lock<std::mutex> lock_gurd(mutex_);
    return queue_.size();
  }

  void clean()
  {
    std::unique_lock<std::mutex> lock_gurd(mutex_);
    while (!queue_.empty()) {
      queue_.pop();
    }
  }

  private:
  std::queue<T> queue_;
  std::mutex  mutex_;
};
} // namespace base

#endif
