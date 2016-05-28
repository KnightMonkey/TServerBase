/*
*	Copyright(c) 2016 - 2020 tianyiheng
*	autor : tianyiheng
*	date : 2016 - 05 - 05
*	brief :
*/

#ifndef __TYH_SOCK_CALLBACK_H__
#define __TYH_SOCK_CALLBACK_H__

#include "tbase/ttypedef.h"
#include "tbase/nocopyable.h"

namespace tyh {


class SockAsyncCallBackHandler : public Nocopyable {
  public:
	SockAsyncCallBackHandler() {
	  data_buffer_ = NULL;
	  buffer_len_ = 0;
	  cur_data_len_ = 0;
	}
	virtual ~SockAsyncCallBackHandler();
	virtual void CallBackFun(bool err, size_t transfered_byte);

	char* buffer() { return data_buffer_ + cur_data_len_;}
	size_t buffer_len() { return buffer_len_ - cur_data_len_; }
	void SetCurDataLen(size_t cur_len) { cur_data_len_ = cur_len; }
	void SetBufferSize(size_t buffer_len);
	size_t DataLength() const { return cur_data_len_; }
	void Clear() { cur_data_len_ = 0; }

	template <typename T>
	T GetHead() {
	  T* head_pt = reinterpret_cast<T*>(data_buffer_);
	  return *head_pt;
	}

  protected:
	char *data_buffer_;
	size_t buffer_len_;
	size_t cur_data_len_;
};

}  // namespace tyh
#endif	// __TYH_SOCK_CALLBACK_H__