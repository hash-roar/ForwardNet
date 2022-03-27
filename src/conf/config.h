#ifndef __FNET_CONFIG_
#define __FNET_CONFIG_

#include <string>
#include <vector>

#include "../base/inet_addr.h"
namespace appconfig {

enum class ConfigError {
  NONE,
  CONF_FILE_NOT_EXIST,
  CONF_SYNTAX_ERROR,
  CONF_PARSE_ERROR,
  CONF_READ_ERROR,
};

class InetAddress;

struct NetConfig {
  base::InetAddress sec;
  base::InetAddress des;
};

struct BaseConfig {
  std::string conf_path;
  uint is_backgound=0;
  std::string log_path;
};

class AppConfig {
  public:
  AppConfig() { }
  ~AppConfig() { }

  ConfigError parseConfFile();



  private:
  std::vector<NetConfig> net_configs;
  BaseConfig base_config;
};

} // namespace appconfig

#endif
