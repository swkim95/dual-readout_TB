#ifndef TBread_h
#define TBread_h 1

#include "TBmid.h"
#include <string>

class TBread {
public:
  TBread() {}
  ~TBread() {}

  TBmid<TBwaveform> readWaveform(FILE* fp);
  TBmid<TBfastmode> readFastmode(FILE* fp);
private:
  TBmidbase readMetadata(FILE* fp);
};

#endif
