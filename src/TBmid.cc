#include "TBmid.h"

TBwaveform::TBwaveform()
: channel_(-1), filled_(false), waveform_(0) {}

void TBwaveform::init() {
  waveform_.resize(1024);
  std::fill(waveform_.begin(), waveform_.end(), 0);
}

TBmidbase::TBmidbase(int ev, int ru, int mi)
: evt_(ev), run_(ru), mid_(mi),
  tcb_trig_type_(0),
  tcb_trig_number_(0),
  tcb_trig_time_(0),
  local_trig_number_(0),
  local_trigger_pattern_(0),
  local_trig_time_(0),
  channelsize_(0) {}

void TBmidbase::setTCB(int ty, int nu, long long ti) {
  tcb_trig_type_ = ty;
  tcb_trig_number_ = nu;
  tcb_trig_time_ = ti;
}

void TBmidbase::setLocal(int nu, int pa, long long ti) {
  local_trig_number_ = nu;
  local_trigger_pattern_ = pa;
  local_trig_time_ = ti;
}

template<typename T>
TBmid<T>::TBmid(int ev, int ru, int mi)
: TBmidbase(ev,ru,mi), channels_(0) {}

template class TBmid<TBwaveform>;
