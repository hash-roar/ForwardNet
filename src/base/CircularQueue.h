#ifndef __FNET_CIRCULARQUE_H_
#define __FNET_CIRCULARQUE_H_
#include <algorithm>
#include <assert.h>
#include <cstddef>
#include <functional>
#include <tuple>
#include <vector>

namespace base {

template <typename T>
class CircularQueue {
  public:
  enum QUE_ERROR {
    NONE,
    OUT_OF_RANGE
  };
  using value_type = T;
  CircularQueue() = default;
  explicit CircularQueue(std::size_t max_items)
      : max_items_(max_items + 1)
      , data_(max_items_)
  {
  }

  CircularQueue(const CircularQueue&) = default;
  CircularQueue& operator=(const CircularQueue&) = default;
  CircularQueue(CircularQueue&& other) noexcept
  {
    copy_moveable(std::move(other));
  }

  CircularQueue& operator=(CircularQueue&& other) noexcept
  {
    copy_moveable(std::move(other));
    return *this;
  }

  void push_back(T&& item)
  {
    if (max_items_ > 0) {
      data_[tail_] = std::move(item);
      tail_ += (tail_ + 1) % max_items_; // tail will not reach max_items_;
      if (head_ == tail_) {
        head_ = (head_ + 1) % max_items_;
        ++overrun_counter_;
      }
    }
  }

  const T& front() const
  {
    assert(max_items_ > 0);
    return data_[head_];
  }

  std::size_t size() const
  {
    if (tail_ >= head_) {
      return tail_ - head_;
    } else {
      return max_items_ - (head_ - tail_);
    }
  }

  const T& at(std::size_t i)
  {
    assert(i < size());
    return data_[(head_ + i) % max_items_];
  }

  std::tuple<const T&, QUE_ERROR> try_get(std::size_t i) const
  {
    if (i < size() && i >= 0) {
      return std::make_tuple(std::ref(data_[(head_ + 1) % max_items_]), NONE);
    } else {
      return std::make_tuple(std::ref(data_[head_]), OUT_OF_RANGE);
    }
  }

  void pop_front()
  {
    head_ = (head_ + 1) % max_items_;
  }

  bool empty() const
  {
    return tail_ == head_;
  }

  bool full() const
  {
    if (max_items_ > 0) {
      return ((tail_ + 1) % max_items_) == head_;
    }
  }

  std::size_t overrun_counter() const
  {
    return overrun_counter_;
  }

  private:
  void copy_moveable(CircularQueue&& other) noexcept
  {
    max_items_ = other.max_items_;
    head_ = other.head_;
    tail_ = other.tail_;
    overrun_counter_ = other.overrun_counter_;
    data_ = std::move(other.data_);

    // to disable other
    other.max_items_ = 0;
    other.head_ = 0;
    other.tail_ = 0;
    other.overrun_counter_ = 0;
  }

  std::size_t max_items_ = 0;
  std::size_t head_ = 0;
  std::size_t tail_ = 0;
  std::size_t overrun_counter_ = 0;
  std::vector<T> data_;
};

}

#endif
