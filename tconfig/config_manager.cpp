/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#include "tconfig/config_loader.h"
#include "tconfig/config_manager.h"

namespace tyh {

ConfigManager::ConfigManager() {

}

ConfigManager::~ConfigManager() {

}

bool ConfigManager::Init(std::string config_file) {
  ConfLoaderPtr->LoadConfig(config_file);

  LoadServerConfig();
  LoadMySqlConfig();
  LoadRedisConfig();
  return true;
}

bool ConfigManager::LoadServerConfig() {
  server_base_config_.ip = ConfLoaderPtr->GetParam<std::string>("server", "ip");
  server_base_config_.port = ConfLoaderPtr->GetParam<int>("server", "port");
  server_base_config_.io_service_size = ConfLoaderPtr->GetParam<int>("server", "io_service_size");
  server_base_config_.synergy_worker_num = ConfLoaderPtr->GetParam<int>("server", "synergy_worker_num");
  server_base_config_.main_round_sleep_ms = ConfLoaderPtr->GetParam<int>("server", "main_round_sleep_ms");
  return true;
}

bool ConfigManager::LoadMySqlConfig() {
  mysql_base_config_.host = ConfLoaderPtr->GetParam<std::string>("mysql", "host");
  mysql_base_config_.user = ConfLoaderPtr->GetParam<std::string>("mysql", "user");
  mysql_base_config_.pswd = ConfLoaderPtr->GetParam<std::string>("mysql", "pswd");
  mysql_base_config_.database = ConfLoaderPtr->GetParam<std::string>("mysql", "db");
  mysql_base_config_.encoding = ConfLoaderPtr->GetParam<std::string>("mysql", "encoding");
  mysql_base_config_.pool_size = ConfLoaderPtr->GetParam<size_t>("mysql", "poolsize");
  return true;
}

bool ConfigManager::LoadRedisConfig() {
  redis_pool_base_config_.host_ = ConfLoaderPtr->GetParam<std::string>("redis", "host");
  redis_pool_base_config_.pswd_ = ConfLoaderPtr->GetParam<std::string>("redis", "pswd");
  redis_pool_base_config_.port_ = ConfLoaderPtr->GetParam<int>("redis", "port");
  redis_pool_base_config_.db_ = ConfLoaderPtr->GetParam<int>("redis", "db");
  redis_pool_base_config_.pool_size_ = ConfLoaderPtr->GetParam<size_t>("redis", "poolsize");
  return true;
}

}  // namespace tyh