/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#include "tlog/tboost_log.h"

#include "TestSqlPool.h"
#include "TestSqlAccessor.h"


TestSqlPool::TestSqlPool() {

}

TestSqlPool::~TestSqlPool() {

}

tyh::MySqlConnPtr TestSqlPool::CreateNewAccessor() {
  LOGGER(INFO) << "TestSqlPool::CreateNewAccessor()";
  tyh::MySqlConnPtr tmp(new TestSqlAccessor);
  return tmp;
}