/*******************************************************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************************************************/

#include "tkvdb/redis_db_pool.h"

namespace tyh {

RedisDbPool::RedisDbPool() {
  init_size_ = 1;
}

RedisDbPool::~RedisDbPool() {

}

RedisConnPtr RedisDbPool::GetRedisAccessor() {
  RedisConnPtr tmp;
  {
	boost::unique_lock<boost::mutex> scoped_lock(mutex_);
	if (que_conn_ptr_list_.size() > 0) {
	  tmp = que_conn_ptr_list_.front();
	  que_conn_ptr_list_.pop_front();
	}
	else {
	  tmp = CreateNewAccessor();
	  tmp->ConnectServer(db_base_conf_.host_, db_base_conf_.port_, db_base_conf_.pswd_);
	  tmp->SelectDatabase(db_base_conf_.database_);
	}
  }
  return tmp;
}

void RedisDbPool::RecycleRedisAccecor(RedisConnPtr) {

}

RedisConnPtr RedisDbPool::CreateNewAccessor() {
  RedisConnPtr tmp(new RedisAccessor);
  return tmp;
}

size_t RedisDbPool::pool_size() {
  boost::unique_lock<boost::mutex> scoped_lock(mutex_);
  return que_conn_ptr_list_.size();
}

bool RedisDbPool::Init(size_t pool_size, std::string host, \
  int port, std::string pswd, int db_index) {
  bool ret = true;

  db_base_conf_.host_ = host;
  db_base_conf_.port_ = port;
  db_base_conf_.pswd_ = pswd;
  db_base_conf_.database_ = db_index;
  init_size_ = pool_size;

  boost::unique_lock<boost::mutex> scoped_lock(mutex_);
  for (size_t i = 0; i < pool_size; ++i) {
	RedisConnPtr tmp = CreateNewAccessor();
	if (tmp->ConnectServer(host, port, pswd)) {
	  tmp->SelectDatabase(db_index);
	  que_conn_ptr_list_.push_back(tmp);
	}
	else {
	  ret = false;
	  break;
	}
  }

  return ret;
}

void RedisDbPool::ReleaseAll() {
  boost::unique_lock<boost::mutex> scoped_lock(mutex_);
  std::deque<RedisConnPtr>::iterator it = que_conn_ptr_list_.begin();
  for (; it != que_conn_ptr_list_.end(); ++it) {
	(*it)->Close();
  }
  que_conn_ptr_list_.clear();
}

}  // namespace tyh