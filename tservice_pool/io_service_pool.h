/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : 2016-05-10
 * brief  :
 *******************************************/

#include <vector>

#include "tbase/boost_common_inc.h"
#include "tbase/ttypedef.h"
#include "tbase/nocopyable.h"

#ifndef __TYH_IO_SERVICE_POOL_H__
#define __TYH_IO_SERVICE_POOL_H__

typedef boost::shared_ptr<boost::asio::io_service> ServicePtr;
typedef boost::shared_ptr<boost::asio::io_service::work> WorkPtr;
typedef boost::asio::io_service IoService;

namespace tyh {

class IOServicePool : public Nocopyable {
public:
  explicit IOServicePool(size_t pool_size);
  ~IOServicePool();
  void Run();
  bool Stop();
  IoService & GetIoService();

private:
  std::vector<ServicePtr> vec_service_ptr_;
  std::vector<WorkPtr>	vec_work_ptr_;
  std::vector<boost::shared_ptr<boost::thread>> vec_thread_ptr_;
  size_t io_index_;
};

} // namespace tyh



#endif  // __TYH_IO_SERVICE_POOL_H__