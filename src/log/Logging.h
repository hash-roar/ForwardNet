#pragma once

#include <string>

namespace flog {

class Logger {
  public:
    enum LogLevel
    {
      DEBUG,
      INFO,
      WARN,
      ERROR,
      FATAL
    };


    Logger(std::string fileName,int line);
    Logger(std::string fileName,int line,LogLevel level);
    Logger(std::string fileName,int line,LogLevel level,const char*functionName);


};
}
