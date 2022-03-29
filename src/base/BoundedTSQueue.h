#ifndef __FNET_BASE_BTSQ_
#define __FNET_BASE_BTSQ_

// bounded thread safe queue with comdition variable

#include <chrono>
#include <condition_variable>
#include <limits>
#include <mutex>
#include <queue>
#include <utility>
namespace base {

template <typename T>
class BoundedTSQueue {
  public:
  using vlaue_type = T;
  using size_typ = typename std::queue<T>::size_type;

  enum QueError {
    None,
    QueFull,
    QueLocked
  };

  explicit BoundedTSQueue(size_typ queueMaxSize = std::numeric_limits<size_typ>::max())
      : que_max_size_(queueMaxSize)
  {
  }

  BoundedTSQueue(const BoundedTSQueue&) = delete;
  BoundedTSQueue operator=(const BoundedTSQueue&) = delete;

  size_typ getMaxSize() const { return que_max_size_; }
  size_typ getCurrentSize()
  {
    std::lock_guard<std::mutex> lock_guard(mutex_);
    return queue_.size();
  }

  bool isFull()
  {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size() = que_max_size_;
  }

  bool isEmpty()
  {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
  }

  bool front(T* dataPtr = nullptr)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    const bool is_empty = queue_.empty();
    if (!is_empty && dataPtr != nullptr) {
      *dataPtr = queue_.front();
    }
    return !is_empty;
  }

  QueError push(const T& t)
  {
    std::lock_guard<std::mutex> lock_guard(mutex_);
    if (queue_.size() >= que_max_size_) {
      return QueFull;
    }
    queue_.push(t);
    cond_.notify_one();
    return None;
  }
  QueError push(T&& t)
  {
    std::lock_guard<std::mutex> lock_guard(mutex_);
    if (queue_.size() >= que_max_size_) {
      return QueFull;
    }
    queue_.emplace(t);
    cond_.notify_one();
    return None;
  }
  // bool pop(T* dataPtr = nullptr)
  //{
  // std::lock_guard<std::mutex> lock_guard(mutex_);
  // if (!queue_.empty()) {
  // if (dataPtr != nullptr) {
  //*dataPtr = std::move(queue_.front());
  //}
  // queue_.pop();
  // return true;
  //}
  // return false;
  //}

  bool popWait(T* dataPtr = nullptr)
  {
    std::unique_lock<std::mutex> lock_guard(mutex_);
    cond_.wait(lock_guard, [this] {
      return !queue_.empty();
    });

    if (dataPtr != nullptr) {
      *dataPtr = std::move(queue_.front());
    }
    queue_.pop();
    return true;
  }

  bool popWaitTime(unsigned int time_ms, T* dataPtr = nullptr)
  {
    std::unique_lock<std::mutex> lock_guard(mutex_);
    auto duration_time = std::chrono::milliseconds(time_ms);

    bool wait_in_time = cond_.wait_for(lock_guard, duration_time, [this] {
      return !queue_.empty();
    });
    if (wait_in_time) {  // if is not out of time 
      if (!queue_.empty()) {
        if (dataPtr != nullptr) {
          *dataPtr = std::move(queue_.front());
        }
      }

      queue_.pop();
      return true;
    }
    return false;
  }

  private:
  std::queue<T> queue_;
  size_typ que_max_size_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

} // namespace base

#endif
