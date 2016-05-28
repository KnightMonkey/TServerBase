/*******************************************************************************
 * Copyright 2016/05 - 2021/07 
 * file	  :
 * author : tianyiheng
 * date   : $time$
 * brief  :
 *******************************************************************************/

#include <stdlib.h>
#include <Windows.h>
#include "char_conv.h"

namespace tyh {

size_t EncodingConv::Utf8ToGb2312(std::string const & in_str, std::string & out_str) {
  wchar_t *src_wch;
  char *des_ch;

  int need_len = MultiByteToWideChar(CP_UTF8, 0, in_str.c_str(), -1, NULL, 0);
  src_wch = new wchar_t[need_len + 1];
  MultiByteToWideChar(CP_UTF8, 0, in_str.c_str(), -1, src_wch, need_len);

  need_len = WideCharToMultiByte(CP_ACP, 0, src_wch, -1, NULL, 0, NULL, NULL);
  des_ch = new char[need_len + 1];
  WideCharToMultiByte(CP_ACP, 0, src_wch, -1, des_ch, need_len, NULL, NULL);

  out_str = des_ch;
  delete[] src_wch;
  delete[] des_ch;
  return out_str.length();
}


size_t EncodingConv::Gb2312ToUtf8(std::string const & in_str, std::string &out_str) {
  wchar_t *wc_tmp;
  char* out_tmp;

  int need_len = MultiByteToWideChar(CP_ACP, 0, in_str.c_str(), -1, NULL, 0);
  wc_tmp = new wchar_t[need_len];
  MultiByteToWideChar(CP_ACP, 0, in_str.c_str(), -1, wc_tmp, need_len);

  need_len = WideCharToMultiByte(CP_UTF8, 0, wc_tmp, -1, NULL, 0, NULL, NULL);
  out_tmp = new char[need_len + 1];
  WideCharToMultiByte(CP_UTF8, 0, wc_tmp, -1, out_tmp, need_len, NULL, NULL);

  return out_str.length();
}

}  // namespace tyh