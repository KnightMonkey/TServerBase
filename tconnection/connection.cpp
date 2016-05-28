/*
*	Copyright (c) 2016-2020 tianyiheng
*   autor : tianyiheng
*	date  : 2016-05-06
*	brief :
*/

#include <string>

#include "tconnection/connection.h"
#include "tsocket/sock_base.h"
#include "tsocket/sock_callback.h"
#include "tpacket/pack_owner.h"
#include "tlog/tboost_log.h"
#include "tconnection/connect_manager.h"
#include "tsynergy_worker/synergy_cell.h"
#include "tsynergy_worker/default_synergy_cell.h"
#include "tsynergy_worker/synergy_cell_manager.h"

namespace tyh {

 ReadCallBackHandler::ReadCallBackHandler(TConnection * con) {
   LOGGER(INFO) << "ReadCallBackHandler::ReadCallBackHandler()";
  connect_pt_ = con;
}

void ReadCallBackHandler::CallBackFun(bool err, size_t transfered_byte) {
  // read head
  LOGGER(INFO) << "ReadCallBackHandler::CallBackFun, err = " << (int)err << ",byte=" << transfered_byte;
  if (!err) {
	cur_data_len_ += transfered_byte;
	if (cur_data_len_ < sizeof(uint32)){
	  connect_pt_->StartRecv(sizeof(uint32) - cur_data_len_);
	}
	else {
	  // read body
	  uint32 pack_len = GetHead<uint32>();
	  if (cur_data_len_ < pack_len) {
		connect_pt_->StartRecv(pack_len - cur_data_len_);
	  }
	  else if (pack_len == cur_data_len_){
		// read finished
		PackBufferPtr pack_buffer_ptr(new PackBufferCell< NORMAL_PACK_BUFFER_SIZE >);
		pack_buffer_ptr->AppendData(data_buffer_, cur_data_len_);
		connect_pt_->pack_owner().PushPackBuffer(pack_buffer_ptr);

		// post next async read
		Clear();
		connect_pt_->StartRecv(sizeof(uint32));
	  }
	}
  }
  else {
	// remote connect closed
	connect_pt_->set_state(ECONNECT_CLOSE);
	// optimize ,append a close packet to pack owner
	// TODO:
  }
}

ReadCallBackHandler::~ReadCallBackHandler() {
  if (NULL != data_buffer_) {
	delete[] data_buffer_, data_buffer_ = NULL;
	buffer_len_ = 0;
	cur_data_len_ = 0;
	LOGGER(INFO) << "ReadCallBackHandler::~ReadCallBackHandler()";
  }
}

WriteCallBackHandler::WriteCallBackHandler(TConnection * con) {
  connect_pt_ = con;
}

void WriteCallBackHandler::CallBackFun(bool err, size_t transfered_byte) {

}

WriteCallBackHandler::~WriteCallBackHandler() {
  if (NULL != data_buffer_) {
	delete[] data_buffer_, data_buffer_ = NULL;
	buffer_len_ = 0;
	cur_data_len_ = 0;
  }
}
//////////////////////////////////////////////////////////////////////////
TConnection::TConnection(boost::asio::io_service & io_ref) :
k_default_buffer_size_(1024),
connect_id_(0)
{
  socket_ptr_.reset(new TSocket(io_ref));
  estate_ = ECONNECT_UNINIT;
  //LOGGER(INFO) << "TConnection::TConnection()";
}

TConnection::~TConnection() {
  if (socket_ptr_.unique() && socket_ptr_->IsOpen()) {
	socket_ptr_->ShutDownAll();
	socket_ptr_->Close();
  }
  LOGGER(DEBUG) << "TConnection::~TConnection(),id = " << connect_id_;
}

void TConnection::StartRecv(size_t data_size_to_recv) {
  //InitAsyncHandler();
  LOGGER(INFO) << "TConnection::StartRecv" << "data_size_to_recv:" << data_size_to_recv;
  socket_ptr_->AsyncRecv(read_cb_ptr_->buffer(), \
	data_size_to_recv, read_cb_ptr_.get());
}

bool TConnection::InitAsyncHandler() {
  read_cb_ptr_.reset(new ReadCallBackHandler(this));
  read_cb_ptr_->SetBufferSize(k_default_buffer_size_);

  write_cb_ptr_.reset(new WriteCallBackHandler(this));
  write_cb_ptr_->SetBufferSize(k_default_buffer_size_);
  return true;
}

bool TConnection::RoundUpdate(time_t cur_time) {
  //LOGGER(INFO) << "TConnection::RoundUpdate()";
  switch (estate_) {
	case ECONNECT_UNINIT:{
	  if (HandleUnInitState(cur_time)) {
		set_state(ECONNECT_CONNECTED);
	  }
	  break;
	}
	case ECONNECT_CONNECTED:{
	  if (HandleConnectedState(cur_time)) {
		set_state(ECONNECT_INIT);
	  }
	  break;
	}
	case ECONNECT_INIT:{
	  if (HandleInitState(cur_time)) {
		set_state(ECONNECT_RUN);
	  }
	  break;
	}
	case ECONNECT_RUN:{
	  /*if (!HandleRunState(cur_time)) {
		set_state(ECONNECT_CLOSE);
	  }*/
	  break;
	}
	case ECONNECT_TIMEOUT:{
	  if (HandleTimeoutState(cur_time)) {
		set_state(ECONNECT_CLOSE);
	  }
	  break;
	}
	case ECONNECT_CLOSE:{
	  HandleCloseState(cur_time);
	  break;
	}
	default: {
	  break;
	}
  }
  return true;
}

// status handle
bool TConnection::HandleUnInitState(time_t cur_time) {
  LOGGER(INFO) << "TConnection::HandleUnInitState : " << cur_time;
  return true;
}

bool TConnection::HandleConnectedState(time_t cur_time) {
  LOGGER(INFO) << "TConnection::HandleConnectedState : " << cur_time;
  return true;
}

bool TConnection::HandleInitState(time_t cur_time) {
  /*
  if (SynergyCellManager::Instance()->HasCell(connect_id_)) {
	SynergyCellManager::Instance()->RemoveCell(connect_id_);
  }
  */
  LOGGER(INFO) << "TConnection::HandleInitState, id = " << connect_id_;
 
  SynergyCellPtr cell_ptr(new DefaultSynergyCell(connect_id_, shared_from_this()));
  SynergyCellManager::Instance()->AddSynergyCell(connect_id_, cell_ptr);

  return true;
}

bool TConnection::HandleRunState(time_t cur_time) {
  PackBufferPtr pack_buffer_ptr;
  while (pack_owner_.PopPackBuffer(pack_buffer_ptr)) {
	LOGGER(INFO) << "TConnection::HandleRunState : " << cur_time;
	std::string pack_content(pack_buffer_ptr->buffer(), pack_buffer_ptr->length());
	size_t send_len = socket_ptr_->SendData(pack_content.c_str(), pack_content.length());
	if (0 == send_len) {
	  // the remote sock has closed
	  // TODO:
	}
	LOGGER(INFO) << "SendData:" << (pack_content.c_str()+4);
  }
  return true;
}

bool TConnection::HandleTimeoutState(time_t cur_time) {
  LOGGER(INFO) << "HandleTimeoutState : " << cur_time;
  return estate_ == ECONNECT_TIMEOUT;
}

bool TConnection::HandleCloseState(time_t cur_time) {
  LOGGER(INFO) << "TConnection::HandleCloseState : " << cur_time;
  socket_ptr_->ShutDownAll();
  socket_ptr_->Close();
  ConnMgrPtr->AddRemove(connect_id_);
  return true;
}

} // namespace tyh