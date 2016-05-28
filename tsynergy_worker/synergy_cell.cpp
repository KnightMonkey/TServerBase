/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#include "synergy_cell.h"

namespace tyh {

SynergyCell::SynergyCell(uint32 cell_id) :
cell_id_(cell_id),
valid_(true){

}

SynergyCell::~SynergyCell() {

}

bool SynergyCell::Run() {

  return true;
}

bool SynergyCell::Run(time_t time_sec) {
  return true;
}

} // namespace tyh