#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>

#include "TBmid.h"
#include "TBread.h"
#include "TBevt.h"

#include "TFile.h"
#include "TTree.h"

int main(void) {
  // get # of events in file
  FILE* fp = fopen("cal_wave_1_117.dat", "rb");
  fseek(fp, 0L, SEEK_END);
  int file_size = ftell(fp);
  fclose(fp);
  int nevt = file_size / 65536;

  std::vector<std::string> filenames {
    "cal_wave_1_117.dat",
    "cal_wave_2_117.dat",
    "cal_wave_3_117.dat",
    "cal_wave_4_117.dat",
    "cal_wave_5_117.dat",
    "cal_wave_6_117.dat",
    "cal_wave_7_117.dat",
    "cal_wave_8_117.dat",
    "cal_wave_9_117.dat",
    "cal_wave_10_117.dat",
    "cal_wave_11_117.dat",
    "cal_wave_12_117.dat",
    "cal_wave_13_117.dat",
    "cal_wave_14_117.dat",
    "cal_wave_15_117.dat"
  };

  std::vector<FILE*> files;
  files.reserve(filenames.size());

  for (unsigned int idx = 0; idx < filenames.size(); idx++)
    files.emplace_back( fopen(filenames.at(idx).c_str(), "rb") );

  TBread reader = TBread();
  auto anevt = TBevt<TBwaveform>();
  TFile* rootfile = TFile::Open("test.root","RECREATE");
  TTree* roottree = new TTree("testtree","testtree");
  roottree->Branch("TBevt",&anevt);

  for (unsigned ievt = 0; ievt < nevt; ievt++) {
    std::vector<TBmid<TBwaveform>> mids;
    mids.reserve(files.size());

    // reference mid
    TBmid<TBwaveform> midref = reader.readWaveform(files.at(0));
    int refevt = midref.evt();
    mids.emplace_back(midref);
    midref.print();

    for (unsigned int idx = 1; idx < files.size(); idx++) {
      TBmid<TBwaveform> amid = reader.readWaveform(files.at(idx));
      amid.print();

      if (amid.evt()!=refevt) // TODO tcb_trig_number difference handling
        throw std::runtime_error("TCB trig numbers are different!");

      mids.emplace_back(amid);
    }

    anevt.set(mids);

    roottree->Fill();
  }

  rootfile->WriteTObject(roottree);
  rootfile->Close();

  return 0;
}
