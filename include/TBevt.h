#ifndef TBevt_h
#define TBevt_h 1

#include <vector>

#include "TBmid.h"

template <class T>
class TBevt {
public:
  TBevt();
  ~TBevt() {}

  void set(std::vector<TBmid<T>> in) { mids_ = in; }
private:
  int evt_;
  std::vector<TBmid<T>> mids_;
};

#endif
