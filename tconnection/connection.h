/*
*	Copyright (c) 2016-2020 tianyiheng
*   autor : tianyiheng
*	date  : 2016-05-06
*	brief :
*/

#ifndef __TYH_T_CONNECTION_H__
#define __TYH_T_CONNECTION_H__

#include <memory>
#include <deque>

#include "tbase/boost_common_inc.h"
#include "tbase/ttypedef.h"
#include "tsocket/sock_base.h"
#include "tbase/nocopyable.h"
#include "tbase/boost_common_inc.h"
#include "tpacket/base_pack.h"
#include "tpacket/pack_owner.h"

namespace tyh {



class TConnection;

class ReadCallBackHandler : public SockAsyncCallBackHandler {
public:
  ReadCallBackHandler(TConnection * con);
  virtual ~ReadCallBackHandler();
  virtual void CallBackFun(bool err, size_t transfered_byte);

private:
  TConnection *connect_pt_;
};

class WriteCallBackHandler : public SockAsyncCallBackHandler {
public:
  WriteCallBackHandler(TConnection * con);
  ~WriteCallBackHandler();
  virtual void CallBackFun(bool err, size_t transfered_byte);

private:
  TConnection *connect_pt_;
};

//	-----------------------------------------------------------------------------------------
// TConnection
// Connection Base class

enum ConnectState {
  ECONNECT_UNINIT,
  ECONNECT_CONNECTED,
  ECONNECT_INIT,
  ECONNECT_RUN,
  ECONNECT_TIMEOUT,
  ECONNECT_CLOSE
};

class TConnection :
private Nocopyable,
public boost::enable_shared_from_this<TConnection> {
public:
  TConnection(boost::asio::io_service &io_ref);
  virtual ~TConnection();
  
public:
  // member function
  inline boost::shared_ptr<TSocket> socket_ptr() { return socket_ptr_; }

  virtual void StartRecv(size_t data_size_to_recv);
  inline boost::shared_ptr<SockAsyncCallBackHandler> read_cb() const { return read_cb_ptr_; }
  inline boost::shared_ptr<SockAsyncCallBackHandler> write_cb() const { return write_cb_ptr_; }
  virtual bool InitAsyncHandler();
  inline void set_connect_id(uint64 id) { connect_id_ = id; }
  inline uint64 connect_id() const { return connect_id_; }
  virtual bool RoundUpdate(time_t cur_time);

protected:
  boost::shared_ptr<TSocket> socket_ptr_;
  boost::shared_ptr<SockAsyncCallBackHandler>  read_cb_ptr_;
  boost::shared_ptr<SockAsyncCallBackHandler>  write_cb_ptr_;
  const int32 k_default_buffer_size_;
  
  // status handle
public:
  virtual bool HandleUnInitState(time_t cur_time);
  virtual bool HandleConnectedState(time_t cur_time);
  virtual bool HandleInitState(time_t cur_time);
  virtual bool HandleRunState(time_t cur_time);
  virtual bool HandleTimeoutState(time_t cur_time);
  virtual bool HandleCloseState(time_t cur_time);

public:
  PackBufferOwner& pack_owner() { return pack_owner_; }
  ConnectState state() { return estate_; }
  inline void set_state(ConnectState e) { estate_ = e; };
protected:
  PackBufferOwner pack_owner_;
  ConnectState estate_;

protected:
  uint64 connect_id_;
};

}  // namesapce tyh
#endif  // __TYH_T_CONNECTION_H__