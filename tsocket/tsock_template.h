/*
 *	Copyright (c) 2016-2020 tianyiheng
 *  autor : tianyiheng
 *  date  : 2016-05-05
 *  brief :
 */

#ifndef __TYH_SOCK_TEMPLATE_H__
#define __TYH_SOCK_TEMPLATE_H__

// c++ std
#include <string>

#include "tbase/ttypedef.h"

namespace tyh {

template<class T>
class TSockBase {
 public:
   TSockBase() {sock_ptr_ = NULL;}
   ~TSockBase() { 
	 if (NULL != sock_ptr_) 
	   delete sock_ptr_, sock_ptr_ = NULL;
   }
  typedef T sock_base_type;

  virtual bool Connect(std::string host, uint32 port) = 0;
  virtual void KeepAlive(bool alive = false) {};
  virtual void Noblocking(bool block = false) {};
  virtual void SetSendBufferSize(uint32 buf_size){};
  virtual void SetRecvBufferSize(uint32 buf_size) {};
  virtual void ShutDownRecv() = 0;
  virtual void ShutDownSend() = 0;
  virtual void ShutDownAll() = 0;
  virtual void Close() = 0;
  virtual bool IsOpen() = 0;

 protected:
   sock_base_type *sock_ptr_;
};

}  // namespace tyh

#endif  // __TYH_SOCK_TEMPLATE_H__