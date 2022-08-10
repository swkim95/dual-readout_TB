#include "TBread.h"

TBmid<TBwaveform> TBread::readWaveform(FILE* fp) {
  int channel;
  char data[64];
  short adc[32736];
  int data_length;
  int run_number;
  int tcb_trig_type;
  int tcb_trig_number;
  long long tcb_trig_time;
  int mid;
  int local_trig_number;
  int local_trigger_pattern;
  long long local_trig_time;
  long long diff_time;
  long long fine_time;
  long long coarse_time;
  int itmp;
  long long ltmp;
  int i;
  int cont;

  // read header
  fread(data, 1, 64, fp);

  // data length
  data_length = data[0] & 0xFF;
  itmp = data[1] & 0xFF;
  itmp = itmp << 8;
  data_length = data_length + itmp;
  itmp = data[2] & 0xFF;
  itmp = itmp << 16;
  data_length = data_length + itmp;
  itmp = data[3] & 0xFF;
  itmp = itmp << 24;
  data_length = data_length + itmp;

  // run number
  run_number = data[4] & 0xFF;
  itmp = data[5] & 0xFF;
  itmp = itmp << 8;
  run_number = run_number + itmp;

  // trigger type
  tcb_trig_type = data[6] & 0xFF;

  // TCB trigger #
  tcb_trig_number = data[7] & 0xFF;
  itmp = data[8] & 0xFF;
  itmp = itmp << 8;
  tcb_trig_number = tcb_trig_number + itmp;
  itmp = data[9] & 0xFF;
  itmp = itmp << 16;
  tcb_trig_number = tcb_trig_number + itmp;
  itmp = data[10] & 0xFF;
  itmp = itmp << 24;
  tcb_trig_number = tcb_trig_number + itmp;

  // TCB trigger time
  fine_time = data[11] & 0xFF;
  fine_time = fine_time * 11;     // actually * (1000 / 90)
  coarse_time = data[12] & 0xFF;
  ltmp = data[13] & 0xFF;
  ltmp = ltmp << 8;
  coarse_time = coarse_time + ltmp;
  ltmp = data[14] & 0xFF;
  ltmp = ltmp << 16;
  coarse_time = coarse_time + ltmp;
  ltmp = data[15] & 0xFF;
  ltmp = ltmp << 24;
  coarse_time = coarse_time + ltmp;
  ltmp = data[16] & 0xFF;
  ltmp = ltmp << 32;
  coarse_time = coarse_time + ltmp;
  ltmp = data[17] & 0xFF;
  ltmp = ltmp << 40;
  coarse_time = coarse_time + ltmp;
  coarse_time = coarse_time * 1000;   // get ns
  tcb_trig_time = fine_time + coarse_time;

  // mid
  mid = data[18] & 0xFF;

  // local trigger #
  local_trig_number = data[19] & 0xFF;
  itmp = data[20] & 0xFF;
  itmp = itmp << 8;
  local_trig_number = local_trig_number + itmp;
  itmp = data[21] & 0xFF;
  itmp = itmp << 16;
  local_trig_number = local_trig_number + itmp;
  itmp = data[22] & 0xFF;
  itmp = itmp << 24;
  local_trig_number = local_trig_number + itmp;

  // local trigger #
  local_trigger_pattern = data[23] & 0xFF;
  itmp = data[24] & 0xFF;
  itmp = itmp << 8;
  local_trigger_pattern = local_trigger_pattern + itmp;
  itmp = data[25] & 0xFF;
  itmp = itmp << 16;
  local_trigger_pattern = local_trigger_pattern + itmp;
  itmp = data[26] & 0xFF;
  itmp = itmp << 24;
  local_trigger_pattern = local_trigger_pattern + itmp;

  // local trigger time
  fine_time = data[27] & 0xFF;
  fine_time = fine_time * 11;     // actually * (1000 / 90)
  coarse_time = data[28] & 0xFF;
  ltmp = data[29] & 0xFF;
  ltmp = ltmp << 8;
  coarse_time = coarse_time + ltmp;
  ltmp = data[30] & 0xFF;
  ltmp = ltmp << 16;
  coarse_time = coarse_time + ltmp;
  ltmp = data[31] & 0xFF;
  ltmp = ltmp << 24;
  coarse_time = coarse_time + ltmp;
  ltmp = data[32] & 0xFF;
  ltmp = ltmp << 32;
  coarse_time = coarse_time + ltmp;
  ltmp = data[33] & 0xFF;
  ltmp = ltmp << 40;
  coarse_time = coarse_time + ltmp;
  coarse_time = coarse_time * 1000;   // get ns
  local_trig_time = fine_time + coarse_time;

  // read waveform
  fread(adc, 2, 32736, fp);

  unsigned channelsize = 32;

  auto amid = TBmid<TBwaveform>(tcb_trig_number,run_number,mid);
  amid.setTCB(tcb_trig_type,tcb_trig_number,tcb_trig_time);
  amid.setLocal(local_trig_number,local_trigger_pattern,local_trig_time);

  std::vector<TBwaveform> waveforms;
  waveforms.reserve(channelsize);

  for (unsigned int idx = 0; idx < channelsize; idx++) {
    auto awave = TBwaveform();
    awave.setChannel(idx+1); // WARNING channel number 1 - 32
    awave.init();
    waveforms.emplace_back(awave);
  }

  // fill waveform for channel
  for (i = 0; i < 1023; i++) {
    for (unsigned int idx = 0; idx < channelsize; idx++)
      waveforms.at(idx).fill(i,adc[i*32+idx]); // should be always 32 here
  }

  amid.setChannels(waveforms);

  return std::move(amid);
}
