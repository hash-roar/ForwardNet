#ifndef __FNET_CONFIG_
#define __FNET_CONFIG_

#include <string>
#include <vector>
namespace appconfig {
struct NetConfig {
  std::string src_ip;
  std::string src_port;
  std::string des_ip;
  std::string des_port;
};

struct BaseConfig{
  std::string conf_path;
  uint is_backgound;
};

struct AppConfig {
  std::vector<NetConfig>  net_configs;
};

} // namespace appconfig

#endif
