#ifndef __FNET_BASE_TOOL_
#define __FNET_BASE_TOOL_

#include <string>
#include <vector>
#include <cstddef>
#include <string.h>

//#define DEBUG_LOG(args) 

namespace base {

  std::vector<std::string> split_str(const std::string &str);
  
  inline void memZero(void* p,size_t n)
  {
    memset(p,0,n);
  }


} //namespace base


#endif
