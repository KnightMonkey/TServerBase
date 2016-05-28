/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/
#include <utility>

#include "tlog/tboost_log.h"
#include "TestSqlAccessor.h"


// prepare statement
// ---------------------------------------------------
PreStmtQueryStock::PreStmtQueryStock(std::string name)
:tyh::PrepareStatement(name) {


}


PreStmtQueryStock::~PreStmtQueryStock() {

}

void PreStmtQueryStock::InitSql(soci::session &sql) {
  try {
	soci::statement stmt = \
	  (sql.prepare << "select stock_code from strategy_stock_info", into(row_data_));
	stmt_pt_ = new soci::statement(stmt);
  }
  catch (soci::mysql_soci_error const &e) {
	LOGGER(ERR) << e.what();
  }
  
}


// TestSqlAccessor
// ---------------------------------------------------
TestSqlAccessor::TestSqlAccessor() {

}

TestSqlAccessor::~TestSqlAccessor() {

}

void TestSqlAccessor::InstallPrepareStatement() {
  tyh::StatementPtr stmt_ptr(new PreStmtQueryStock("stmt1"));
  stmt_ptr->InitSql(sql_instance_);

  statement_map_.insert(std::make_pair(stmt_ptr->name_, stmt_ptr));
  LOGGER(INFO) << "TestSqlAccessor::InstallPrepareStatement()";
}