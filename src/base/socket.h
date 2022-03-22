#ifndef __FNET_BASE_
#define __FNET_BASE_

#include <netinet/in.h>
#include <sys/socket.h>
namespace base {
  
  template<typename To,typename From>
  inline To implicit_cast(From const &f)
  {
    return f;
  }

  template<typename From,typename To>
  inline To down_cast(From * f)
  {
    if (false) {
      implicit_cast<From,To>(0);
    }

    return static_cast<To>(f);
  }

   const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
   const struct sockaddr* cast(const struct sockaddr_in6* addr);
  //{
    //return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
  //}
   const struct sockaddr* sockaddr_in_cast(struct sockaddr_in);
   const struct sockaddr* sockaddr_in6_cast(const struct sockaddr_in* addr);

   struct sockaddr_in6 getLocalAddr(int sockfd);
   struct sockaddr_in6 getPeerAddr(int sockfd);
   bool isSelfConnect(int sockfd);

} //namespace base



#endif
