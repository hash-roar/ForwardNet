
#ifndef __FNET_BASE_
#define __FNET_BASE_

#include <string>
#include <vector>
#include <cstddef>
#include <string.h>

namespace base {

  std::vector<std::string> split_str(const std::string &str);
  
  inline void memZero(void* p,size_t n)
  {
    memset(p,0,n);
  }
} //namespace base


#endif
