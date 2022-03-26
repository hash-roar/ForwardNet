#ifndef __FNET_BASE_SOCK_
#define __FNET_BASE_SOCK_

#include "inet_addr.h"
#include <cstddef>
#include <cstdint>
#include <endian.h>
#include <netinet/in.h>
#include <sys/socket.h>
namespace base {

// socket_encapsulation

class InetAddress;

class Socket {
  public:
  explicit Socket(int sockfd)
      : sockfd_(sockfd)
  {
  }

  // Socket(Socket&&) // move constructor in C++11
  ~Socket();

  int fd() const { return sockfd_; }
  // return true if success.
  // bool getTcpInfo(struct tcp_info*) const;
  // bool getTcpInfoString(char* buf, int len) const;

  /// abort if address in use
  void bindAddress(const InetAddress& localaddr);
  /// abort if address in use
  void listen();

  /// On success, returns a non-negative integer that is
  /// a descriptor for the accepted socket, which has been
  /// set to non-blocking and close-on-exec. *peeraddr is assigned.
  /// On error, -1 is returned, and *peeraddr is untouched.
  int accept(InetAddress* peeraddr);

  void shutdownWrite();

  ///
  /// Enable/disable TCP_NODELAY (disable/enable Nagle's algorithm).
  ///
  void setTcpNoDelay(bool on);

  ///
  /// Enable/disable SO_REUSEADDR
  ///
  void setReuseAddr(bool on);

  ///
  /// Enable/disable SO_REUSEPORT
  ///
  void setReusePort(bool on);

  ///
  /// Enable/disable SO_KEEPALIVE
  ///
  void setKeepAlive(bool on);

  private:
  const int sockfd_;
};

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
void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in6* addr);

void toIpPort(char* buf, size_t size, const struct sockaddr* addr);
void toIp(char* buf, size_t size, const struct sockaddr* addr);

// sock cast

const struct sockaddr* cast(const struct sockaddr_in6* addr);
//{
// return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
//}

// const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
// const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);
// struct sockaddr* sockaddr_cast(struct sockaddr_in6* addr);
// const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);
// const struct sockaddr_in6* sockaddr_in6_cast(const struct sockaddr* addr);

struct sockaddr_in6 getLocalAddr(int sockfd);
struct sockaddr_in6 getPeerAddr(int sockfd);
bool isSelfConnect(int sockfd);

} // namespace base

#endif
