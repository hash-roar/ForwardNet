#include "Timestamp.h"
#include <chrono>
#include <cstdint>
#include <string>

using namespace base;

using namespace std::chrono;

Timestamp Timestamp::now()
{
  auto now_from_poch = duration_cast<microseconds>(system_clock::now().time_since_epoch());
  return Timestamp(now_from_poch.count());
}
std::string Timestamp::toString() const
{
  //char buf[32]={0};
  //int64_t second = time_point_cast<seconds>(time_point_).time_since_epoch().count();
  //int64_t microsecond = time_point_cast<microseconds>(time_point_).time_since_epoch().count();
  //
  //TODO
}

std::string Timestamp::toFormattedString() const
{
  //TODO
}
