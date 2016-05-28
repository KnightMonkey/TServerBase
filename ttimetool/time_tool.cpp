/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : 2010-05-11
 * brief  :
 *******************************************/

#include "time_tool.h"

namespace tyh {

namespace TChrono = boost::chrono;

const int TChronoTime::s_kHourTimeSec = 3600;
const int TChronoTime::s_kDayTimeSec = 86400;

TChronoTime::TChronoTime() {
}

TChronoTime::~TChronoTime() {
}

time_t TChronoTime::NowTime() {
  return TChrono::system_clock::to_time_t(TChrono::system_clock::now());
}

time_t TChronoTime::TodayStartTime() {
  time_t now_time = NowTime();
  time_t tmp = now_time % s_kDayTimeSec;
  time_t ret = now_time - (tmp+8*s_kHourTimeSec);
  return ret;
}

time_t TChronoTime::TodayHourTime(size_t hour) {
  return TodayStartTime() + (hour*s_kHourTimeSec);
}

unsigned long long TChronoTime::NowMiliSec() {
  TChrono::system_clock::time_point time_pt = TChrono::system_clock::now();
  TChrono::system_clock::duration dur = time_pt.time_since_epoch();

  return dur.count()/10000;
}

unsigned long long TChronoTime::NowMicroSec() {
  TChrono::system_clock::time_point time_pt = TChrono::system_clock::now();
  TChrono::system_clock::duration dur = time_pt.time_since_epoch();
  return dur.count() / 10;
}

void TChronoTime::LocalTime(time_t t, struct tm* out_tm) {
  if (NULL != out_tm) {
#ifdef WIN32
	localtime_s(out_tm, &t);
#else
	localtime_r(&t, out_tm);
#endif
  }
}



} // namespace tyh