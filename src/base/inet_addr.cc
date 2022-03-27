
#include "inet_addr.h"
#include "tool.h"  //memZero
#include "cast.h" //hostToNetwork16
#include <cassert> //assert
#include <netdb.h> //struct hostnet
#include "base.h" //debug_micro
#include <string_view>


using namespace base;

static thread_local char thread_resolve_buf[64*1024]; //static ?

InetAddress::InetAddress(uint16_t portArg, bool loopbackOnly, bool ipv6)
{
  static_assert(offsetof(InetAddress, addr6_) == 0, "addr6_ offset 0");
  static_assert(offsetof(InetAddress, addr_) == 0, "addr_ offset 0");
  if (ipv6)
  {
    memZero(&addr6_, sizeof addr6_);
    addr6_.sin6_family = AF_INET6;
    in6_addr ip = loopbackOnly ? in6addr_loopback : in6addr_any;
    addr6_.sin6_addr = ip;
    addr6_.sin6_port = hostToNetwork16(portArg);
  }
  else
  {
    memZero(&addr_, sizeof addr_);
    addr_.sin_family = AF_INET;
    in_addr_t ip = loopbackOnly ? INADDR_LOOPBACK : INADDR_ANY;
    addr_.sin_addr.s_addr = hostToNetwork32(ip);
    addr_.sin_port = hostToNetwork16(portArg);
  }
}

InetAddress::InetAddress(std::string_view ip, uint16_t portArg, bool ipv6)
{
    memZero(&addr_, sizeof addr_);
    fromIpPort(ip.data(), portArg, &addr_);
}

uint32_t InetAddress::ipv4NetEndian() const
{
  assert(family() == AF_INET);
  return addr_.sin_addr.s_addr;
}

uint16_t InetAddress::port() const
{
  return networkToHost16(portNetEndian());
}

bool InetAddress::resolve(std::string_view hostname, InetAddress* out)
{
  assert(out != NULL);
  struct hostent hent;
  struct hostent* he = NULL;
  int herrno = 0;
  memZero(&hent, sizeof(hent));

  int ret = gethostbyname_r(hostname.data(), &hent, thread_resolve_buf, sizeof(thread_resolve_buf), &he, &herrno);
  if (ret == 0 && he != NULL)
  {
    assert(he->h_addrtype == AF_INET && he->h_length == sizeof(uint32_t));
    out->addr_.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);
    return true;
  }
  else
  {
    if (ret)
    {
      //LOG_SYSERR << "InetAddress::resolve";
      DEBUG_LOG
    }
    return false;
  }
}



