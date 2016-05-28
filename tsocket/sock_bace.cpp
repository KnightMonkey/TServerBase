/*
*	Copyright(c) 2016 - 2020 tianyiheng
*	autor : tianyiheng
*	date : 2016 - 05 - 05
*	brief :
*/

#include <cassert>
#include "tsocket/sock_base.h"
#include "tlog/tboost_log.h"


namespace tyh {

TSocket::TSocket(boost::asio::io_service & io_srv_ref) {
  sock_ptr_ = new BoostTcp::socket(io_srv_ref);
}

TSocket::~TSocket() {
  if (NULL == sock_ptr_) {
	delete sock_ptr_, sock_ptr_ = NULL;
  }
}

bool TSocket::Connect(std::string host, uint32 port) {
  BoostTcp::endpoint pt(boost_ip::address::from_string(host), port);
  boost::system::error_code ec;
  sock_ptr_->connect(pt, ec);
  if (ec) {
	// failt to connect
  }
  return !ec;
}

void TSocket::KeepAlive(bool alive) {
  boost::asio::socket_base::keep_alive option(alive);
  sock_ptr_->set_option(option);
}

void TSocket::Noblocking(bool block) {
  boost::asio::socket_base::non_blocking_io command(block);
  sock_ptr_->io_control(command);
}

void TSocket::SetSendBufferSize(uint32 buf_size) {
  boost::asio::socket_base::send_buffer_size option(buf_size);
  sock_ptr_->set_option(option);
}

void TSocket::SetRecvBufferSize(uint32 buf_size) {
  boost::asio::socket_base::receive_buffer_size option(buf_size);
  sock_ptr_->set_option(option);
}

void TSocket::ShutDownRecv() {
  sock_ptr_->shutdown(BoostTcp::socket::shutdown_receive);
}

void TSocket::ShutDownSend() {
  sock_ptr_->shutdown(BoostTcp::socket::shutdown_send);
}

void TSocket::ShutDownAll() {
  sock_ptr_->shutdown(BoostTcp::socket::shutdown_both);
}

void TSocket::Close() {
  if (NULL != sock_ptr_ && sock_ptr_->is_open()) {
	sock_ptr_->close();
  }
}

bool TSocket::IsOpen() {
  return sock_ptr_->is_open();
}

size_t TSocket::SendData(const char *data, size_t data_length) {
  std::size_t sended_len = 0;
  try {
	sended_len = \
	  sock_ptr_->send(boost::asio::buffer(data, data_length));
  }
  catch (boost::system::system_error err){
	LOGGER(INFO) << "TSocket::SendData:error info:" << err.what();
  }
  return sended_len;
}

size_t TSocket::RecvData(char* buffer, size_t buffer_len) {
  std::size_t recv_len = 0;
  try {
	recv_len = \
	  sock_ptr_->receive(boost::asio::buffer(buffer, buffer_len));
  }
  catch (boost::system::system_error err) {
	// error ,indicate the socket has closed
	LOGGER(INFO) << "TSocket::RecvData:error info:" << err.what();
  }
  return recv_len;
}

bool TSocket::AsyncRecv(char *buffer, size_t buffer_len, \
  SockAsyncCallBackHandler * cb_handler) {
  assert(NULL != cb_handler);
  //LOGGER(INFO) << "TSocket::AsyncRecv,buffer = " << (int)cb_handler->buffer() << ", len = " << buffer_len;
  sock_ptr_->async_receive(
	boost::asio::buffer(cb_handler->buffer(), buffer_len),
	boost::bind(&TSocket::ReadCallBack, this, \
	  boost::asio::placeholders::error, \
	  boost::asio::placeholders::bytes_transferred, \
	  cb_handler)
	);
  return true;
}

bool TSocket::AsyncSend(char *buffer, size_t buffer_len, \
  SockAsyncCallBackHandler * cb_handler) {
  sock_ptr_->async_send(
	boost::asio::buffer(cb_handler->buffer(), buffer_len),
	boost::bind(&TSocket::WriteCallBack, this, \
	boost::asio::placeholders::error, \
	boost::asio::placeholders::bytes_transferred, \
	cb_handler)
	);
  return true;
}

void TSocket::ReadCallBack(const boost::system::error_code& error, \
  size_t bytes_transferred, \
  SockAsyncCallBackHandler * cb_call) {
  if (error) {
	// error occurs
	cb_call->CallBackFun(true, bytes_transferred);
	LOGGER(INFO) << "ReadCallBack:error info:" << error.message();
  }
  else {
	// normal 
	cb_call->CallBackFun(false, bytes_transferred);
  }
}

void TSocket::WriteCallBack(const boost::system::error_code& error, \
  size_t bytes_transferred, \
  SockAsyncCallBackHandler * cb_call) {
  if (error) {
	// error occurs
	cb_call->CallBackFun(true, bytes_transferred);
  }
  else {
	// normal 
	cb_call->CallBackFun(false, bytes_transferred);
  }
}

}  // namespace tyh 