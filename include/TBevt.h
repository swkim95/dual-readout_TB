#ifndef TBevt_h
#define TBevt_h 1

#include <vector>

#include "TBmid.h"
#include "TBdetector.h"

template <class T>
class TBevt {
public:
  TBevt();
  ~TBevt() {}

  void set(std::vector<TBmid<T>> in) { mids_ = in; }
  void setTCB(int tcb) { evt_ = tcb; }

  int tcb() { return evt_; }

  TBmid<T> mid(unsigned idx) const { return mids_.at(idx); }
  int size() const { return static_cast<int>(mids_.size()); }

  const T data(const TBcid& cid) const;

private:
  int evt_;
  std::vector<TBmid<T>> mids_;
};

#endif
