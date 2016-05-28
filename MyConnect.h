/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : 2016-05-11
 * brief  :
 *******************************************/

#include "tsocket/sock_base.h"
#include "tlog/tboost_log.h"
#include "tpacket/base_pack.h"
#include "tconnection/connection.h"
#include "tsingleton/singleton_template.h"
#include "tbase_server/base_server.h"

#ifndef __TYH_SERVER_BASE_MY_CONNECT_H__
#define __TYH_SERVER_BASE_MY_CONNECT_H__

class MyConnection : public tyh::TConnection {
public:
  MyConnection(boost::asio::io_service &io_ref);
  ~MyConnection();

public:
  virtual bool HandleRunState(time_t cur_time);
  virtual bool HandleInitState(time_t cur_time);
};



#endif	// __TYH_SERVER_BASE_MY_CONNECT_H__