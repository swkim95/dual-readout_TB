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
  auto roottree = new TTree("testtree","testtree");
  roottree->Branch("TBevt",&anevt);

  TButility utility = TButility();
  utility.loading("mapping_data_MCPPMT_positiveSignal.csv");
  utility.loadped("sample_ped.csv");

  // TODO define histos
  std::vector<TH1D*> histos;
  histos.reserve(32*15);

  std::vector<TH1D*> histowaves;
  histowaves.reserve(32*15);

  for (unsigned idx = 0; idx < 32*15; idx++) {
    auto ahist = new TH1D(static_cast<TString>("hist")+std::to_string(idx),"ADC",1000,-1000000.,1000000.);
    histos.emplace_back(ahist);
  }

  for (unsigned idx = 0; idx < 32*15; idx++) {
    auto ahist = new TH1D(static_cast<TString>("wave")+std::to_string(idx),"wave",1024,0.,1024.);
    histowaves.emplace_back(ahist);
  }

  for (unsigned ievt = 0; ievt < nevt; ievt++) {
    // fill TBevt
    std::vector<TBmid<TBwaveform>> mids;
    mids.reserve(files.size());

    // reference mid
    TBmid<TBwaveform> midref = reader.readWaveform(files.at(0));
    int refevt = midref.evt();
    mids.emplace_back(midref);
    anevt.setTCB(refevt);
    // midref.print();

    for (unsigned int idx = 1; idx < files.size(); idx++) {
      TBmid<TBwaveform> amid = reader.readWaveform(files.at(idx));
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
        float ped = utility.retrievePed(cid);
        float adc = achannel.pedcorrectedADC(ped);

        histos.at((amid.mid()-1)*32+(achannel.channel()-1))->Fill(adc);

        if (ievt==1) {
          auto awave = achannel.waveform();
          for (unsigned kdx = 0; kdx < awave.size(); kdx++)
            histowaves.at((amid.mid()-1)*32+(achannel.channel()-1))->SetBinContent(kdx+1,awave.at(kdx));
        }

      }

    }

  }

  for (unsigned idx = 0; idx < histos.size(); idx++)
    rootfile->WriteTObject(histos.at(idx));

  for (unsigned idx = 0; idx < histowaves.size(); idx++)
    rootfile->WriteTObject(histowaves.at(idx));

  rootfile->WriteTObject(roottree);
  rootfile->Close();

  return 0;
}
