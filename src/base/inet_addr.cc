
#include "inet_addr.h"
#include "tool.h"
#include <cstdint>
#include <netinet/in.h>
#include <sys/socket.h>

using namespace base;

InetAddr::InetAddr(uint16_t portArg, bool loopback, bool ipv6)
{
  // static_assert(offsetof(InetAddr,addr6_)==0,"addr6_ offset 0" );
  // static_assert(offsetof(InetAddr,addr_)==0,"addr6_ offset 0" );
  if (ipv6) {
    memZero(&addr6_, sizeof(addr6_));
    base::memZero(&addr6_, sizeof(addr6_));
    addr6_.sin6_family = AF_INET6;
    in6_addr ip = loopback ? in6addr_loopback : in6addr_any;
    addr6_.sin6_addr = ip;
    addr6_.sin6_port =
  }
}
