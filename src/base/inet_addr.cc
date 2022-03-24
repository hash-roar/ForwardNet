
#include "inet_addr.h"
#include "./tool.h"
#include "socket.h"
#include <cstdint>
#include <cstring>
#include <netinet/in.h>
#include <string_view>
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
    addr6_.sin6_port = hostToNetwork16(portArg);
  }else {
    memZero(&addr_,sizeof(addr_));
    addr_.sin_family = AF_INET;
    in_addr_t ip = loopback? INADDR_LOOPBACK : INADDR_ANY;
    addr_.sin_addr.s_addr =hostToNetwork32(ip);
    addr_.sin_port = hostToNetwork16(portArg); 
  }

}

InetAddr::InetAddr(std::string_view ip,uint16_t portArg,bool ipv6)
{
  if (ipv6 || strchr(ip.data(),':')) {
    memZero(&addr6_,sizeof(addr6_));
  }
}

std::string InetAddr::toIpPort() const
{
  char buf[64] = "";

}
