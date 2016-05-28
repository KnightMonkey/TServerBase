/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   :	2016-05-10
 * brief  :
 *******************************************/
#include <algorithm>
#include "io_service_pool.h"

namespace tyh {

IOServicePool::IOServicePool(size_t pool_size) : io_index_(0){
  if (0 == pool_size) {
	throw std::runtime_error("io_service_pool.size = 0");
  }
  for (size_t i = 0; i < pool_size; ++i) {
	ServicePtr service_ptr(new boost::asio::io_service);
	vec_service_ptr_.push_back(service_ptr);
	WorkPtr work_ptr(new boost::asio::io_service::work(*service_ptr));
	vec_work_ptr_.push_back(work_ptr);
  }
}

IOServicePool::~IOServicePool() {

}

void IOServicePool::Run() {
  for (size_t i = 0; i < vec_service_ptr_.size(); ++i) {
	boost::shared_ptr<boost::thread> thread_ptr(
	  new boost::thread(
		boost::bind(&boost::asio::io_service::run, vec_service_ptr_[i])
	  )
	);
	vec_thread_ptr_.push_back(thread_ptr);
  }
}

bool IOServicePool::Stop() {
  for (size_t i = 0; i < vec_service_ptr_.size(); ++i) {
	vec_service_ptr_[i]->stop();
  }
  for (size_t i = 0; i < vec_thread_ptr_.size(); ++i) {
	vec_thread_ptr_[i]->join();
  }
  return true;
}

IoService & IOServicePool::GetIoService() {
  if (io_index_ >= vec_service_ptr_.size()) {
	io_index_ = 0;
  }
  return (*(vec_service_ptr_[io_index_++]));
}

} // namespace tyh