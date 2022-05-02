#include "LogManager.h"
#include "base.h"
#include "logger.h"
#include <memory>
#include <mutex>
#include <utility>

using namespace flog;

// thread safe
LogManager& LogManager::instance()
{
  static LogManager static_instance;
  return static_instance;
}

std::shared_ptr<logger> LogManager::default_logger()
{
  return default_logger_;
}

void LogManager::register_logger(std::shared_ptr<logger> new_logger)
{
  std::lock_guard<std::mutex> lock(logger_map_mutex_);
  register_logger_(new_logger);
}

void LogManager::register_logger_(std::shared_ptr<logger> new_logger)
{
  auto logger_name = new_logger->name();
  if (loggers_.find(logger_name) != loggers_.end()) {
    throw_flog_ex("logger with name" + logger_name + "  already exist");
  }
  loggers_[logger_name] = std::move(new_logger);
}

std::optional<std::shared_ptr<logger>> LogManager::get_logger(const std::string & logger_name)
{

}
