#include "FileUtil.h"
#include "base.h"
#include <bits/types/struct_tm.h>
#include <cerrno>
#include <cstdio>
#include <filesystem>
#include <tuple>

using namespace base;

// File::

File::FILE_ERR File::file_ok(std::string_view filename)
{
  namespace fs = std::filesystem;
  auto file_path = fs::path { filename };
  if (fs::exists(file_path)) {
    if (fs::is_regular_file(file_path)) {
      return NONE;
    } else {
      return BAD_FILE_TYPE;
    }
  } else {
    return FILE_NOT_EXIST;
  }
}

std::tuple<std::string, std::string> File::split_by_extension(const std::string& filename)
{
  namespace fs = std::filesystem;
  auto file_path = fs::path { filename };
  auto file_basename = file_path.stem();
  auto file_extension = file_path.extension();
  return std::make_tuple(file_basename.string(), file_extension.string());
}

void File::close()
{

  // we can do some other things here
  if (fd_ != nullptr) {
    std::fclose(fd_);
    fd_ = nullptr;
  }
}

File::FILE_ERR File::open(const std::string& filename, bool truncate)
{
  close();
  filename_ =filename;
  auto *mode = "ab";
  auto *truncate_mode = "wb";

  auto result = file_ok(filename);
  if (result!=NONE) {
    return result;
  }

  for (int tries=0; tries<open_tries_; ++tries) {
    if (truncate) {
      std::FILE *tmp;
      std::fopen(filename.c_str(),truncate_mode);
      if (tmp==nullptr) {
        continue; //try agin
      }
      std::fclose(tmp);
    }
    fd_ =  std::fopen(filename.c_str(), mode);
    if (fd_!=nullptr) {
      return NONE;
    }
  }
  return OPEN_FILE_ERROR;
}

void  File::reopen(bool truncate)
{
  if (filename_.empty()) {
    throw_fnet_ex("fail to reopen file,was not opened before");
  }
  this->open(filename_,truncate);
}

void File::flush()
{
  if (std::fflush(fd_)!=0) {
    throw_fnet_ex("failed to flush to file"+filename_);
  }
}

void File::write(const char *buffer, std::size_t len)
{
  if (( std::fwrite(buffer,1,len,fd_) )!=len) {
    throw_fnet_ex("failed to write to file"+filename_);
  }
}
std::size_t File::size() const
{
  auto file_path = std::filesystem::path{filename_};
  return std::filesystem::file_size(file_path);
}
