/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : 2016-05-10
 * brief  :
 *******************************************/

#include "tbase_server/base_server.h"
#include "tlog/tboost_log.h"
#include "tbase/boost_common_inc.h"
#include "tsynergy_worker/synergy_cell.h"
#include "tsynergy_worker/default_synergy_cell.h"
#include "tsynergy_worker/synergy_cell_manager.h"
#include "tconfig/config_manager.h"

namespace tyh {

TBaseServer::TBaseServer(size_t service_size)
: io_service_pool_(service_size),
acceptor_(io_service_pool_.GetIoService()),
signal_set_(io_service_pool_.GetIoService()),
server_state_(ESERVER_INIT)
{
}

ConnectionPtr TBaseServer::GetNewConnection() {
  ConnectionPtr connect_ptr(new TConnection(io_service_pool_.GetIoService()));
  return connect_ptr;
}

void TBaseServer::Listen(std::string host, int port) {
  BoostTcp::resolver addr_resolver(acceptor_.get_io_service());
  BoostTcp::resolver::query resolve_query(host, boost::lexical_cast<std::string>(port));
  BoostTcp::endpoint addr_endpt = *addr_resolver.resolve(resolve_query);
  acceptor_.open(addr_endpt.protocol());
  acceptor_.set_option(BoostTcp::acceptor::reuse_address(true));
  acceptor_.bind(addr_endpt);
  acceptor_.listen();
}

bool TBaseServer::StartAccept() {
  //ConnectionPtr connect_ptr(new TConnection(io_service_pool_.GetIoService()));
  ConnectionPtr connect_ptr = GetNewConnection();
  acceptor_.async_accept(connect_ptr->socket_ptr()->socket(), \
	boost::bind(&TBaseServer::HandleAccept, \
	this, boost::asio::placeholders::error, connect_ptr)
	);
  return true;
}

bool TBaseServer::HandleAccept(const boost::system::error_code& err, \
  ConnectionPtr connect_ptr) {
  if (!err) {
	connect_ptr->InitAsyncHandler();
	connect_ptr->set_state(ECONNECT_CONNECTED);
	ConnectMgr::Instance()->AddCacheConnnection(connect_ptr);
	connect_ptr->StartRecv(sizeof(uint32));
  }
  StartAccept();
  return true;
}

bool TBaseServer::LoadConfig() {
  server_conf_.host = ConfMgr->server_base_config().ip;
  server_conf_.port = ConfMgr->server_base_config().port;
  server_conf_.synergy_worker_num = ConfMgr->server_base_config().synergy_worker_num;
  server_conf_.main_round_sleep_ms = ConfMgr->server_base_config().main_round_sleep_ms;
  LOGGER(INFO) << "TBaseServer::LoadConfig()";
  return true;
}

bool TBaseServer::Init() {
  InitDefaultName();
  TBoostLogger::InitLogger(server_name_);
  LoadConfig();
  InitDatabase();
  synergy_worker_ptr_.reset(new SynergyWorker(server_conf_.synergy_worker_num));
  LOGGER(INFO) << "TBaseServer::Init()";
  return true;
}

bool TBaseServer::InitDatabase() {
  return true;
}

void TBaseServer::HandleSignal() {
  LOGGER(DEBUG) << "====<HandleSignal()>, server will be stop ...";
  server_state_ = ESERVER_STOP;
}

void TBaseServer::Run() {
  if (Init()) {
	signal_set_.add(SIGINT);
	signal_set_.add(SIGTERM);
#if defined(SIGQUIT)
	signal_set_.add(SIGQUIT);	  // ¼üÅÌ Ctrl+4
#endif // defined(SIGQUIT)
	signal_set_.async_wait(boost::bind(&TBaseServer::HandleSignal, this));

	Listen(server_conf_.host, server_conf_.port);

	LOGGER(INFO) << "Server running, host = " << server_conf_.host << ", port = " << server_conf_.port;
	StartAccept();
	SetState(ESERVER_RUN);
	io_service_pool_.Run();
	OnLoopBefore();
	MainLoop();
	OnLoopEnd();
	io_service_pool_.Stop();
	LOGGER(DEBUG) << "[server stopped, the program is exiting ....] ";
  }
}

void TBaseServer::MainLoop() {
  while (ESERVER_STOP != server_state_) {
	time_t now_time = time(NULL);
	MainRun(now_time);
	boost::this_thread::sleep_for(\
	  boost::chrono::milliseconds(server_conf_.main_round_sleep_ms));
  }
  if (0 < synergy_worker_ptr_->worker_size()) {
	synergy_worker_ptr_->Stop();
	LOGGER(DEBUG) << "[synergy_worker stopped] ";
  }
}

void TBaseServer::MainRun(time_t cur_time) {
  ConnMgrPtr->RoundUpdate(cur_time);
  std::vector<boost::shared_ptr<SynergyCell> > vec_cell;
  SynergyCellManager::Instance()->GetAllSynergyCell(&vec_cell);
  if (0 < synergy_worker_ptr_->worker_size()) {
	synergy_worker_ptr_->set_time_sec(cur_time);
	for (std::vector<boost::shared_ptr<SynergyCell> >::iterator it = vec_cell.begin(); \
	  it != vec_cell.end(); ++it) {
	  synergy_worker_ptr_->AddSynergyCell(*it);
	}
	synergy_worker_ptr_->WaitFinish();
	//LOGGER(INFO) << "-->synergy_worker_ptr_->WaitFinish() ";
  }
  else {
	for (std::vector<boost::shared_ptr<SynergyCell> >::iterator it = vec_cell.begin(); \
	  it != vec_cell.end(); ++it) {
	  (*it)->Run();
	}
	LOGGER(INFO) << "--> [handle in main thread]";
  }
  vec_cell.clear();
  // debug
  SynergyCellManager::Instance()->RoundRemove();
  LOGGER(DEBUG) << "--> [connection size:]" << ConnMgrPtr->Size();
  LOGGER(DEBUG) << "--> [synergycell size:]" << SynergyCellManager::Instance()->Size();
}

void TBaseServer::OnLoopBefore() {

}


void TBaseServer::OnLoopEnd() {

}

}  // namespace tyh
