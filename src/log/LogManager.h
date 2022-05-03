#pragma once

// this is singleton pattern
// we have to use cpp11+,so the singleton needn't double check

#include "base.h"
#include "logger.h"
#include <memory>
#include <mutex>
#include <optional>
#include <unordered_map>
namespace flog {
class LogManager : noncopyable {
  public:
  using log_levels = std::unordered_map<std::string, level::level_enum>;

  void register_logger(std::shared_ptr<logger> new_logger);
  std::optional<std::shared_ptr<logger>> get_logger(const std::string& logger_name);
  std::shared_ptr<logger> default_logger();
  void set_default_logger(std::shared_ptr<logger> new_logger);

  static LogManager& instance();

  private:
  LogManager(); // which means it can only be initialized by singleton
  ~LogManager();

  void register_logger_(std::shared_ptr<logger> new_logger);

  private:
  std::mutex logger_map_mutex_;

  std::unordered_map<std::string, std::shared_ptr<logger>> loggers_;
  log_levels log_levels_;
  std::shared_ptr<logger> default_logger_;
};
}
