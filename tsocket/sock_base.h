/*
*	Copyright(c) 2016 - 2020 tianyiheng
*	autor : tianyiheng
*	date : 2016 - 05 - 05
*	brief :
*/

#ifndef __TYH_BOOST_SOCK_BASE_H__
#define __TYH_BOOST_SOCK_BASE_H__

#include "tbase/ttypedef.h"
#include "tsocket/boost_sock_inc.h"
#include "tsocket/tsock_template.h"
#include "tsocket/sock_callback.h"

namespace boost_ip = boost::asio::ip;
typedef	  boost_ip::tcp	BoostTcp;

namespace tyh {
 
class TSocket : public TSockBase<BoostTcp::socket> {
public:
  TSocket(boost::asio::io_service & io_srv_ref);
  ~TSocket();

public:
  virtual bool Connect(std::string host, uint32 port);
  virtual void KeepAlive(bool alive = false);
  virtual void Noblocking(bool block = false);
  virtual void SetSendBufferSize(uint32 buf_size);
  virtual void SetRecvBufferSize(uint32 buf_size);
  virtual void ShutDownRecv();
  virtual void ShutDownSend();
  virtual void ShutDownAll();
  virtual void Close();
  virtual bool IsOpen();
  BoostTcp::socket& socket() { return *sock_ptr_; };

public:
  size_t SendData(const char *data, size_t data_length);
  size_t RecvData(char* buffer, size_t buffer_len);
  bool AsyncRecv(char *buffer, size_t buffer_len, \
	SockAsyncCallBackHandler * cb_handler);
  bool AsyncSend(char *buffer, size_t buffer_len, \
	SockAsyncCallBackHandler * cb_handler);

protected:
  void ReadCallBack(const boost::system::error_code& error, \
	size_t bytes_transferred, \
	SockAsyncCallBackHandler * cb_call);
  void WriteCallBack(const boost::system::error_code& error, \
	size_t bytes_transferred, \
	SockAsyncCallBackHandler * cb_call);
};

}  // namespace tyh 

#endif  // __TYH_BOOST_SOCK_BASE_H__