
#include "tool.h"

namespace {

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
}
