#pragma once

#include <cstddef>
#include <cstring>
#include <memory.h>
#include <string>
#include <string_view>

namespace flog {
template <int SIZE>
class FixedBuffer {
  public:
  FixedBuffer()
      : cur_(data_)
  {
  }

  void append(const char* buffer, std::size_t len)
  {
    if (static_cast<std::size_t>(avail()) > len) {
      memcpy(cur_, buffer, len);
      cur_ += len;
    }
  }

  const char* data() const { return data_; }
  int length() const { return static_cast<int>(cur_ - data_); }
  char* cur() { return cur_; }
  int avail() const { return static_cast<int>(end() - cur_); }
  void add(std::size_t len) { cur_ += len; }

  void reset() { cur_ = data_; }
  void bzero() { memset(data_, 0, sizeof(data_)); }

  private:
  const char* end() const { return data_ + sizeof(data_); }
  char data_[SIZE];
  char* cur_;
};

class LogStream {
  using self = LogStream;

  public:
  using Buffer = FixedBuffer<4000>;
  self& operator<<(bool v)
  {
    buffer_.append(v?"1":"0", 1);
    return *this;
  }

  // operator overloading for 

  self& operator<<(const char* str)
  {
    if (str) {
    buffer_.append(str, strlen(str));
    }else {
    buffer_.append("(null)", 6);
    }

    return *this;
  }

  self& operator<<(const unsigned char* str)
  {
    return operator<<(reinterpret_cast<const char*>(str));
  }

  self& operator<<(const std::string& v)
  {
    buffer_.append(v.c_str(), v.size());
    return *this;
  }

  self& operator<<(const std::string_view & v)
  {
    buffer_.append(v.data(), v.size());
    return *this;
  }

  self& operator<<(const Buffer& v)
  {

    return *this;
  }

  private:
  Buffer buffer_;

};
}
