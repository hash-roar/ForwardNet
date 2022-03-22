
#include "socket.h"
#include <netinet/in.h>
#include <sys/socket.h>

namespace base {

struct sockaddr* sockaddr_cast(struct sockaddr_in6* addr)
{
  return static_cast<struct sockaddr*>(implicit_cast<void*>(addr));
}
const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr)
{
  return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}
const struct sockaddr* sockaddr_in_cast(const struct sockaddr_in6* addr)
{
  return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}
  const struct sockaddr* sockaddr_in6_cast(const struct sockaddr_in6* addr)
  {
    return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
  }
} // namespace base
