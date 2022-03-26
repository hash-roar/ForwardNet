#include <netinet/tcp.h>
#include "socket.h"
#include "base.h"
#include "inet_addr.h"
#include "tool.h"
#include <arpa/inet.h>
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
namespace base {

class InetAddress;

// socket_encapsulation
Socket::~Socket()
{
  if (::close(sockfd_) < 0) {
    printf("close socket error");
  }
}

void Socket::bindAddress(const InetAddress& localaddr)
{
  auto ret = ::bind(sockfd_, localaddr.getSockAddr(),
      static_cast<socklen_t>(sizeof(struct sockaddr_in)));
  if (ret < 0) {
    DEBUG_LOG
  }
}

void Socket::listen()
{
  auto ret = ::listen(sockfd_, SOMAXCONN);
  if (ret < 0) {
    DEBUG_LOG
  }
}

int Socket::accept(InetAddress *peeraddr)
{
  struct sockaddr_in addr;
  memZero(&addr, sizeof addr);
  socklen_t addr_len = 
    static_cast<socklen_t>(sizeof(*peeraddr->getSockAddr()));
  int connfd = ::accept(sockfd_,reinterpret_cast<struct sockaddr*>(&addr),&addr_len);
  if (connfd >= 0)
  {
    peeraddr->setSockAddr(addr);
  }
  return connfd;
}

void Socket::shutdownWrite()
{
  if (::shutdown(sockfd_,SHUT_WR)<0) {
    DEBUG_LOG
  }
}

void Socket::setTcpNoDelay(bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY,
               &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReuseAddr(bool on)
{
   int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
               &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReusePort(bool on)
{
#ifdef SO_REUSEPORT
  int optval = on ? 1 : 0;
  int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT,
                         &optval, static_cast<socklen_t>(sizeof optval));
  if (ret < 0 && on)
  {
    DEBUG_LOG
  }
#else
  if (on)
  {
    DEBUG_LOG
  }
#endif
}

void Socket::setKeepAlive(bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
               &optval, static_cast<socklen_t>(sizeof optval));
  // FIXME CHECK
}



} // namespace base
