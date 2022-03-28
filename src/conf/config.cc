#include "config.h"
#include "../base/tool.h"
#include "../include/json/document.h"
#include <cassert>
#include <cstdint>
#include <filesystem>

using namespace appconfig;
using namespace rapidjson;

ConfigError AppConfig::parseConfFile()
{
  // ensure that config file exist
  namespace fs = std::filesystem;
  auto conf_file_path = base_config.conf_path;
  auto fs_conf_path = fs::path { conf_file_path };
  if (!fs::exists(fs_conf_path) || !fs::is_regular_file(fs_conf_path)) {
    return ConfigError::CONF_FILE_NOT_EXIST;
  }

  // read file
  std::string config_content;
  auto read_error = base::readFileToString(config_content, conf_file_path);
  if (read_error != base::BaseError::None) {
    return ConfigError::CONF_READ_ERROR;
  }

  // parse file
  Document jsondocu;
  if (jsondocu.Parse(config_content.c_str()).HasParseError()) {
    return ConfigError::CONF_PARSE_ERROR;
  }

  if (!jsondocu.IsObject() || !jsondocu["net"].IsArray()) {
    return ConfigError::CONF_SYNTAX_ERROR;
  }


  for (const auto& item:jsondocu["net"].GetArray()) {
    assert(item["src_ip"].IsString());
    assert(item["src_port"].IsNumber());
    assert(item["des_ip"].IsString());
    assert(item["des_port"].IsNumber());
    if (!item["src_ip"].IsString() ||!item["src_port"].IsNumber()||
        !item["des_ip"].IsString()||!item["des_port"].IsNumber()) {
      return ConfigError::CONF_SYNTAX_ERROR;
    }
    auto net_config = NetConfig{};
    net_config.src.setSockAddr(item["src_ip"].GetString(),
        static_cast<uint16_t>(item["src_port"].GetUint()));
    net_config.des.setSockAddr(item["des_ip"].GetString(),
        static_cast<uint16_t>(item["des_port"].GetUint()));
  }
  return ConfigError::NONE;
}
