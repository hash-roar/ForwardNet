#include "tool.h"
#include <cstdio>
#include <filesystem>
#include <string>

using namespace base;
namespace {

const static int kMaxReadSringBuf = 128*1024;

std::vector<std::string> split_str(const std::string& str)
{
  std::vector<std::string> result {};
  size_t start = 0;
  size_t pos = str.find('/');
  while (pos != std::string::npos) {
    if (pos > start) {
      result.push_back(str.substr(start, pos = start));
    }
    start = pos + 1;
    pos = str.find('/', start);
  }

  if (start < str.length()) {
    result.push_back(str.substr(start));
  }

  return result;
}

  BaseError readFileToString(std::string & str,std::string const path)
  {
    namespace fs=std::filesystem;
    auto fs_file_path = fs::path{path};
    auto file_size = fs::file_size(fs_file_path);
    if (file_size > kMaxReadSringBuf){
      return BaseError::ReadFileTooLarge;
    }
    auto file_ptr =  ::fopen(path.c_str(),"r");
    if (file_ptr==nullptr) {
      return BaseError::ReadFileToStringError;
    }
    str.resize(file_size);
    fread(const_cast<char *>(str.data()),file_size,1,file_ptr);
    fclose(file_ptr);
    return BaseError::NONE;

  }


}
