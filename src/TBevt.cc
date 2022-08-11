#include "TBevt.h"

template<typename T>
TBevt<T>::TBevt()
: evt_(0), mids_(0) {}

template<typename T>
const T TBevt<T>::data(const TBcid& cid) const {
  T adata = T();

  for (const TBmid<T>& amid : mids_) {
    if (amid.mid()==cid.mid()) {
      adata = amid.channel(cid.channel()-1); // WARNING channel number 1 - 32
      break;
    }
  }

  return adata;
}

template class TBevt<TBwaveform>;
template class TBevt<TBfastmode>;
