/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : 2016-05-10
 * brief  :
 *******************************************/

#ifndef __TYH_CONNECT_MANAGER_H__
#define __TYH_CONNECT_MANAGER_H__

#include <vector>
#include <map>
#include <list>
#include <functional>

#include "tbase/ttypedef.h"
#include "tbase/boost_common_inc.h"
#include "tsingleton/singleton_template.h"
#include "tconnection/connection.h"
#include "boost/thread/mutex.hpp"

namespace tyh {

class DefaultConnectEach {
public:
  DefaultConnectEach(time_t cur_time){ cur_time_ = cur_time; }
  ~DefaultConnectEach() {}
  void operator ()(boost::shared_ptr<TConnection> conn_ptr);
  time_t cur_time_;
};


class ConnectMgr : public TSingleton<ConnectMgr> {
protected:
  ConnectMgr();
  ~ConnectMgr() {}
  friend class TSingleton<ConnectMgr>;

public:
  void AddConnection(boost::shared_ptr<TConnection> conn_ptr);
  void AddCacheConnnection(boost::shared_ptr<TConnection> conn_ptr);
  void RemoveConnection(uint64 connect_id);
  void AddRemove(uint64 connect_id);
  size_t Size() { return map_connect_.size(); }
  template<class T = DefaultConnectEach>
  void ForEach(T each_call) {	
	typedef std::map<uint64, boost::shared_ptr<TConnection> >::iterator ConnectIterator;
	for (ConnectIterator it = map_connect_.begin(); it != map_connect_.end(); ++it) {
	  each_call(it->second);
	}
  }
  // Ò»ÂÖ¸üÐÂ
  virtual bool RoundUpdate(time_t cur_time);
  void UpdateRemove(time_t cur_time);
  void UpdateCacheConnect(time_t cur_time);

protected:
  uint64 GetUniqueConnectID();

private:
  std::map<uint64, boost::shared_ptr<TConnection>> map_connect_;
  
  uint64 id_index_;
  std::list<uint64>	used_id_list_;
  std::vector<uint64> vec_id_to_remove_;

  std::vector< boost::shared_ptr<TConnection> > vec_temp_conn_list_;
  boost::mutex mutex_;
};

#define ConnMgrPtr	ConnectMgr::Instance()

}  // namespace tyh
#endif  // __TYH_CONNECT_MANAGER_H__