#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include <memory>

#include "TBmid.h"
#include "TBread.h"
#include "TBevt.h"
#include "TButility.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

int main(void) {
  // get # of events in file
  FILE* fp = fopen("cal_fast_1_117.dat", "rb");
  fseek(fp, 0L, SEEK_END);
  int file_size = ftell(fp);
  fclose(fp);
  int nevt = file_size / 256;

  std::vector<std::string> filenames {
    "cal_fast_1_117.dat",
    "cal_fast_2_117.dat",
    "cal_fast_3_117.dat",
    "cal_fast_4_117.dat",
    "cal_fast_5_117.dat",
    "cal_fast_6_117.dat",
    "cal_fast_7_117.dat",
    "cal_fast_8_117.dat",
    "cal_fast_9_117.dat",
    "cal_fast_10_117.dat",
    "cal_fast_11_117.dat",
    "cal_fast_12_117.dat",
    "cal_fast_13_117.dat",
    "cal_fast_14_117.dat",
    "cal_fast_15_117.dat"
  };

  std::vector<FILE*> files;
  files.reserve(filenames.size());

  for (unsigned int idx = 0; idx < filenames.size(); idx++)
    files.emplace_back( fopen(filenames.at(idx).c_str(), "rb") );

  TBread reader = TBread();
  auto anevt = TBevt<TBfastmode>();
  TFile* rootfile = TFile::Open("fast.root","RECREATE");
  auto roottree = new TTree("testtree","testtree");
  roottree->Branch("TBevt",&anevt);

  TButility utility = TButility();
  utility.loading("mapping_data_MCPPMT_positiveSignal.csv");

  // TODO define histos
  std::vector<TH1D*> histos;
  histos.reserve(32*15);

  for (unsigned idx = 0; idx < 32*15; idx++) {
    auto ahist = new TH1D(static_cast<TString>("hist")+std::to_string(idx),"ADC",1000,-100000.,100000.);
    histos.emplace_back(ahist);
  }

  for (unsigned ievt = 0; ievt < nevt; ievt++) {
    // fill TBevt
    std::vector<TBmid<TBfastmode>> mids;
    mids.reserve(files.size());

    // reference mid
    TBmid<TBfastmode> midref = reader.readFastmode(files.at(0));
    int refevt = midref.evt();
    mids.emplace_back(midref);
    anevt.setTCB(refevt);
    // midref.print();

    for (unsigned int idx = 1; idx < files.size(); idx++) {
      TBmid<TBfastmode> amid = reader.readFastmode(files.at(idx));
      // amid.print();

      if (amid.evt()!=refevt) // TODO tcb_trig_number difference handling
        throw std::runtime_error("TCB trig numbers are different!");

      mids.emplace_back(amid);
    }

    anevt.set(mids);

    roottree->Fill();

    // analysis example
    for (int idx = 0; idx < anevt.size(); idx++) {
      auto amid = anevt.mid(idx);

      for (int jdx = 0; jdx < amid.channelsize(); jdx++) {
        const auto achannel = amid.channel(jdx);
        const auto cid = TBcid(amid.mid(),achannel.channel());
        int adc = achannel.adc();

        histos.at((amid.mid()-1)*32+(achannel.channel()-1))->Fill(static_cast<float>(adc));
      }

    }

  }

  for (unsigned idx = 0; idx < histos.size(); idx++)
    rootfile->WriteTObject(histos.at(idx));

  rootfile->WriteTObject(roottree);
  rootfile->Close();

  return 0;
}
