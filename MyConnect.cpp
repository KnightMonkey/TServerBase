/******************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : 2016-05-11
 * brief  :
 *******************************************/

#include "MyConnect.h"

MyConnection::MyConnection(boost::asio::io_service &io_ref) :
tyh::TConnection(io_ref) {
  LOGGER(INFO) << "MyConnection::MyConnection()";
}

MyConnection::~MyConnection() {

}

bool MyConnection::HandleRunState(time_t cur_time) {
  TConnection::HandleRunState(cur_time);
  //LOGGER(INFO) << "MyConnection::HandleRunState";
  return true;
}

bool MyConnection::HandleInitState(time_t cur_time) {
  TConnection::HandleInitState(cur_time);
  LOGGER(INFO) << "MyConnection::HandleInitState" << std::endl;
  return true;
}