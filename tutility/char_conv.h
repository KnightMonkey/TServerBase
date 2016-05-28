/*******************************************************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************************************************/

#ifndef __TYH_UTINITY_CHAR_CONV_H__
#define __TYH_UTINITY_CHAR_CONV_H__

#include <string>

namespace tyh {

class EncodingConv {
protected:
  EncodingConv() {}
  ~EncodingConv() {}
public:
  static size_t Utf8ToGb2312(std::string const & in_str, std::string & out_str);
  static size_t Gb2312ToUtf8(std::string const & in_str, std::string &out_str);
};

}  // namespace tyh



#endif  // __TYH_UTINITY_CHAR_CONV_H__