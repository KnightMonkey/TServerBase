#include <iostream>
#include "tsocket/sock_base.h"
#include "tlog/tboost_log.h"
#include "tpacket/base_pack.h"
#include "tconnection/connection.h"
#include "tsingleton/singleton_template.h"
#include "tbase_server/base_server.h"
#include "MyServer.h"
#include "tconfig/config_manager.h"
#include "tkvdb/redis_accessor.h"
#include "tutility/char_conv.h"

using namespace tyh;

#include <iostream>

int main(int argc, char* argv[]) {

  /*std::string host = "192.168.2.56";
  uint32 port = 6089;*/
  //TBaseServer base_server(1, host, port);
  //base_server.Run();

  // test redis
  tyh::RedisAccessor redis_client;
  if (redis_client.ConnectServer("222.73.57.12", 6379, "kunyan")) {
	tyh::RedisCmdReply *cmd_reply = NULL;
	redis_client.DoCommand(&cmd_reply, "hget News_20160523 %s", "082335_d4b89b06637817c4261e8b2ef41ee0de");
	printf("reply type = %d\n", cmd_reply->reply_type());
	tyh::RedisStringReply *str_reply = dynamic_cast<tyh::RedisStringReply *>(cmd_reply);
	std::string utf8_str;
	tyh::EncodingConv::Utf8ToGb2312(str_reply->reply_value_, utf8_str);
	printf("reply result:%s\n", utf8_str.c_str());
	tyh::RedisAccessor::FreeCmdReply(cmd_reply);
	// ping
	if (redis_client.Ping()) {
	  printf("redis ping 正常\n");
	}
	// select db
	if (redis_client.SelectDatabase(0)) {
	  printf("reids select db 正常\n");
	}
  }
  else {
	printf("error,连接服务器失败\n");
  }
  // test redis

  ////////////////////////////////////////////////

  LOGGER(INFO) << "start load config ....";
  ConfMgr->Init("./config_file/server_config.txt");
  LOGGER(INFO) << "end load config ....";

  size_t io_service_size = ConfMgr->server_base_config().io_service_size;
  
  MyServer my_server(io_service_size/*, host, port*/);
  my_server.Run();
  return 0;
}