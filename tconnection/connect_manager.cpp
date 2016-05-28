/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : 2016-05-10
 * brief  :
 *******************************************/
#include <algorithm>
#include <utility>

#include "tconnection/connect_manager.h"
#include "tlog/tboost_log.h"

namespace tyh {

void DefaultConnectEach::operator()(boost::shared_ptr<TConnection> conn_ptr) {
  conn_ptr->RoundUpdate(cur_time_);
}

// ----------------------------------------------------------------------------
ConnectMgr::ConnectMgr() {
  id_index_ = 1;
}

void ConnectMgr::AddCacheConnnection(boost::shared_ptr<TConnection> conn_ptr) {
  boost::unique_lock<boost::mutex> scoped_lock(mutex_);
  vec_temp_conn_list_.push_back(conn_ptr);
  LOGGER(INFO) << "AddCacheConnnection ...";
}

void ConnectMgr::UpdateCacheConnect(time_t cur_time) {
  boost::unique_lock<boost::mutex> scoped_lock(mutex_);
  for (std::vector< boost::shared_ptr<TConnection> >::iterator it = vec_temp_conn_list_.begin();
	it != vec_temp_conn_list_.end(); ++it) {
	AddConnection(*it);
  }
  vec_temp_conn_list_.clear();
}

void ConnectMgr::AddConnection(boost::shared_ptr<TConnection> conn_ptr) {
  uint64 unique_id = GetUniqueConnectID();
  conn_ptr->set_connect_id(unique_id);
  map_connect_.insert(std::make_pair(unique_id, conn_ptr));
  LOGGER(DEBUG) << "AddConnection, id = " << unique_id << ",state=" << (int)conn_ptr->state();
}

void ConnectMgr::RemoveConnection(uint64 connect_id) {
  std::map<uint64, boost::shared_ptr<TConnection>>::iterator it;
  it = map_connect_.find(connect_id);
  if (it != map_connect_.end()) {
	uint64 unique_id = it->first;
	map_connect_.erase(it);
	used_id_list_.push_back(unique_id);
	LOGGER(DEBUG) << "ConnectMgr::RemoveConnection,id = " << unique_id;
  }
}

uint64 ConnectMgr::GetUniqueConnectID() {
  uint64 unique_id = 0;
  if (used_id_list_.empty()) {
	unique_id = id_index_++;
  }
  else {
	unique_id = used_id_list_.front();
	std::remove(used_id_list_.begin(), used_id_list_.end(), unique_id);
  }
  //unique_id = id_index_++;
  return unique_id;
}

void ConnectMgr::AddRemove(uint64 connect_id) {
  vec_id_to_remove_.push_back(connect_id);
  LOGGER(INFO) << "ConnectMgr::AddRemove";
}

void ConnectMgr::UpdateRemove(time_t cur_time) {
  for (std::vector<uint64>::iterator it = vec_id_to_remove_.begin();
	it != vec_id_to_remove_.end(); ++it) {
	RemoveConnection(*it);
	LOGGER(INFO) << "ConnectMgr::UpdateRemove, id = " << *it;
  }
  vec_id_to_remove_.clear();
}

// Ò»ÂÖ¸üÐÂ
bool ConnectMgr::RoundUpdate(time_t cur_time) {
  UpdateCacheConnect(cur_time);
  DefaultConnectEach conn_each_call(cur_time);
  ConnectMgr::Instance()->ForEach(conn_each_call);
  UpdateRemove(cur_time);
  return true;
}

}  // namespace tyh
