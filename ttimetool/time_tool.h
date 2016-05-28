/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#ifndef __TYH_TIME_TOOL_H__
#define __TYH_TIME_TOOL_H__

//#define BOOST_CHRONO_HEADER_ONLY

#include <ctime>

#include "tbase/boost_common_inc.h"

namespace tyh {

class TChronoTime {
protected:
  TChronoTime();
  ~TChronoTime();

public:
  static time_t NowTime();
  static time_t TodayStartTime();
  static time_t TodayHourTime(size_t hour);
  static unsigned long long NowMiliSec();
  static unsigned long long NowMicroSec();
  static const int s_kHourTimeSec;
  static const int s_kDayTimeSec;
  static void LocalTime(time_t t,struct tm* out_tm);
};

} // namespace tyh
#endif	// __TIME_TOOL_H__