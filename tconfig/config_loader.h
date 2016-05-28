/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#ifndef __TYH_CONFIG_LOADER_H__
#define __TYH_CONFIG_LOADER_H__

#include <string>

#include "tsingleton/singleton_template.h"
#include "boost/property_tree/ini_parser.hpp"

namespace tyh {

class ConfigLoader : public TSingleton<ConfigLoader> {
protected:
  ConfigLoader();
  ~ConfigLoader();
  friend class TSingleton<ConfigLoader>;

public:
  bool LoadConfig(std::string config_file);
  template <typename DataType>
  DataType GetParam(const std::string &sec_name, \
	const std::string &key_name) {
	std::string data_path = sec_name + "." + key_name;
	return pt_.get<DataType>(data_path);
  }

private:
  boost::property_tree::ptree pt_;
};

#define ConfLoaderPtr	ConfigLoader::Instance()

}	// namespace tyh



#endif	// __TYH_CONFIG_LOADER_H__