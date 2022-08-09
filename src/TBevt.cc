#include "TBevt.h"

template<typename T>
TBevt<T>::TBevt()
: evt_(0), mids_(0) {}

template class TBevt<TBwaveform>;
