#ifndef __FNET_NET_BUFFER_
#define __FNET_NET_BUFFER_

#include <algorithm>
#include <assert.h>
#include <string>
#include <cstddef>
#include <string.h>
#include <string_view>
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
  // get buffer info
  size_t readableByte() const
  {
    return write_index_ - read_index_;
  }
  size_t writableBytes()
  {
    return buffer_.size() - write_index_;
  }
  size_t prependableBytes()
  {
    return read_index_;
  }
  const char* peek() const { return begin() + read_index_; }
  const char* findCRLF() const
  {
    auto crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF + 2);
    return crlf == beginWrite() ? nullptr : crlf;
  }
  const char* findCRLF(const char* start) const
  {
    assert(peek() <= start);
    assert(start <= beginWrite());
    auto crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF + 2);
    return crlf == beginWrite() ? nullptr : crlf;
  }
  const char* findEOL()
  {
    const void* eol = memchr(peek(), '\n', readableByte());
    return static_cast<const char*>(eol);
  }
  const char* findEOL(const char* start)
  {
    assert(peek() <= start);
    assert(start <= beginWrite());
    const void* eol = memchr(peek(), '\n', readableByte());
    return static_cast<const char*>(eol);
  }

  // get data back form buffer
  void retrieve(size_t len)
  {
    assert(len <= readableByte());
    if (len < readableByte()) {
      read_index_ += len;
    } else {
      retrieveAll();
    }
  }
  void retrieveInt64()
  {
    retrieve(sizeof(int64_t));
  }

  void retrieveInt32()
  {
    retrieve(sizeof(int32_t));
  }

  void retrieveInt16()
  {
    retrieve(sizeof(int16_t));
  }

  void retrieveInt8()
  {
    retrieve(sizeof(int8_t));
  }

  void retrieveUntill(const char* end)
  {
    assert(peek() <= end);
    assert(end <= beginWrite());
    retrieve(end - peek());
  }

  void retrieveAll()
  {
    read_index_ = kCheapPrepend;
    write_index_ = kCheapPrepend;
  }

  std::string retrieveAllAsString()
  {
    return retrieveAsString(readableByte());
  }
  std::string retrieveAsString(size_t len)
  {
    assert(len <= readableByte());
    std::string result(peek(), len);
    retrieve(len);
    return result;
  }
  
  std::string_view toStringView()const
  {
    return std::string_view(peek(),readableByte());
  }

  void append(const std::string_view&str)
  {
    
  }
  void append(const char* data,size_t len)
  {

  }

  public:
  char* beginWrite() { return begin() + write_index_; }
  const char* beginWrite() const { return begin() + write_index_; }

  private:
  char* begin()
  {
    return &*buffer_.begin();
  }
  const char* begin() const
  {
    return &*buffer_.begin();
  }

  private:
  std::vector<char> buffer_;
  size_t read_index_;
  size_t write_index_;

  static const char kCRLF[];
};
}

#endif
