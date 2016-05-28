/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/
#include "tsingleton/singleton_template.h"
#include "MyServer.h"
#include "MyConnect.h"
#include "tconfig/config_manager.h"

#include "TestSqlAccessor.h"
#include "TestSqlPool.h"

#include "tkvdb/redis_db_pool.h"
#include "tutility/char_conv.h"

MyServer::MyServer(size_t pool_size/*, std::string host, uint32 port*/) :
  tyh::TBaseServer(pool_size/*, host, port*/)
{


}

MyServer::~MyServer() {

}

ConnectionPtr MyServer::GetNewConnection() {
  LOGGER(INFO) << "MyServer::GetNewConnection()";
  boost::shared_ptr<MyConnection> conn_ptr(new MyConnection(io_service_pool_.GetIoService()));
  return conn_ptr;
}

bool MyServer::InitDatabase() {
  // mysql config
  tyh::MysqlBaseConfig const &mysql_conf = tyh::ConfMgr->mysql_base_config();

  LOGGER(INFO) << "MyServer::LoadConfig()";
  tyh::TSingleton<TestSqlPool>::Instance()->Init(
	mysql_conf.pool_size,
	mysql_conf.host,
	mysql_conf.user,
	mysql_conf.pswd,
	mysql_conf.database,
	mysql_conf.encoding
	);

  // redis config
  tyh::RedisPoolConfig const & redis_pool_config = tyh::ConfMgr->redis_pool_config();
  tyh::TSingleton<tyh::RedisDbPool>::Instance()->Init(
	redis_pool_config.pool_size_,
	redis_pool_config.host_,
	redis_pool_config.port_,
	redis_pool_config.pswd_,
	redis_pool_config.db_
	);

  return true;
}

bool MyServer::LoadConfig() {
  tyh::TBaseServer::LoadConfig();
  return true;
}

void MyServer::MainRun(time_t cur_time) {
  tyh::TBaseServer::MainRun(cur_time);
  LOGGER(INFO) << "MyServer::MainRun";

  // 下面的逻辑为测试逻辑
  // test read data from mysql
#define SqlPoolPtr tyh::TSingleton<TestSqlPool>::Instance()
  tyh::MySqlConnPtr sql_accessor_ptr = SqlPoolPtr->GetMySqlAccessor();
  try {
	tyh::StatementPtr stmt_ptr = sql_accessor_ptr->GetStatement("stmt1");
	boost::shared_ptr<PreStmtQueryStock> stock_stmt_pt = boost::dynamic_pointer_cast<PreStmtQueryStock>(stmt_ptr);
	if (NULL != stock_stmt_pt) {
	  stock_stmt_pt->Execute();
	  while (stock_stmt_pt->Fetch()) {
		const tyh::RowData& row_data = stock_stmt_pt->row_data();
		LOGGER(INFO) << "Row data:" << row_data.get<std::string>(0);
	  }
	  SqlPoolPtr->RecycleSqlAccessor(sql_accessor_ptr);
	}
	else {
	  LOGGER(ERR) << "GetStatement(\"stmt1\")";
	}
  }
  catch (std::string & e){
	LOGGER(ERR) << e;
  }
  LOGGER(INFO) << "Test read data from [mysql] end ...";
  // mysql tes end
  // Test read data from redis

#define RedisPoolPtr tyh::TSingleton<tyh::RedisDbPool>::Instance()
  tyh::RedisConnPtr redis_accessor_ptr = RedisPoolPtr->GetRedisAccessor();
  tyh::RedisCmdReply *cmd_reply_pt = NULL;
  LOGGER(INFO) << "Test read data from redis";
  redis_accessor_ptr->DoCommand(&cmd_reply_pt, "hget News_20160523 %s", "082335_d4b89b06637817c4261e8b2ef41ee0de");
  if (NULL != cmd_reply_pt) {
	if (cmd_reply_pt->reply_type() == tyh::EREDIS_REPLY_STRING) {
	  tyh::RedisStringReply *str_reply = dynamic_cast<tyh::RedisStringReply *>(cmd_reply_pt);
	  std::string gbk_str;
	  tyh::EncodingConv::Utf8ToGb2312(str_reply->reply_value_, gbk_str);
	  LOGGER(DEBUG) << "str_reply:" << gbk_str;
	}
	else {
	  LOGGER(ERR) << "failed to read data from redis";
	}
  }
  RedisPoolPtr->RecycleRedisAccecor(redis_accessor_ptr);
  LOGGER(INFO) << "Test read data from [redis] end ...";
  // redis test end
}