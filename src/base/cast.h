#ifndef __FNET_BASE_CAST_
#define __FNET_BASE_CAST_

#include <cstdint>
#include <endian.h>
#include <sys/socket.h>
#include <netinet/in.h>
namespace base {

// endian cast
//
inline uint64_t hostToNetwork64(uint64_t host64)
{
  return htobe64(host64);
}

inline uint32_t hostToNetwork32(uint32_t host32)
{
  return htobe32(host32);
}

inline uint16_t hostToNetwork16(uint16_t host16)
{
  return htobe16(host16);
}

inline uint64_t networkToHost64(uint64_t net64)
{
  return be64toh(net64);
}

inline uint32_t networkToHost32(uint32_t net32)
{
  return be32toh(net32);
}

inline uint16_t networkToHost16(uint16_t net16)
{
  return be16toh(net16);
}

// cast
template <typename To, typename From>
inline To implicit_cast(From const& f)
{
  return f;
}

template <typename From, typename To>
inline To down_cast(From* f)
{
  if (false) {
    implicit_cast<From, To>(0);
  }

  return static_cast<To>(f);
}

// ip cast

void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in* addr);
//void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in6* addr);

//void toIpPort(char* buf, std::size_t size, const struct sockaddr* addr);
//void toIp(char* buf, std::size_t size, const struct sockaddr* addr);


}

#endif
