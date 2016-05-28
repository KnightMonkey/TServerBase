/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#include <map>
#include <vector>

#include "synergy_cell.h"
#include "tbase/boost_common_inc.h"
#include "tsingleton/singleton_template.h"
#include "tbase/ttypedef.h"
#include "tlog/tboost_log.h"
#include "ttimetool/time_tool.h"

#ifndef __TYH_SYNERGY_CELL_MANAGER_H__
#define __TYH_SYNERGY_CELL_MANAGER_H__

namespace tyh {

typedef boost::shared_ptr<SynergyCell> SynergyCellPtr;

class SynergyCellManager : public TSingleton<SynergyCellManager> {
protected:
  SynergyCellManager();
  ~SynergyCellManager();
  friend class TSingleton<SynergyCellManager>;

public:
  bool AddSynergyCell(uint32 cell_key, SynergyCellPtr cell_ptr);
  SynergyCellPtr GetSynergyCell(uint32 cell_key);
  bool HasCell(uint32 cell_key);
  void GetAllSynergyCell(std::vector<SynergyCellPtr> * vec_cell_pt);
  bool RemoveCell(uint32 cell_key);
  void RoundRemove();
  size_t Size() { return map_synergy_cell_.size(); }
  
private:
  std::map<uint32, SynergyCellPtr> map_synergy_cell_;
  
};


}	// namespace tyh
#endif	// __TYH_SYNERGY_CELL_MANAGER_H__