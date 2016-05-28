/*
*	Copyright(c) 2016 - 2020 tianyiheng
*	autor : tianyiheng
*	date : 2016 - 05 - 05
*	brief :
*/

#ifndef __TYH_NOCOPY_ABLE_H__
#define __TYH_NOCOPY_ABLE_H__

namespace tyh{

class Nocopyable {
protected:
  Nocopyable(){}
  ~Nocopyable(){}

private:
  Nocopyable(const Nocopyable & r) {}
  const Nocopyable& operator = (const Nocopyable & r){
  }
};
}
#endif