#ifndef __FNET_BASE_TIMESTAMP_
#define __FNET_BASE_TIMESTAMP_

#include <chrono>
#include <cstdint>
#include <ctime>
#include <string>
#include <type_traits>
namespace base {
class Timestamp {
  using time_point_type = std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds>;
  using time_micro_type = std::chrono::microseconds;

  public:
  Timestamp()
      : time_point_(time_micro_type::zero())
  {
  }

  explicit Timestamp(int64_t microsecond)
      : time_point_(time_micro_type(microsecond))
  {
  }
  // explicit Timestamp(std::time_t microsecond):time_point_(time_micro_type(microsecond)){}
  explicit Timestamp(time_point_type const& time_pt)
      : time_point_(time_pt)
  {
  }

  int64_t microSecondSinceEpoch() const { return time_point_.time_since_epoch().count(); }
  std::time_t secondSinceEpoch() const
  {
    // return static_cast<time_t>(time_point_.time_since_epoch().count() / KMicroSecondPerSecond);
    return std::chrono::system_clock::to_time_t(time_point_);
  }

  time_point_type get_time_point() const { return time_point_; }

  void swap(Timestamp& other)
  {
    std::swap(time_point_, other.time_point_);
  }

  std::string toString() const;
  std::string toFormattedString() const;

  static Timestamp now();
  static Timestamp invalid()
  {
    return Timestamp();
  }
  static Timestamp fromUnixTime(std::time_t time) // implicit cast from time_t to int64_t??
  {
    return fromUnixTime(time, 0);
  }
  static Timestamp fromUnixTime(std::time_t time, int microsecond)
  {
    return Timestamp(static_cast<int64_t>(time) * KMicroSecondPerSecond + microsecond);
  }

  static const int KMicroSecondPerSecond = 1000 * 1000;

  private:
  time_point_type time_point_;
};

inline bool operator <(Timestamp lhs,Timestamp rhs)
{
  return lhs.microSecondSinceEpoch() < rhs.microSecondSinceEpoch();
}

inline bool operator ==(Timestamp lhs,Timestamp rhs)
{
  return lhs.microSecondSinceEpoch() == rhs.microSecondSinceEpoch();
}

inline double timeDifferenceNum(Timestamp high,Timestamp low)
{
  auto diff = high.microSecondSinceEpoch() - low.microSecondSinceEpoch();
  return static_cast<double>(diff)/Timestamp::KMicroSecondPerSecond;
}

inline auto timeDifference(Timestamp high, Timestamp low)
{
  return high.get_time_point()-low.get_time_point();
}




}

#endif
