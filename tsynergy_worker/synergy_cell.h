/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/
#include <ctime>
#include "tbase/ttypedef.h"

#ifndef __TYH_SYNERGY_CELL_H__
#define __TYH_SYNERGY_CELL_H__

namespace tyh {

class SynergyCell {
public:
  SynergyCell(uint32 cell_id);
  virtual ~SynergyCell();

  virtual bool Run();
  virtual bool Run(time_t time_sec);
  inline uint32 cell_id() const { return cell_id_; }
  bool valid() { return valid_; }
protected:
  uint32 cell_id_;
  bool valid_;
};



} // namespace tyh


#endif	// __TYH_SYNERGY_CELL_H__