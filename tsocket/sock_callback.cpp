/*
*	Copyright(c) 2016 - 2020 tianyiheng
*	autor : tianyiheng
*	date : 2016 - 05 - 05
*	brief :
*/

#include <cassert>
#include "sock_callback.h"


namespace tyh {
void SockAsyncCallBackHandler::CallBackFun(bool err, \
	size_t transfered_byte) {
}

SockAsyncCallBackHandler::~SockAsyncCallBackHandler() {
  if (NULL != data_buffer_) {
	delete[] data_buffer_, data_buffer_ = NULL;
	buffer_len_ = 0;
	cur_data_len_ = 0;
  }
}

void SockAsyncCallBackHandler::SetBufferSize(size_t buffer_len) {
  if (NULL != data_buffer_) {
	delete[] data_buffer_, data_buffer_ = NULL;
  }
  data_buffer_ = new char[buffer_len];
  assert(NULL != data_buffer_);
  buffer_len_ = buffer_len;
  cur_data_len_ = 0;
}

} // namespace tyh