/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#include "ttimer.h"

namespace tyh {

TTimer::TTimer(time_t start_time) {
  start_time_ = start_time;
}

TTimer::~TTimer() {

}

time_t TTimer::ElapsedSec(time_t now_time) {
  return now_time - start_time_;
}

void TTimer::Reset(time_t start_time) {
  start_time_ = start_time;
}

}  // namespace tyh