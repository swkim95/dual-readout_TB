#include "TButility.h"

#include <iostream>
#include <stdexcept>
#include <fstream>

TBdetector::detid TButility::detid(int tid) const {
  if ( static_cast<int>(TBdetector::detid::DWC1analogue) <= tid &&
       static_cast<int>(TBdetector::detid::DWC1digital) > tid )
    return TBdetector::detid::DWC1analogue;
  if ( static_cast<int>(TBdetector::detid::DWC1digital) <= tid &&
       static_cast<int>(TBdetector::detid::DWC2analogue) > tid )
    return TBdetector::detid::DWC1digital;
  if ( static_cast<int>(TBdetector::detid::DWC2analogue) <= tid &&
       static_cast<int>(TBdetector::detid::DWC2digital) > tid )
    return TBdetector::detid::DWC2analogue;
  if ( static_cast<int>(TBdetector::detid::DWC2digital) <= tid &&
       static_cast<int>(TBdetector::detid::extTrig1) > tid )
    return TBdetector::detid::DWC2digital;
  if ( static_cast<int>(TBdetector::detid::extTrig1) == tid )
    return TBdetector::detid::extTrig1;
  if ( static_cast<int>(TBdetector::detid::extTrig2) == tid )
    return TBdetector::detid::extTrig2;
  if ( static_cast<int>(TBdetector::detid::veto) == tid )
    return TBdetector::detid::veto;
  if ( static_cast<int>(TBdetector::detid::preshower) == tid )
    return TBdetector::detid::preshower;
  if ( static_cast<int>(TBdetector::detid::tail) == tid )
    return TBdetector::detid::tail;
  if ( static_cast<int>(TBdetector::detid::muon) == tid )
    return TBdetector::detid::muon;

  return TBdetector::detid::nulldet;
}

void TButility::loading(const std::string& path) {
  std::ifstream in;
  int mid, ch;
  int module, tower;
  int isCeren, isSiPM;
  int plate, column;

  in.open(path);

  while (true) {
    in >> mid >> ch >> module >> tower >> isCeren >>
    isSiPM >> plate >> column;

    if (!in.good())
      break;

    auto cid = TBcid(mid,ch);

    // check auxiliary
    // hard-coded!!!
    if ( module==3 ) {
      TBdetector::detid adetid = this->detid(tower);
      auto det = TBdetector(adetid);

      // column starts from 0 for all auxiliary detectors
      int col = tower - static_cast<int>(adetid);
      det.encodeColumn(col);

      mapping_.insert(std::make_pair(cid,det));

      continue;
    }

    if ( isSiPM ) {
      auto det = TBdetector(TBdetector::detid::SiPM);
      det.encodeModule(module,tower,static_cast<bool>(isCeren));
      det.encodeSiPM(plate,column);

      mapping_.insert(std::make_pair(cid,det));

      continue;
    }

    // PMT
    // TODO MCPPMT
    auto det = TBdetector(TBdetector::detid::PMT);
    det.encodeModule(module,tower,static_cast<bool>(isCeren));

    mapping_.insert(std::make_pair(cid,det));
  }

  in.close();
}

TBdetector TButility::find(const TBcid& cid) const {
  if (mapping_.find(cid)==mapping_.end())
    return TBdetector(TBdetector::detid::nulldet);

  return mapping_.at(cid);
}

void TButility::loadped(const std::string& path) {
  std::ifstream in;
  in.open(path, std::ios::in);

  int mid, ch;
  float ped;

  while (true) {
    in >> mid >> ch >> ped;

    if (!in.good())
      break;

    auto cid = TBcid(mid,ch);
    pedmap_.insert(std::make_pair(cid,ped));
  }

  in.close();
};

float TButility::retrievePed(const TBcid& cid) const {
  if (pedmap_.find(cid)==pedmap_.end()) {
    cid.print();
    throw std::runtime_error("TButility - cannot find pedestal of the TBcid!");
  }

  return pedmap_.at(cid);
}

TBcid TButility::getcid(int did) const {
  bool found = false;
  for (auto it = mapping_.begin(); it != mapping_.end(); ++it) {
    if(it->second.detType()==did) {
      found = true;
      return it->first;
    }    
  }
  if(!found) 
    throw std::runtime_error("TButility - cannot find detector!");

  TBcid dummy(-1,-1);
  return dummy;
}

int TButility::pid(float psadc, float muadc) const {
  int pid = 0; //nothing
  //hardcoded cuts
  float psc1 = 200, psc2 = 800;
  float muc1 = 200;
  if(psadc>psc1 && psadc<psc2){
    if(muadc<muc1)
      pid = 1; //hadron
    else 
      pid = 2; //mu
  }
  if(psadc>psc2)
    pid = 3; //el

  return pid;
}