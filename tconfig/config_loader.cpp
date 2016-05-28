/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#include "config_loader.h"

namespace tyh {

ConfigLoader::ConfigLoader() {

}

ConfigLoader::~ConfigLoader() {

}

bool ConfigLoader::LoadConfig(std::string config_file) {
  boost::property_tree::ini_parser::read_ini(config_file, pt_);
  return true;
}

}	// namespace tyh