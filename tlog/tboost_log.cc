/*=============================================================================
#    Copyright (c) 2015
#    ShanghaiKunyan.  All rights reserved
#
#    Filename     : /home/tianyiheng_test/testproj/boost/boost_log/MyBoostLog/tboost_log.cc
#    Author       : tianyiheng
#    Email        : tianyiheng@kunyan-inc.com 
#    Date         : 2016-04-26 10:08
#    Description  : 
=============================================================================*/
#include "tboost_log.h"
#include <ctime>

// -------------------------------------------------------
// the color of log info which will output console
enum ColorType {
  COLOR_WHITE,
  COLOR_YELLOW,
  COLOR_GREEN,
  COLOR_PURPLE,
  COLOR_RED
};

static const char* k_color[] = {
  "\033[1;37m",
  "\033[1;33m",
  "\033[1;32m",
  "\033[1;35m",
  "\033[1;31m" 
};
// -------------------------------------------------------

class LoggerInit{
 public:
  LoggerInit() {}
  ~LoggerInit() {}
 friend class TBoostLogger;
 protected:
  void RegisterConsoleSink();
  void RegisterFileSink();
 
 private:
  static void AddCommAttr();
  static void TConsoleFormatter(logging::record_view const & rec, 
      logging::formatting_ostream &strm);
  static std::string CurrenTime();

  static void TFileFormatter(logging::record_view const & rec, 
      logging::formatting_ostream &strm);
};

std::string LoggerInit::CurrenTime() {
  std::stringstream timestramp;
  time_t now_time_t = time(NULL);
  std::tm now_tm;

#ifndef WIN32
  localtime_r(&now_time_t, &now_tm);
#else
  localtime_s(&now_tm, &now_time_t);
#endif

  timestramp << (now_tm.tm_year+1900) << "/" << (now_tm.tm_mon+1) << "/" \
             << now_tm.tm_mday << " " << now_tm.tm_hour << ":" \
             << now_tm.tm_min << ":" << now_tm.tm_sec; 
  return timestramp.str();
}

// the operator "<<" is used for regular stream formatting
std::ostream & operator << (std::ostream & strm, LogLevel level) {
  static const char * s_levle_info[] = {
   "INFO", "DEBUG", "NOTIFY", "WARN", "ERROR"  
  };
  static size_t info_size = sizeof(s_levle_info)/sizeof(const char *); 
  if (static_cast<size_t>(level) < info_size) {
    return strm << s_levle_info[level];  
  } else {
    return strm << static_cast<int>(level);
  }
    
  return strm;
}

// add common attributes
void LoggerInit::AddCommAttr() {
  boost::shared_ptr<logging::core> log_core_ptr = logging::core::get();
  log_core_ptr->add_global_attribute("LineID", attrs::counter<unsigned long>(1)); 
  log_core_ptr->add_global_attribute("ProcName", attrs::constant<std::string>("MyProc")); 
   
}

void LoggerInit::TConsoleFormatter(logging::record_view const & rec, 
    logging::formatting_ostream &strm){
  strm << logging::extract<unsigned long>("LineID", rec) << ":";
  // log level
  logging::value_ref<LogLevel, tag::severity> level = rec[severity];

#ifndef WIN32
  strm << k_color[level.get<LogLevel>()];
#endif

  //strm << logging::extract<std::string>("ProcName", rec) << ":";
  strm << "[" << level << "]";
  // time stamp
  std::string timestamp = CurrenTime();
  strm << "[" << timestamp << "]";

  // message
  strm << rec[expr::smessage]; 
#ifndef WIN32
  strm << "\033[0m";
#endif
} 

void LoggerInit::TFileFormatter(logging::record_view const & rec, 
      logging::formatting_ostream &strm) {
  //strm << logging::extract<std::string>("ProcName", rec) << ":";
  strm << logging::extract<unsigned long>("LineID", rec) << ":";
  // log level
  logging::value_ref<LogLevel, tag::severity> level = rec[severity];
  strm << "[" << level << "]";
  // time stamp
  std::string timestamp = CurrenTime();
  strm << "[" << timestamp << "]";
  // message
  strm << rec[expr::smessage]; 
}

void LoggerInit::RegisterConsoleSink() {
  try {
    typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;
    boost::shared_ptr<text_sink> console_sink(new text_sink);
    boost::shared_ptr<std::ostream> console_stream(&std::clog, boost::null_deleter());
    console_sink->locked_backend()->add_stream(console_stream);
    // set formmater 
    console_sink->set_formatter(&LoggerInit::TConsoleFormatter);
    logging::core::get()->add_sink(console_sink); 
  }
  catch (std::exception &e){
    std::cerr << e.what() << std::endl;
  }
}

void LoggerInit::RegisterFileSink() {
  try {
    typedef sinks::synchronous_sink<sinks::text_file_backend> text_sink;
    boost::shared_ptr<text_sink> sink(new text_sink(
           keywords::file_name = "lg%Y%m%d_%5N.log",
           keywords::rotation_size = 3*1024*1024  
          ));
    sink->locked_backend()->set_file_collector(sinks::file::make_collector(
          keywords::target = "./logs",
          keywords::max_size = 16*1024*1024,
          keywords::min_free_space = 48*1024*1024
          ));  
    sink->locked_backend()->scan_for_files();
    sink->set_formatter(&LoggerInit::TFileFormatter); 
    sink->set_filter(severity >= DEBUG);

    logging::core::get()->add_sink(sink); 
  }
  catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

// --------------------------------------------------------
void TBoostLogger::InitLogger(std::string program_name) {
  static unsigned s_init_cout = 0;
  if (0 == s_init_cout) {
    LoggerInit::AddCommAttr();
    static LoggerInit init_logger;
    init_logger.RegisterConsoleSink();
    init_logger.RegisterFileSink();
    s_init_cout++;
  }
}




