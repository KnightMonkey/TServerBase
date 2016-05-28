/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#ifndef __TYH_SERVER_BASE_MY_SERVER_H__
#define __TYH_SERVER_BASE_MY_SERVER_H__

#include "tbase_server/base_server.h"

class MyServer : public tyh::TBaseServer {
public:
  explicit MyServer(size_t pool_size/*, std::string host, uint32 port*/);
  ~MyServer();

protected:
  virtual bool LoadConfig();
  virtual bool InitDatabase();

  virtual ConnectionPtr GetNewConnection();
  virtual void MainRun(time_t cur_time);

  virtual void InitDefaultName(){ server_name_ = "my_server"; }
};

#endif // __TYH_SERVER_BASE_MY_SERVER_H__