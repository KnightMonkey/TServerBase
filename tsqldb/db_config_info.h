/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#ifndef __TYH_TSQLDB_CONFIG_INFO_H__
#define __TYH_TSQLDB_CONFIG_INFO_H__

#include <string>

struct DbSqlConfig{
  std::string host_;
  std::string user_;
  std::string pswd_;
  std::string database_;
  std::string encoding_set_;
};



#endif  // __TYH_TSQLDB_CONFIG_INFO_H__