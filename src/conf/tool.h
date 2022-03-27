#ifndef __FNET_BASE_TOOL_
#define __FNET_BASE_TOOL_

#include <string>
#include <tuple>
#include <filesystem>
#include <vector>
#include <cstddef>
#include <string.h>

//#define DEBUG_LOG(args) 

namespace base {


  enum class BaseError{
    NONE,
    ReadFileToStringError,
    ReadFileTooLarge
  };

  std::vector<std::string> split_str(const std::string &str);
  
  inline void memZero(void* p,size_t n)
  {
    memset(p,0,n);
  }

  BaseError readFileToString(std::string & str,std::string const path);

} //namespace base


#endif
