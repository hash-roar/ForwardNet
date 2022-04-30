#ifndef __FNET_FILE_H_
#define __FNET_FILE_H_

#include "base.h"
#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>
#include <tuple>
// this Encapsulatation is not for cross-platform
// some of file system opration needs c++17 filesystem
// in most cases of filesystem error I think throwing exception is goode idea
// 
//
namespace base {

class File : noncopyable {
  public:
  enum FILE_ERR {
    NONE,
    OPEN_FILE_ERROR,
    BAD_FILE_TYPE,
    BAD_FILE_NAME,
    FILE_NOT_EXIST
  };

  File() = default;
  // this is only for regular file
  FILE_ERR open(const std::string& filename, bool truncate = false);
  void reopen(bool truncate);
  void flush();
  void close();
  void write(const char* buffer, std::size_t len);

  std::size_t size() const;

  static std::tuple<std::string,std::string> split_by_extension(const std::string& filename);


  // test file is exsit or is regular file
  FILE_ERR file_ok(std::string_view filename);
  std::string filename() const
  {
    return filename_;
  }

  private:
  const int open_tries_ = 5;
  const unsigned int open_interval_ = 10;
  std::string filename_; // this means that the file name can not be Chinese
  std::FILE* fd_ { nullptr };
};
}

#endif
