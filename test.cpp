/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************/

#include <iostream>
#include "tsocket/sock_base.h"
#include "tlog/tboost_log.h"
#include "tpacket/base_pack.h"
#include "tconnection/connection.h"
#include "tsingleton/singleton_template.h"
#include "tbase_server/base_server.h"

using namespace tyh;

#include <iostream>

class CBase{
public:
  CBase(){}
  virtual void done(){
	std::cout << "CBase" << std::endl;
  }
  int num_;
};

class CA :public CBase{
public:
  CA(){}
  virtual void done(){
	std::cout << "CA" << std::endl;
  }
};


int test_main(int argc, char* argv[]) {
  /*
  TBoostLogger::InitLogger("Server");
  LOGGER(INFO) << "hello my server base";
  LOGGER(ERR) << "hello my server base";
  boost::asio::io_service io;
  TSocket sock(io);
  PackBufferCell<NORMAL_PACK_BUFFER_SIZE> pack_buff;
  pack_buff.AppendData("123456", 6);
  LOGGER(INFO) << "buffer_cell length() = " << pack_buff.length();
  TConnection conn(io);


  boost::shared_ptr<CA> ptr_a(new CA());
  boost::shared_ptr<CBase> ptr_b, ptr_c;
  //ptr_b = boost::dynamic_pointer_cast<CBase>(ptr_a);
  ptr_c = ptr_a;
  ptr_b->done();
  ptr_c->done();
  //ptr_c = ptr_a;
  

  std::string host = "192.168.2.56";
  uint32 port = 6089;
  TBaseServer base_server(1);
  base_server.Run();*/
  return 0;
}