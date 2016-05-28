/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#include "tsqldb/mysql_accessor.h"


// ≤‚ ‘‘§¥¶¿Ì”Ôæ‰
class PreStmtQueryStock : public tyh::PrepareStatement {
public:
  PreStmtQueryStock(std::string name);
  virtual ~PreStmtQueryStock();
  virtual void InitSql(soci::session &sql);
};







class TestSqlAccessor : public tyh::MySqlAccessor {
public:
  TestSqlAccessor();
  ~TestSqlAccessor();

  virtual void InstallPrepareStatement();
};