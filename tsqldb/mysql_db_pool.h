/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#ifndef __TYH_TSQLDB_MYSQL_DB_POOL_H__
#define __TYH_TSQLDB_MYSQL_DB_POOL_H__

#include <string>
#include <deque>

#include "tbase/boost_common_inc.h"
#include "boost/shared_ptr.hpp"
#include "boost/thread/mutex.hpp"

#include "soci.h"
#include "soci-mysql.h"

#include "tbase/ttypedef.h"
#include "tbase/nocopyable.h"
#include "./db_config_info.h"
#include "./mysql_accessor.h"

namespace tyh {

typedef boost::shared_ptr<MySqlAccessor> MySqlConnPtr;

class MySqlDbPool : public Nocopyable {
public:
  MySqlDbPool();
  ~MySqlDbPool();

  bool Init(size_t pool_size, std::string host, std::string user, std::string pswd, \
	std::string db_name, std::string encoding_set);

  size_t pool_size();
  inline const DbSqlConfig& db_config() const { return db_config_; }
  MySqlConnPtr GetMySqlAccessor();
  void RecycleSqlAccessor(MySqlConnPtr sql_ptr);

  void ReleaseAll();
protected:
  virtual MySqlConnPtr CreateNewAccessor();

protected:
  size_t init_size_;
  DbSqlConfig db_config_;

  std::deque<MySqlConnPtr> que_conn_ptr_;
  boost::mutex mutex_;
};


}	// namespace tyh






#endif  // __TYH_TSQLDB_MYSQL_DB_POOL_H__