/*******************************************************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************************************************/

#ifndef __TYH_SERVERBASE_KVDB_REDIS_ACCESSOR_H__
#define __TYH_SERVERBASE_KVDB_REDIS_ACCESSOR_H__

#include <string>
#include <vector>

#include "hiredis.h"
#define NO_QFORKIMPL //这一行必须加才能正常使用



namespace tyh {
// reply type
// -----------------------------------------------------------------------
enum RedisReplyType {
  EREDIS_REPLY_NONE = 0,
  EREDIS_REPLY_STRING = 1,
  EREDIS_REPLY_ARRAY = 2,
  EREDIS_REPLY_INTEGER =3,
  EREDIS_REPLY_NIL = 4,
  EREDIS_REPLY_STATUS = 5,
  EREDIS_REPLY_ERROR = 6
};


struct RedisCmdReply {
public:
  RedisCmdReply() { }
  virtual ~RedisCmdReply() {}
  virtual RedisReplyType reply_type() = 0;
};

struct RedisStringReply : public RedisCmdReply {
public:
  RedisStringReply() 
	: RedisCmdReply(){
  }
  virtual ~RedisStringReply() {};

  virtual RedisReplyType reply_type() { return EREDIS_REPLY_STRING; }

  std::string reply_value_;
};

struct RedisArrayReply : public RedisCmdReply {
public:
  RedisArrayReply(){}
  virtual ~RedisArrayReply() {
	std::vector<RedisCmdReply *>::iterator it = reply_value_.begin();
	for (; it != reply_value_.end(); ++it) {
	  if (NULL != *it) {
		delete (*it);
		*it = NULL;
	  }
	}
  }
  virtual RedisReplyType reply_type() { return EREDIS_REPLY_ARRAY; }
  
  std::vector<RedisCmdReply*> reply_value_;
};


struct RedisIntReply : public RedisCmdReply {
public:
  RedisIntReply(){ reply_value_ = 0; }
  virtual ~RedisIntReply() {}
  virtual RedisReplyType reply_type() { return EREDIS_REPLY_INTEGER; }

  long long reply_value_;
};

struct RedisNilReply : public RedisCmdReply {
public:
  RedisNilReply() { reply_value_ = 0; };
  virtual ~RedisNilReply() {}
  virtual RedisReplyType reply_type() { return EREDIS_REPLY_NIL; }

  int reply_value_;
};


struct RedisStatusReply : public RedisCmdReply {
public:
  RedisStatusReply() {};
  virtual ~RedisStatusReply() {};
  virtual RedisReplyType reply_type() { return EREDIS_REPLY_STATUS; }

  std::string reply_value_;
};

struct RedisErrorReply : public RedisCmdReply {
public:
  RedisErrorReply() {};
  virtual ~RedisErrorReply() {};
  virtual RedisReplyType reply_type() { return EREDIS_REPLY_ERROR; }

  std::string reply_value_;
};


// Redis accessor
// ---------------------------------------------------------------------------------
class RedisAccessor {
public:
  RedisAccessor();
  ~RedisAccessor();
  bool ConnectServer(std::string host, int port, std::string pswd);
  bool Ping();
  bool DoCommand(RedisCmdReply **out_reply_pt, char * format, ...);

  static void FreeCmdReply(RedisCmdReply* cmd_reply);
  void Close();
  bool SelectDatabase(int db_index);
  bool Reconnect();
protected:
  RedisCmdReply* FetchCmdReply(redisReply *redis_reply);
  
private:
  redisContext* context_;

  std::string host_;
  int port_;
  std::string pswd_;

  int db_index_;
};


}  // namespace tyh


#endif  // __TYH_SERVERBASE_KVDB_REDIS_ACCESSOR_H__