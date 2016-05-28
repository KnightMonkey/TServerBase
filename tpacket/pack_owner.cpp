/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : 2016-05-19
 * brief  :
 *******************************************/

#include "pack_owner.h"

namespace tyh {

bool PackBufferOwner::PushPackBuffer(PackBufferPtr pack_buff_ptr) {
  boost::unique_lock<boost::mutex> scoped_lock(mutex_);
  pack_buffer_queue_.push_back(pack_buff_ptr);
  return true;
}

bool PackBufferOwner::IsEmpty() {
  boost::unique_lock<boost::mutex> scoped_lock(mutex_);

  return pack_buffer_queue_.empty();
}

bool PackBufferOwner::PopPackBuffer(PackBufferPtr & r_ptr_) {
  bool ret = false;
  boost::unique_lock<boost::mutex> scoped_lock(mutex_);
  if (!pack_buffer_queue_.empty()) {
	r_ptr_ = pack_buffer_queue_.front();
	pack_buffer_queue_.pop_front();
	ret = true;
  }
  return ret;
}

}