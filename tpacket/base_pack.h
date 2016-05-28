/******************************************
 * Copyright (C) 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : 2016-05-09
 * brief  :
 *******************************************/

#ifndef __TYH_BASE_PACK_H__
#define __TYH_BASE_PACK_H__

#include <cassert>
#include <memory>
#include "tbase/ttypedef.h"

namespace tyh {

enum PackBufferSize {
MIN_PACK_BUFFER_SIZE = 1024,		// 1K
NORMAL_PACK_BUFFER_SIZE = 8192,		// 8K
MID_PACK_BUFFER_SIZE = 16384,		// 16K
MAX_PACK_BUFFER_SIZE = 64512		// 63K
};

template <PackBufferSize pack_size = MIN_PACK_BUFFER_SIZE>
class PackBufferCell {
public:
  PackBufferCell() {
	read_indx_ = 0;
	write_indx_ = 0;
	buffer_size_ = pack_size;
  }

  ~PackBufferCell() {}

  bool AppendData(char *data, uint32 len) {
	uint32 final_len = ((buffer_size_ - write_indx_) < len) ? 
	  (buffer_size_ - write_indx_) : (len);
	if (0 < final_len) {
	  memcpy(buffer_ + write_indx_, data, final_len);
	  write_indx_ += final_len;
	}
	else {
	  return false;
	}
	return (final_len == len);
  }

  inline const char* buffer() { return buffer_ + read_indx_; }
  inline uint32 length() { return write_indx_ - read_indx_; }

  void Reset() {
	read_indx_ = 0;
	write_indx_ = 0;
  }

private:
  char buffer_[pack_size];
  uint16 read_indx_;
  uint16 write_indx_;
  uint32 buffer_size_;
};

class PackHead {
public:
  PackHead() :pack_length_(0) {}
  ~PackHead() {}
  inline uint32 pack_length() const { return pack_length_; }
  inline void set_pack_length(int32 pack_length) { pack_length_ = pack_length; }

protected:
  uint32 pack_length_;
};

class PackBase : public PackHead {
public:
  PackBase(uint16 pack_type) : pack_type_(pack_type){
  }

  ~PackBase() {
  }

  inline uint16 pack_type() const { return pack_type_; }

protected:
  uint16 pack_type_;
};


} // namespace tyh
#endif  // __TYH_BASE_PACK_H__