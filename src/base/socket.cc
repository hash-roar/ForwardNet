
#include "socket.h"
#include <arpa/inet.h>
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
namespace base {

// socket_encapsulation

Socket::~Socket()
{
  if (::close(sockfd_)<0) {
    printf("close socket error");
  }
}

const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr)
{
  return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}

struct sockaddr* sockaddr_cast(struct sockaddr_in6* addr)
{
  return static_cast<struct sockaddr*>(implicit_cast<void*>(addr));
}

const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr)
{
  return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}

const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr)
{
  return static_cast<const struct sockaddr_in*>(implicit_cast<const void*>(addr));
}

const struct sockaddr_in6* sockaddr_in6_cast(const struct sockaddr* addr)
{
  return static_cast<const struct sockaddr_in6*>(implicit_cast<const void*>(addr));
}

void toIpPort(char* buf, size_t size,
    const struct sockaddr* addr)
{
  if (addr->sa_family == AF_INET6) {
    buf[0] = '[';
    toIp(buf + 1, size - 1, addr);
    size_t end = ::strlen(buf);
    const struct sockaddr_in6* addr6 = sockaddr_in6_cast(addr);
    uint16_t port = networkToHost16(addr6->sin6_port);
    snprintf(buf + end, size - end, ":%u", port);
  }
  void toIp(char* buf, size_t size, const struct sockaddr* addr)
  {
    if (addr->sa_family == AF_INET) {
      assert(size >= INET_ADDRSTRLEN);
      const struct sockaddr_in* addr4 = sockaddr_in_cast(addr);
      ::inet_ntop(AF_INET, &addr4->sin_addr, buf, static_cast<socklen_t>(size));
    } else if (addr->sa_family == AF_INET6) {
      assert(size >= INET_ADDRSTRLEN);
      const struct sockaddr_in6* addr4 = sockaddr_in6_cast(addr);
      ::inet_ntop(AF_INET, &addr4->sin6_addr, buf, static_cast<socklen_t>(size));
    }
  }

  void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in* addr)
  {
    addr->sin_family = AF_INET;
    addr->sin_port = hostToNetwork16(port);
    if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0) {
      printf("something wrong in from ip port");
    }
  }

  void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in6* addr)
  {
    addr->sin6_family = AF_INET;
    addr->sin6_port = hostToNetwork16(port);
    if (::inet_pton(AF_INET6, ip, &addr->sin6_addr) <= 0) {
      printf("something wrong in from ip port");
    }
  }

} // namespace base
