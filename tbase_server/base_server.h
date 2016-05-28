/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : 2015-05-10
 * brief  :
 *******************************************/

#ifndef __TYH_BASE_SERVER_H__
#define __TYH_BASE_SERVER_H__

#include <string>


#include "tbase/ttypedef.h"
#include "tbase/boost_common_inc.h"
#include "tservice_pool/io_service_pool.h"
#include "tbase/nocopyable.h"
#include "tsocket/sock_base.h"
#include "tconnection/connection.h"
#include "tconnection/connect_manager.h"
#include "tlog/tboost_log.h"
#include "tsynergy_worker/synergy_worker.h"

typedef boost::shared_ptr<tyh::TConnection> ConnectionPtr;

namespace tyh {

enum ServerState {
  ESERVER_INIT,
  ESERVER_RUN,
  ESERVER_STOP
};

struct ServerInfo {
  std::string host;
  uint32 port;
  size_t synergy_worker_num;
  int main_round_sleep_ms;
};

class TBaseServer : protected Nocopyable {
public:
  explicit TBaseServer(size_t service_size);

   void Run();
   std::string server_name() { return server_name_; }
   void set_server_name(std::string name) { server_name_ = name; }
   

protected:
  virtual bool Init();
  virtual bool LoadConfig();
  virtual bool InitDatabase();

  void Listen(std::string host, int port);
  bool StartAccept();
  virtual bool HandleAccept(const boost::system::error_code& err, \
	ConnectionPtr connect_ptr);
  virtual void OnLoopBefore();
  void MainLoop();
  virtual void OnLoopEnd();

  virtual void MainRun(time_t cur_time);
  virtual ConnectionPtr GetNewConnection();

  virtual void InitDefaultName() = 0;

protected:
  inline void SetState(ServerState estate) { server_state_ = estate; }
  void HandleSignal();

protected:
  IOServicePool io_service_pool_;
  BoostTcp::acceptor acceptor_;
  ServerState server_state_;

  ServerInfo server_conf_;
  boost::shared_ptr<SynergyWorker> synergy_worker_ptr_;
  boost::asio::signal_set signal_set_;

  std::string server_name_;
};

} // namespace tyh
#endif	// __TYH_BASE_SERVER_H__