/******************************************
 * Copyright (C) 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : 2016-05-09
 * brief  :
 *******************************************/

#ifndef __TYH_PACK_OWNER_H__
#define __TYH_PACK_OWNER_H__

#include <cassert>
#include <memory>
#include <deque>

#include "tbase/ttypedef.h"
#include "tpacket/base_pack.h"
#include "tbase/nocopyable.h"
#include "boost/shared_ptr.hpp"
#include "boost/thread/mutex.hpp"


namespace tyh {

typedef boost::shared_ptr < PackBufferCell< NORMAL_PACK_BUFFER_SIZE > > PackBufferPtr;
typedef std::deque< PackBufferPtr > PacketBufferQueue;

class PackBufferOwner : public Nocopyable {
public:
  PackBufferOwner() {}
  ~PackBufferOwner() {}
 
public:
  bool PushPackBuffer(PackBufferPtr pack_buff_ptr);
  bool IsEmpty();
  bool PopPackBuffer(PackBufferPtr & r_ptr_);

protected:
  PacketBufferQueue pack_buffer_queue_;

private:
  boost::mutex mutex_;
};


} // namespace tyh

#endif	// __TYH_PACK_OWNER_H__