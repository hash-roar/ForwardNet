#include "base.h"
#include <utility>

using namespace flog;

const char* flog_exception::what() const noexcept
{
  return message_.c_str();
}

void throw_flog_ex(const std::string &message,int last_errno)
{
  throw(flog_exception(message,last_errno));
}
void throw_flog_ex(std::string message)
{
  throw(flog_exception(std::move(message)));
}


