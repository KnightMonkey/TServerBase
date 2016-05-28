/*=============================================================================
#    Copyright (c) 2015
#    ShanghaiKunyan.  All rights reserved
#
#    Filename     : /home/tianyiheng_test/testproj/boost/boost_log/MyBoostLog/tboost_log.h
#    Author       : tianyiheng
#    Date         : 2016-04-26 09:52
#    Description  : logger tool
=============================================================================*/

#ifndef __HOME_TIANYIHENG_PROJECT_BOOST_LOG_H__
#define __HOME_TIANYIHENG_PROJECT_BOOST_LOG_H__
// std
#include <string>
#include <iostream>
// boost
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions/attr_fwd.hpp>
#include <boost/log/expressions/attr.hpp>
#include <boost/log/expressions/keyword_fwd.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/sources/global_logger_storage.hpp>

// namespace alias
namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

// define log level
enum LogLevel{
  INFO,
  DEBUG,
  NOTIFY,
  WARNNIG,
  ERR
};

// declaration log keyword
BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned long long)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", LogLevel)

// global logger
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(MyLogger, src::severity_logger_mt<LogLevel>)

// interface for init logger
class TBoostLogger {
 public:     
  static void InitLogger(std::string program_name); 
};

// define use macro
#define LOGGER(LOG_LEVEL) BOOST_LOG_SEV(MyLogger::get(), LOG_LEVEL) 
#endif
