/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#include "default_synergy_cell.h"
#include "ttimetool/time_tool.h"
#include "tlog/tboost_log.h"

namespace tyh {

DefaultSynergyCell::DefaultSynergyCell(uint32 cell_id, boost::shared_ptr<TConnection> conn) :
SynergyCell(cell_id), 
connection_ptr_(conn)
{
  LOGGER(INFO) << "DefaultSynergyCell::DefaultSynergyCell, con_addr=" << (int)conn.get();

}

DefaultSynergyCell::~DefaultSynergyCell() {

}

bool DefaultSynergyCell::Run() {
  if (connection_ptr_.use_count() > 0 && \
	connection_ptr_->state() == ECONNECT_RUN) {
	time_t now_time = TChronoTime::NowTime();
	connection_ptr_->HandleRunState(now_time);
  }
  else {
	// release connect
	//connection_ptr_.reset();
	valid_ = false;
  }
  return true;
}

bool DefaultSynergyCell::Run(time_t time_sec) {
  if (connection_ptr_.use_count() > 0 && \
	connection_ptr_->state() == ECONNECT_RUN) {
	connection_ptr_->HandleRunState(time_sec);
  }
  else {
	// release connect
	//connection_ptr_.reset();
	valid_ = false;
  }
  return true;
}

}  // namespace tyh