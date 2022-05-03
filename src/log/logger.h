#pragma once

#include "base.h"
#include <string>
namespace flog {

class logger {
  using log_level_t = level::level_enum;
  public:
  logger();

  const std::string& name() const;
  private:
  std::string log_name_;
  log_level_t  level_{level::info};
    
};
}
