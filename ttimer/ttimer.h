/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#ifndef __TYH_SERVER_BASE_TTIMER_TTIMER_H__
#define __TYH_SERVER_BASE_TTIMER_TTIMER_H__

#include <ctime>

#include "tbase/ttypedef.h"
#include "ttimetool/time_tool.h"


namespace tyh {

// ¶¨Ê±Æ÷
class TTimer {
public:
  explicit TTimer(time_t start_time);
  ~TTimer();
  time_t ElapsedSec(time_t now_time);
  void Reset(time_t start_time);

private:
  time_t start_time_;
};


} // namespace tyh



#endif  // __TYH_SERVER_BASE_TTIMER_TTIMER_H__