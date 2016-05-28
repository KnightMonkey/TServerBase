/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : 2016-05-11
 * brief  :
 *******************************************/
#include "synergy_worker.h"
#include "tlog/tboost_log.h"

namespace tyh {

SynergyWorker::SynergyWorker(size_t worker_size)
:worker_size_(worker_size) ,
stop_(false),
working_size_(0){
  time_sec_ = 0;
  Start();
}

SynergyWorker::~SynergyWorker() {

}

void SynergyWorker::WorkerThread(int thread_id) {
  while (!stop_) {
	typedef boost::unique_lock<boost::mutex> UniqueLock;
	UniqueLock queue_lock(queue_mutex_);
	while (synergy_cell_queue_.empty()) {
	  if (stop_) {
		break;
	  }
	  have_work_.wait(queue_lock);
	}	
	if (synergy_cell_queue_.size() > 0) {
	  SynergyCellPtr cell_ptr = synergy_cell_queue_.front();
	  synergy_cell_queue_.pop_front();
	  queue_lock.unlock();
	  cell_ptr->Run(time_sec_);
	  LOGGER(DEBUG) << "[SynergyWorker::WorkerThread,cell_ptr->Run()], thread,id_ =  " << thread_id << std::endl;
	}
	ReduceWorkingSize();
  }
  //std::cerr << "exit thread,id =  " << thread_id << std::endl;
  return;
}

void SynergyWorker::Start() {
  for (size_t i = 0; i < worker_size_; ++i) {
	ThreadPtr thread_ptr(
	  new boost::thread(boost::bind(&SynergyWorker::WorkerThread, this, i+1))
	  );
	vec_worker_.push_back(thread_ptr);
  }
  return;
}

void SynergyWorker::ReduceWorkingSize() {
  typedef boost::unique_lock<boost::mutex> UniqueLock;
  UniqueLock working_lock(working_mutex_);
  if (working_size_ > 0) {
	--working_size_;
	working_var_.notify_one();
  }
}

void SynergyWorker::IncreaseWorkingSize() {
  typedef boost::unique_lock<boost::mutex> UniqueLock;
  UniqueLock working_lock(working_mutex_);
	++working_size_;
}

void SynergyWorker::WaitFinish() {
  typedef boost::unique_lock<boost::mutex> UniqueLock;
  UniqueLock working_lock(working_mutex_);
  while (working_size_ > 0) {
	//std::cerr << "working_size = " << working_size_ << std::endl;
	working_var_.wait(working_lock);
  }
  //std::cerr << "working_size = " << working_size_ << std::endl;
}

void SynergyWorker::AddSynergyCell(SynergyCellPtr cell_ptr) {
  IncreaseWorkingSize();
  typedef boost::unique_lock<boost::mutex> UniqueLock;
  UniqueLock queue_lock(queue_mutex_);
  synergy_cell_queue_.push_back(cell_ptr);
  have_work_.notify_one();
}

void SynergyWorker::Stop() {
  stop_ = true;
  have_work_.notify_all();
  for (std::vector<ThreadPtr>::iterator it = vec_worker_.begin(); \
	it != vec_worker_.end(); ++it) {
	(*it)->join();
  }
}

}  // namespace tyh