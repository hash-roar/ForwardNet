#ifndef __FNET_BASE_
#define __FNET_BASE_

#include <netinet/in.h>
namespace base {
  
  template<typename From,typename To>
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

  const struct sockaddr* sockadd_cast(const struct sockaddr_in6* addr)
  {
    return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
  }
} //namespace base



#endif
