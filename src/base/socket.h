#ifndef __FNET_BASE_SOCK_
#define __FNET_BASE_SOCK_

#include <cstddef>
#include <cstdint>
#include <endian.h>
#include <netinet/in.h>
#include <sys/socket.h>
namespace base {

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

// endian cast
//
inline uint16_t hostToNetwork16(uint16_t host16)
{
  return htobe16(host16);
}
inline uint32_t hostToNetwork32(uint32_t host32)
{
  return htobe32(host32);
}

// ip cast

void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in* addr);
void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in6* addr);

void toIpPort(char* buf, size_t size, const struct sockaddr* addr);
void toIp(char* buf, size_t size, const struct sockaddr* addr);

// sock cast

const struct sockaddr* cast(const struct sockaddr_in6* addr);
//{
// return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
//}

const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);
struct sockaddr* sockaddr_cast(struct sockaddr_in6* addr);
const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);
const struct sockaddr_in6* sockaddr_in6_cast(const struct sockaddr* addr);

struct sockaddr_in6 getLocalAddr(int sockfd);
struct sockaddr_in6 getPeerAddr(int sockfd);
bool isSelfConnect(int sockfd);

} // namespace base

#endif
