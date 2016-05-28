/*******************************************************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************************************************/

#ifndef __TYH_KVDB_REDIS_DB_POOL_H__
#define __TYH_KVDB_REDIS_DB_POOL_H__

#include <string>
#include <deque>

#include "tbase/boost_common_inc.h"
#include "boost/shared_ptr.hpp"
#include "boost/thread/mutex.hpp"

#include "tbase/ttypedef.h"
#include "tbase/nocopyable.h"
#include "redis_accessor.h"



namespace tyh {


struct RedisBaseConfig {
  std::string host_;
  std::string pswd_;
  int port_;
  int database_;
};

typedef boost::shared_ptr<RedisAccessor> RedisConnPtr;

class RedisDbPool : public Nocopyable {
public:
  RedisDbPool();
  ~RedisDbPool();
  RedisConnPtr GetRedisAccessor();
  void RecycleRedisAccecor(RedisConnPtr);
  size_t pool_size();
  bool Init(size_t pool_size, std::string host, int port, \
	std::string pswd, int db_index);

  void ReleaseAll();
protected:
  RedisConnPtr CreateNewAccessor();
protected:
  std::deque<RedisConnPtr> que_conn_ptr_list_;
  boost::mutex mutex_;

  RedisBaseConfig db_base_conf_;
  size_t init_size_;
};


}  //  namespace tyh


#endif  // __TYH_KVDB_REDIS_DB_POOL_H__