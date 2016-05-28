/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/
#include <utility>
#include "synergy_cell_manager.h"

namespace tyh {
SynergyCellManager::SynergyCellManager() {
}

SynergyCellManager::~SynergyCellManager() {
}

bool SynergyCellManager::AddSynergyCell(uint32 cell_key, SynergyCellPtr cell_ptr) {
  std::map<uint32, SynergyCellPtr>::iterator it_cell = map_synergy_cell_.find(cell_key);
  if (it_cell == map_synergy_cell_.end()) {
	LOGGER(INFO) << "SynergyCellManager::AddSynergyCell, key=" << cell_key;
	map_synergy_cell_.insert(std::make_pair(cell_key, cell_ptr));
	return true;
  }
  
  return false;
}

SynergyCellPtr SynergyCellManager::GetSynergyCell(uint32 cell_key) {
  std::map<uint32, SynergyCellPtr>::iterator it_cell = map_synergy_cell_.find(cell_key);
  if (it_cell != map_synergy_cell_.end()) {
	return it_cell->second;
  }
  return SynergyCellPtr();
}

bool SynergyCellManager::HasCell(uint32 cell_key) {
  return (map_synergy_cell_.find(cell_key) != map_synergy_cell_.end());
}

bool SynergyCellManager::RemoveCell(uint32 cell_key) {
  std::map<uint32, SynergyCellPtr>::iterator it_cell = map_synergy_cell_.find(cell_key);
  if (it_cell != map_synergy_cell_.end()) {
	LOGGER(INFO) << "SynergyCellManager::RemoveCell, key=" << cell_key;
	map_synergy_cell_.erase(it_cell);
	
  }
  return true;
}

void SynergyCellManager::GetAllSynergyCell(std::vector<SynergyCellPtr> * vec_cell_pt) {
  if (NULL == vec_cell_pt) {
	return;
  }
  typedef std::map<uint32, SynergyCellPtr>::iterator CellIterator;
  for (CellIterator it = map_synergy_cell_.begin(); it != map_synergy_cell_.end(); ++it) {
	vec_cell_pt->push_back(it->second);
  }
}

void SynergyCellManager::RoundRemove() {
  typedef std::map<uint32, SynergyCellPtr>::iterator CellIterator;
  for (CellIterator it = map_synergy_cell_.begin(); it != map_synergy_cell_.end();) {
	if (!(it->second->valid())) {
	  LOGGER(INFO) << "-----SynergyCellManager::RoundRemove()";
	  CellIterator it_remove = it++;
	  map_synergy_cell_.erase(it_remove);
	}
	else {
	  it++;
	}
  }
}

} // namespace tyh