#include "logger.h"
#include <string>

using namespace flog;

const std::string& logger::name() const
{
  return log_name_;
}
