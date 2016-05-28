/*******************************************************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************************************************/
#include <sstream>
#include <cstdarg>
#include "redis_accessor.h"

#if defined _WIN32
#include "win32fixes.h"
#endif

namespace tyh {


RedisAccessor::RedisAccessor() :
  context_(NULL),
  host_(""),
  port_(0),
  pswd_(""){
  db_index_ = 0;

}

RedisAccessor::~RedisAccessor()  {

}

bool RedisAccessor::ConnectServer(std::string host, int port, std::string pswd) {
  bool ret = true;

  host_ = host;
  port_ = port;
  pswd_ = pswd;

  context_ = redisConnect(host.c_str(), port);
  if (NULL == context_) {
	ret = false;
  }
  if (0 != context_->err) {
	ret = false;
  }
  if (ret) {
	redisReply *reply = \
	  reinterpret_cast<redisReply *>(redisCommand(context_, "auth %s", pswd.c_str()));
	// judge reply type if is status
	if (NULL != reply && REDIS_REPLY_ERROR == reply->type) {
	  ret = false;
	  freeReplyObject(reply);
	  //throw std::string(reply->str);
	}
  }

  return ret;
}

bool RedisAccessor::Ping() {
  redisReply *reply = reinterpret_cast<redisReply*>(redisCommand(context_, "PING"));
  std::string res;
  if (NULL != reply) {
	res.assign(reply->str, reply->len);
  }
  return res == "PONG";
}

RedisCmdReply* RedisAccessor::FetchCmdReply(redisReply *redis_reply) {
  if (NULL == redis_reply) {
	return NULL;
  }
  RedisCmdReply*  result_pt = NULL;
  switch (redis_reply->type) {
	case REDIS_REPLY_STRING: {
	  RedisStringReply *cmd_reply = new RedisStringReply;
	  cmd_reply->reply_value_.assign(redis_reply->str, redis_reply->len);
	  result_pt = cmd_reply;
	  break;
	}
	case REDIS_REPLY_ARRAY: {
	  RedisArrayReply *cmd_reply = new RedisArrayReply;
	  for (size_t i = 0; i < redis_reply->elements; ++i) {
		redisReply *tmp_pt = redis_reply->element[i];
		RedisCmdReply* ret_tem = FetchCmdReply(tmp_pt);
		if (NULL != ret_tem) {
		  cmd_reply->reply_value_.push_back(ret_tem);
		}
	  }
	  result_pt = cmd_reply;
	  break;
	}
	case REDIS_REPLY_INTEGER: {
	  RedisIntReply *cmd_reply = new RedisIntReply;
	  cmd_reply->reply_value_ = redis_reply->integer;
	  result_pt = cmd_reply;
	  break;
	}
	case REDIS_REPLY_NIL: {
	  RedisNilReply *cmd_reply = new RedisNilReply;
	  result_pt = cmd_reply;
	  break;
	}
	case REDIS_REPLY_STATUS: {
	  RedisStatusReply *cmd_reply = new RedisStatusReply;
	  cmd_reply->reply_value_.assign(redis_reply->str, redis_reply->len);
	  break;
	}
	case REDIS_REPLY_ERROR: {
	  RedisErrorReply *cmd_reply = new RedisErrorReply;
	  cmd_reply->reply_value_.assign(redis_reply->str, redis_reply->len);
	  result_pt = cmd_reply;
	  break;
	}
	default: {
	  break;
	}
  }
  return result_pt;
}


bool RedisAccessor::DoCommand(RedisCmdReply ** out_reply_pt, char * format, ...) {
  bool ret = true;
  char buff[1024] = "";
  va_list vsl;
  va_start(vsl, format);
  redisReply * redis_reply = \
	reinterpret_cast<redisReply*>(redisvCommand(context_, format, vsl));
  
  if (NULL == redis_reply || NULL == out_reply_pt) {
	ret = false;
  }
  else {
	if (NULL != *out_reply_pt) {
	  delete (*out_reply_pt);
	}
	RedisCmdReply * cmd_reply = FetchCmdReply(redis_reply);
	if (NULL != out_reply_pt) {
	  *out_reply_pt = cmd_reply;
	}
	freeReplyObject(redis_reply);
  }

  va_end(vsl);
  return ret;
}

void RedisAccessor::FreeCmdReply(RedisCmdReply* cmd_reply) {
  if (NULL != cmd_reply){
	delete cmd_reply;
  }
}

void RedisAccessor::Close() {
  redisFree(context_);
  context_ = NULL;
}

bool RedisAccessor::SelectDatabase(int db_index) {
  bool ret = false;
  std::stringstream ss;
  ss << "select " << db_index;
  std::string result;
  redisReply *reply = reinterpret_cast<redisReply *>(redisCommand(context_, ss.str().c_str()));
  if (NULL != reply) {
	result.assign(reply->str, reply->len);
	freeReplyObject(reply);
  }
  ret = (result == "OK");
  if (ret) {
	db_index_ = db_index;
  }
  
  return ret;
}

bool RedisAccessor::Reconnect() {
  redisFree(context_);
  context_ = NULL;
  return ConnectServer(host_, port_, pswd_);
}

}  // namesapce tyh