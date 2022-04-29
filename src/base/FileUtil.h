#ifndef __FNET_FILE_H_
#define __FNET_FILE_H_

#include "base.h"
// this Encapsulatation is not for cross-platform

namespace base {

class File:noncopyable{
  public:
  enum FILE_ERR {
    NONE,
    BAD_FILE_NAME,
    FILE_NOT_EXIST
  };

  File() = default;

  private:

  std::string filename_;
  std::FILE * fd_{nullptr};
};
}

#endif
