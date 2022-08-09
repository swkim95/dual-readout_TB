#ifndef TBdetector_h
#define TBdetector_h 1

#include <vector>
#include <iostream>

class TBcid {
public:
  TBcid(int midin, int channelin);
  ~TBcid() {}

  bool operator< (const TBcid& rh) const;

  int mid() const { return mid_; }
  int channel() const { return channel_; }

  void print() const;

private:
  int mid_;
  int channel_;
};

class TBdetector {
public:
  enum detid {
    nulldet = -1,
    SiPM = 0,
    PMT,
    MCPPMT, // not used
    DWC1analogue = 10,
    DWC1digital = 12,
    DWC2analogue = 16,
    DWC2digital = 18,
    extTrig1 = 22,
    extTrig2,
    veto,
    preshower,
    tail,
    muon
  };
public:
  TBdetector(detid in);
  ~TBdetector() {}

  detid det() const { return det_; }
  uint64_t id() const { return id_; }

  bool isSiPM() const { return det_==detid::SiPM; }
  bool isModule() const;

  void encodeModule(int mod, int tow, bool isc);
  void encodeSiPM(int plate, int column);
  void encodeColumn(int column);

  int module() const;
  int tower() const;
  bool isCeren() const;
  int plate() const;
  int column() const;
private:
  detid det_;
  uint64_t id_;
};

#endif
