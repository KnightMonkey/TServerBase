/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/


#ifndef __TYH_CONFIG_MANAGER_H__
#define __TYH_CONFIG_MANAGER_H__

#include "tsingleton/singleton_template.h"
#include <string>

namespace tyh {

// server basic config
struct ServerBaseConifg {
  std::string ip;
  int port;
  size_t io_service_size;
  size_t synergy_worker_num;
  int main_round_sleep_ms;
};

// database basic config
struct MysqlBaseConfig {
  std::string host;
  std::string user;
  std::string pswd;
  std::string database;
  std::string encoding;
  size_t pool_size;
};

struct RedisPoolConfig {
  RedisPoolConfig() {
	db_ = 0;
  }
  std::string host_;
  std::string pswd_;
  int port_;
  int db_;
  size_t pool_size_;
};

class ConfigManager : public TSingleton<ConfigManager> {
protected:
  ConfigManager();
  ~ConfigManager();
  friend class TSingleton<ConfigManager>;
public:
  bool Init(std::string config_file);

  bool LoadServerConfig();
  bool LoadMySqlConfig();
  bool LoadRedisConfig();

  const ServerBaseConifg& server_base_config() const { 
	return server_base_config_; 
  }

  const MysqlBaseConfig& mysql_base_config() const {
	return mysql_base_config_;
  }

  const RedisPoolConfig& redis_pool_config() const {
	return redis_pool_base_config_;
  }
private:
  ServerBaseConifg server_base_config_;
  MysqlBaseConfig mysql_base_config_;
  RedisPoolConfig redis_pool_base_config_;
};

#define ConfMgr ConfigManager::Instance()
}	// namespace tyh


#endif  // __TYH_CONFIG_MANAGER_H__