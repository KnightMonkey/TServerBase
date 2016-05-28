/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#include "tconnection/connection.h"
#include "tbase/ttypedef.h"
#include "synergy_cell.h"

#ifndef __TYH_DEFAULT_SYNERGY_CELL_H__
#define __TYH_DEFAULT_SYNERGY_CELL_H__

namespace tyh {

class DefaultSynergyCell : public SynergyCell {
public:
  DefaultSynergyCell(uint32 cell_id, boost::shared_ptr<TConnection> conn);
  virtual ~DefaultSynergyCell();

  virtual bool Run();
  virtual bool Run(time_t time_sec);
  boost::shared_ptr<TConnection> connection_ptr_;
};



} // namespace tyh


#endif	// __TYH_DEFAULT_SYNERGY_CELL_H__