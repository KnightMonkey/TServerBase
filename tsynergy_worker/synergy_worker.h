/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#ifndef __TYH_SYNERGY_WORKER_H__
#define __TYH_SYNERGY_WORKER_H__

#include <deque>
#include <vector>

#include "boost/thread.hpp"
#include "boost/thread/condition.hpp"
#include "boost/thread/condition_variable.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/smart_ptr/shared_ptr.hpp"
#include "synergy_cell.h"
#include "tsingleton/singleton_template.h"
#include "tbase/nocopyable.h"
#include "ttimetool/time_tool.h"

namespace tyh {


typedef boost::shared_ptr<SynergyCell> SynergyCellPtr;
typedef boost::shared_ptr<boost::thread> ThreadPtr;

class SynergyWorker : public Nocopyable {
public:
  explicit SynergyWorker(size_t worker_size = 1);
  ~SynergyWorker();

public:
  inline size_t worker_size() const { return worker_size_; }
  void WaitFinish();
  void AddSynergyCell(SynergyCellPtr cell_ptr);
  void Stop();
  inline void set_time_sec(time_t sec) { time_sec_ = sec; }
  inline time_t time_sec() const { return time_sec_; }
protected:
  void Start();
  void WorkerThread(int thread_id);

  void ReduceWorkingSize();
  void IncreaseWorkingSize();

protected:
  size_t worker_size_;
  std::deque<SynergyCellPtr> synergy_cell_queue_;
  std::vector<ThreadPtr> vec_worker_;
  bool stop_;
  // sync
  boost::mutex queue_mutex_, working_mutex_;
  boost::condition_variable_any working_var_, have_work_;
  size_t working_size_;
  time_t time_sec_;
};



}	// namespace tyh


#endif