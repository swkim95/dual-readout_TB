#ifndef TButility_h
#define TButility_h 1

#include <vector>
#include <algorithm>
#include <map>

#include "TBdetector.h"

// TODO filename class

class TButility {
public:
  TButility() {}
  ~TButility() {}

  void loading(const std::string& path);
  void loadped(const std::string& path);

  TBdetector::detid detid(int tid) const;
  TBdetector find(const TBcid& cid) const;
  float retrievePed(const TBcid& cid) const;

private:
  std::map<TBcid, TBdetector> mapping_;
  std::map<TBcid, float> pedmap_;
};

#endif
