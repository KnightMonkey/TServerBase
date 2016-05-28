/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#include <iostream>
#include <sstream>
#include <string>

#include "mysql_db_pool.h"
#include "tlog/tboost_log.h"

namespace tyh {

MySqlDbPool::MySqlDbPool() {
  init_size_ = 1;
}

MySqlDbPool::~MySqlDbPool() {

}

bool MySqlDbPool::Init(size_t pool_size, std::string host, std::string user, \
  std::string pswd, std::string db_name, std::string encoding_set) {

  db_config_.database_ = db_name;
  db_config_.host_ = host;
  db_config_.pswd_ = pswd;
  db_config_.user_ = user;
  init_size_ = pool_size;
  db_config_.encoding_set_ = encoding_set;

  std::stringstream sstrm;
  sstrm << "host=" << host << " db=" << db_name \
	<< " user=" << user << " password='" << pswd << "'";
  bool ret = true;
  std::string conn_str = sstrm.str();
  boost::unique_lock<boost::mutex> scoped_lock(mutex_);
  for (size_t i = 0; i < init_size_; ++i) {
	MySqlConnPtr conn_sql_ptr = CreateNewAccessor();
	if (!conn_sql_ptr->Init(host, user, pswd, db_name, encoding_set)) {
	  ret = false;
	  break;
	}
	else {
	  que_conn_ptr_.push_back(conn_sql_ptr);
	}
  }

  return ret;
}

MySqlConnPtr MySqlDbPool::CreateNewAccessor() {
  MySqlConnPtr tmp(new MySqlAccessor);
  return tmp;
}

MySqlConnPtr MySqlDbPool::GetMySqlAccessor() {
  MySqlConnPtr tmp;
  {
	boost::unique_lock<boost::mutex> scoped_lock(mutex_);
	if (que_conn_ptr_.size() > 0) {
	  tmp = que_conn_ptr_.front();
	  que_conn_ptr_.pop_front();
	}
	else {
	  tmp = CreateNewAccessor();
	  tmp->Init(db_config_.host_, db_config_.user_, \
		db_config_.pswd_, db_config_.database_, db_config_.encoding_set_);
	  //que_conn_ptr_.push_back(tmp);
	}
  }
  return tmp;
}

void MySqlDbPool::RecycleSqlAccessor(MySqlConnPtr sql_ptr) {
  boost::unique_lock<boost::mutex> scoped_lock(mutex_);
  que_conn_ptr_.push_back(sql_ptr);
}

size_t MySqlDbPool::pool_size() {
  boost::unique_lock<boost::mutex> scoped_lock(mutex_);
  return que_conn_ptr_.size();
}

void MySqlDbPool::ReleaseAll() {
  boost::unique_lock<boost::mutex> scoped_lock(mutex_);
  std::deque<MySqlConnPtr>::iterator it = que_conn_ptr_.begin();
  for (; it != que_conn_ptr_.end(); ++it) {
	(*it)->Close();
  }
  que_conn_ptr_.clear();
}

}	// namespace tyh