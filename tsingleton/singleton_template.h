/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : 2016-05-09
 * brief  :
 *******************************************/
#define MT_SINGLETON

#ifndef __TYH_SINGLETON_H__
#define __TYH_SINGLETON_H__

#include "tbase/boost_common_inc.h"

namespace tyh {

template <class T>
class TSingleton {
public:
  static  T* Instance() {
#ifdef 	MT_SINGLETON
	if (NULL == s_instance_) {
	  boost::unique_lock<boost::mutex> scoped_lock(s_mutex_);
#endif
	  if (NULL == s_instance_) {
		s_instance_ = new T;
	  }
#ifdef MT_SINGLETON
	}
#endif
	return s_instance_;
  }

  static void Release() {
#ifdef 	MT_SINGLETON
	if (NULL != s_instance_) {
	  boost::unique_lock<boost::mutex> scoped_lock(s_mutex_);
#endif
	  if (NULL != s_instance_) {
		delete s_instance_;
		s_instance_ = NULL;
	  }
#ifdef MT_SINGLETON
	}
#endif
  }

protected:
  TSingleton() {}
  ~TSingleton() {}

private:
  static T* s_instance_;

#ifdef MT_SINGLETON
static boost::mutex s_mutex_;
#endif
};

// init

template <class T>
T* TSingleton<T>::s_instance_ = NULL;

#ifdef MT_SINGLETON
template <class T>
boost::mutex TSingleton<T>::s_mutex_;
#endif

}  // namespace tyh;


#endif __TYH_SINGLETON_H__