/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#include <sstream>

#include "mysql_accessor.h"

namespace tyh  {

// prepare statement --------------------------
PrepareStatement::PrepareStatement(std::string name) {
  name_ = name;
  stmt_pt_ = NULL;
}

PrepareStatement::~PrepareStatement() {
  if (NULL != stmt_pt_) {
	delete stmt_pt_, stmt_pt_ = NULL;
  }
}

bool PrepareStatement::Execute() {
  bool ret = true;
  try {
	stmt_pt_->execute(false);
  }
  catch (soci::mysql_soci_error & e) {
	LOGGER(ERR) << "PrepareStatement::Execute(), " << e.what();
	ret = false;
  }
  return ret;
}

bool PrepareStatement::Fetch() {
  return stmt_pt_->fetch();
}

const RowData& PrepareStatement::row_data() {
  return row_data_;
}

void PrepareStatement::InitSql(soci::session &sql) {
  soci::statement stmt = (sql.prepare << "select * from test_db");
  stmt_pt_ = new soci::statement(stmt);
}

// MySqlAccessor -------------------------------

MySqlAccessor::MySqlAccessor() {

}

MySqlAccessor::~MySqlAccessor() {
}


bool MySqlAccessor::Init(std::string host, std::string user, \
  std::string pswd, std::string db_name, std::string encoding_set) {
  bool ret = true;
  std::stringstream sstrm;
  sstrm << "host=" << host << " db=" << db_name \
	<< " user=" << user << " password='" << pswd << "'";
  std::string conn_string = sstrm.str();
  try {
	sql_instance_.open("mysql", conn_string);
	std::string tmp = "SET names ";
	tmp.append(encoding_set);
	sql_instance_ << tmp;
	InstallPrepareStatement();
  }
  catch (soci::mysql_soci_error &e) {
	std::cerr << e.what() << std::endl;
	ret = false;
  }
  return ret;
}

StatementPtr MySqlAccessor::GetStatement(std::string stmt_name) {
  StatementPtr tmp;
  std::map<std::string, StatementPtr >::iterator it = statement_map_.find(stmt_name);
  if (it != statement_map_.end()) {
	tmp = it->second;
  }
  else {
	std::string msg = "have not statement named ";
	msg += stmt_name;
	throw msg;
  }
  return tmp;
}

void MySqlAccessor::InstallPrepareStatement() {
  StatementPtr stmt_ptr(new PrepareStatement("base_statement"));
  stmt_ptr->InitSql(sql_instance_);
}

void MySqlAccessor::Close() {
  sql_instance_.close();
}

}	// namespace tyh