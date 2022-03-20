#ifndef __FNET_BASE_
#define __FNET_BASE_

#include <cstdint>
#include <netinet/in.h>
#include <string_view>
#include <sys/socket.h>
namespace base {

class InetAddr {
  public:
  explicit InetAddr(uint16_t port = 0, bool loopback = false, bool ipv6 = false);

  InetAddr(std::string_view ip, uint16_t port, bool ipv6 = false);

  explicit InetAddr(const struct sockaddr_in& addr)
      : addr_(addr)
  {
  }

  explicit InetAddr(const struct sockaddr_in6& addr)
      : addr6_(addr)
  {
  }

  sa_family_t family() const {return addr_.sin_family;}
  std::string toIp() const;
  std::string toIpPort() const;
  uint16_t port() const;

  const struct sockaddr* getSockAddr() const
  {
    return ;
  }

  private:
  union {
    struct sockaddr_in addr_;
    struct sockaddr_in6 addr6_;
  };
};

}; // namespace bae

#endif