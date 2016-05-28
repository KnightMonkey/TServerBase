/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#ifndef __TYH_TSQLDB_MYSQL_ACCESSOR_H__
#define __TYH_TSQLDB_MYSQL_ACCESSOR_H__

#include <map>
#include <string>

#include "tbase/boost_common_inc.h"
#include "tlog/tboost_log.h"

#include "soci.h"
#include "soci-mysql.h"

#include "tbase/ttypedef.h"
#include "tbase/nocopyable.h"

namespace tyh {

typedef soci::row RowData;

class PrepareStatement {
 public:
   PrepareStatement(std::string name);

   virtual ~PrepareStatement();
   bool Execute();
   bool Fetch();
   const RowData& row_data();
   virtual void InitSql(soci::session &sql);
 public:
   // member
   soci::statement *stmt_pt_;
   std::string name_;

   soci::row row_data_;
};

typedef boost::shared_ptr<PrepareStatement> StatementPtr;

class MySqlAccessor : public Nocopyable {
 public:
   MySqlAccessor();
   ~MySqlAccessor();
   bool Init(std::string host, std::string user, \
	 std::string pswd, std::string db_name, std::string encoding_set);

   soci::session & sql_instance() { return sql_instance_; }
   StatementPtr GetStatement(std::string stmt_name);

   virtual void InstallPrepareStatement();
   void Close();
 protected:
   soci::session sql_instance_;
  
   std::map<std::string, StatementPtr > statement_map_;
};




}	// namespace tyh



#endif  // __TYH_TSQLDB_MYSQL_ACCESSOR_H__