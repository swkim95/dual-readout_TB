#ifndef TBmid_h
#define TBmid_h 1

#include <vector>

class TBwaveform {
public:
  TBwaveform();
  ~TBwaveform() {}

  void init();
  void finish() { filled_ = true; }
  bool isFilled() const { return filled_; }

  void setChannel(int ch) { channel_ = ch; }
  int channel() const { return channel_; }

  std::vector<short> waveform() const { return waveform_; }

  void fill(unsigned int bin, short val) { waveform_.at(bin) = val; }
private:
  int channel_;
  bool filled_;
  std::vector<short> waveform_;
};

// class TBfastmode { TODO
//
// }

class TBmidbase {
public:
  TBmidbase(int ev, int ru, int mi);
  ~TBmidbase() {}

  int evt() const { return evt_; }
  int run() const { return run_; }
  int mid() const { return mid_; }

  int tcb_trig_type() const { return tcb_trig_type_; }
  int tcb_trig_number() const { return tcb_trig_number_; }
  long long tcb_trig_time() const { return tcb_trig_time_; }
  void setTCB(int ty, int nu, long long ti);

  int local_trig_number() const { return local_trig_number_; }
  int local_trigger_pattern() const { return local_trigger_pattern_; }
  long long local_trig_time() const { return local_trig_time_; }
  void setLocal(int nu, int pa, long long ti);

  int channelsize() const { return channelsize_; }
  void setChannelSize(int chsize) { channelsize_ = chsize; }
private:
  // metadata
  int evt_;
  int run_;
  int mid_;
  int tcb_trig_type_;
  int tcb_trig_number_;
  long long tcb_trig_time_;
  int local_trig_number_;
  int local_trigger_pattern_;
  long long local_trig_time_;
  int channelsize_;
};

template <class T> // waveform or fastmode
class TBmid : public TBmidbase {
public:
  TBmid(int ev, int ru, int mi);
  ~TBmid() {}

  const T channel(unsigned int idx) const { return channels_.at(idx); }
  void setChannels(std::vector<T> ch) { channels_ = ch; }
private:
  std::vector<T> channels_;
};

#endif
