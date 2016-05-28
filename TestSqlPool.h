/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#include "tsqldb/mysql_db_pool.h"


class TestSqlPool : public tyh::MySqlDbPool {
public:
  TestSqlPool();
  ~TestSqlPool();

protected:
  virtual tyh::MySqlConnPtr CreateNewAccessor();


};