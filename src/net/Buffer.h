#ifndef __FNET_NET_BUFFER_
#define __FNET_NET_BUFFER_

#include <cstddef>
#include <tuple>
#include <vector>
namespace net {

class Buffer {
  public:
  static const size_t kCheapPrepend = 8;
  static const size_t kInitSzie = 1024;

  explicit Buffer(size_t initSize = kInitSzie)
      : buffer_(kCheapPrepend + initSize)
      , read_index_(kCheapPrepend)
      , write_index_(kCheapPrepend)
  {
  }

  void swap(Buffer& rhs)
  {
    buffer_.swap(rhs.buffer_);
    std::swap(read_index_, rhs.read_index_);
    std::swap(write_index_, rhs.write_index_);
  }

  size_t readableByte() const
  {
    return write_index_ - read_index_;
  }

  private:
  std::vector<char> buffer_;
  size_t read_index_;
  size_t write_index_;

  static const char kCRLF[];
};
}

#endif
