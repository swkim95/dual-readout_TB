#include "TBmonit.h"
#include "TBmid.h"
#include "TBevt.h"
#include "TBplot.h"
#include "TBread.h"
#include "TButility.h"

#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>

#include "TFile.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"

TBmonit::TBmonit(const std::string& outputName)
: fastList_(0), waveList_(0), outputName_(outputName) {}

void TBmonit::setFastmodeFiles(const boost::python::list& fastList) {
  for (unsigned idx = 0; idx < boost::python::len(fastList); idx++)
    fastList_.push_back(boost::python::extract<std::string>(fastList[idx]));
}

void TBmonit::setWaveformFiles(const boost::python::list& waveList) {
  for (unsigned idx = 0; idx < boost::python::len(waveList); idx++)
    waveList_.push_back(boost::python::extract<std::string>(waveList[idx]));
}

void TBmonit::padSet(TPad* tPad, double margin) {
  tPad->Draw();
  tPad->cd();

  tPad->SetTopMargin(margin);
  tPad->SetLeftMargin(margin);
  tPad->SetRightMargin(margin);
  tPad->SetBottomMargin(margin);
}

void TBmonit::MonitPlots(int max_evt) {
  TButility utility = TButility();
  utility.loading(mappingpath_);
  utility.loadped(pedestalpath_);

  FILE* fp = fopen((waveList_.at(0)).c_str(), "rb");
  fseek(fp, 0L, SEEK_END);
  long long file_size = ftell(fp);
  printf("file_size %lld\n",file_size);
  fclose(fp);
  int nevt_wave = file_size / 65536;

  std::vector<FILE*> filesWave;
  filesWave.reserve(waveList_.size());

  for (unsigned int idx = 0; idx < waveList_.size(); idx++)
    filesWave.emplace_back( fopen(waveList_.at(idx).c_str(), "rb") );

  fp = fopen((fastList_.at(0)).c_str(), "rb");
  fseek(fp, 0L, SEEK_END);
  file_size = ftell(fp);
  fclose(fp);
  int nevt_fast = file_size / 256;

  if(max_evt!=-1) {
    nevt_wave=max_evt;
    nevt_fast=max_evt;
  }

  printf("FAST : %d | WAVE : %d\n", nevt_fast, nevt_wave);

  std::vector<FILE*> filesFast;
  filesFast.reserve(fastList_.size());

  for (unsigned int idx = 0; idx < fastList_.size(); idx++)
    filesFast.emplace_back( fopen(fastList_.at(idx).c_str(), "rb") );

  TBread readerWave = TBread();
  auto anevtWave = TBevt<TBwaveform>();
  TBread readerFast = TBread();
  auto anevtFast = TBevt<TBfastmode>();

  TH2D* comparisonWAVEvsFAST = new TH2D("comparisonWAVEvsFAST", "Wave vs Fast;Wave [ADC];Fast [ADC]", 2*DistBinADC_, -DistMaxADC_, DistMaxADC_, 3*DistBinADC_, -DistMaxADC_, 2*DistMaxADC_);
  comparisonWAVEvsFAST->SetStats(0);

  auto HitMapAccuWave = new TBplot(1000, 1000, 4096., 200, "HitMapAccuWave", TBplotbase::kind::hitmap);
  auto HitMapSingWave = new TBplot(1000, 1000, 2000., 200, "HitMapSingWave", TBplotbase::kind::hitmap);
  auto disMapAccuWave = new TBplot(1000, 500, DistMaxADC_, DistBinADC_, "disMapAccuWave", TBplotbase::kind::distribution);
  auto wavMapWave = new TBplot(1000, 500, DistMaxADC_, DistBinADC_, "wavMapWave", TBplotbase::kind::waveform);
  auto wavSingWave = new TBplot(1000, 500, DistMaxADC_, DistBinADC_, "wavSingWave", TBplotbase::kind::waveform);
  auto disMapAccuFast = new TBplot(1000, 500, DistMaxADC_, DistBinADC_, "disMapAccuFast", TBplotbase::kind::distribution);
  auto HitMapAccuFast = new TBplot(1000, 1000, 100000., 1000., "HitMapAccuFast", TBplotbase::kind::hitmap);
  auto DWCfast = new TBplot(1000, 1000, DistMaxADC_, DistBinADC_, "DWCfast", TBplotbase::kind::dwc);
  auto AuxFast = new TBplot(1000, 330, 100000., 1000., "AuxFast", TBplotbase::kind::auxiliary);
  auto AuxWave = new TBplot(1000, 330, DistMaxADC_, DistBinADC_, "AuxWave", TBplotbase::kind::auxiliary);

  std::vector<float> FastXaxis;
  std::vector<long long> FastYaxis;
  std::vector<float> FastYaxis_;
  std::vector<float> WaveXaxis;
  std::vector<long long> WaveYaxis;
  std::vector<float> WaveYaxis_;

  std::chrono::time_point time_begin = std::chrono::system_clock::now(); //delete
  printf("\n");

  for (unsigned ievt = 0; ievt < nevt_wave; ievt++) {
    if (ievt>0&&ievt%8==0) {
      std::chrono::duration time_taken = std::chrono::system_clock::now()-time_begin; //delete
      float percent_done=1.*ievt/nevt_wave;
      std::chrono::duration time_left = time_taken * (1/percent_done - 1);
      std::chrono::minutes minutes_left = std::chrono::duration_cast<std::chrono::minutes>(time_left);
      std::chrono::seconds seconds_left = std::chrono::duration_cast<std::chrono::seconds>(time_left - minutes_left);
      std::cout << "\r\033[F"<< " " << ievt << " events  "<< minutes_left.count() << ":";
      printf("%02d left   %.1f%%            \n",int(seconds_left.count()),percent_done*100);
    }

    std::vector<TBmid<TBwaveform>> midsWave;
    midsWave.reserve(filesWave.size());

    TBmid<TBwaveform> midrefWave = readerWave.readWaveform(filesWave.at(0));
    int refevt_wave = midrefWave.evt();

    midsWave.emplace_back(midrefWave);
    anevtWave.setTCB(refevt_wave);

    for (unsigned int idx = 1; idx < filesWave.size(); idx++) {
      TBmid<TBwaveform> amid = readerWave.readWaveform(filesWave.at(idx));
      if (amid.evt()!=refevt_wave)
        throw std::runtime_error("TCB trig numbers are different! [Waveform]");

      midsWave.emplace_back(amid);
    }

    float sipmCadc = 0;
    float sipmSadc = 0;

    for (int idx = 0; idx < midsWave.size(); idx++) {
      auto amid = midsWave.at(idx);

      for (int jdx = 0; jdx < amid.channelsize(); jdx++) {
        const auto achannel = amid.channel(jdx);
        const auto cid = TBcid(amid.mid(),achannel.channel());

        auto awave = achannel.waveform();

        float ped = 0;

        for (int tdx = 0; tdx < 100; tdx++)
          ped+=(float)awave.at(tdx+1)/100.;

        auto pedwave = achannel.pedcorrectedWaveform(ped);
        float adc = achannel.pedcorrectedADC(ped);

        float peak = *std::max_element(pedwave.begin()+1, pedwave.end()-23);

        auto tmpId = utility.find(cid);

        HitMapAccuWave->fillADC(tmpId, peak/(float)nevt_wave);
        disMapAccuWave->fillADC(tmpId, peak);
        HitMapSingWave->fillADC(tmpId, peak);

        if (tmpId.det() == TBdetector::detid::preshower || tmpId.det() == TBdetector::detid::tail || tmpId.det() == TBdetector::detid::muon)
          AuxWave->fillAux(tmpId, peak);

        if (tmpId.det() == TBdetector::detid::preshower || tmpId.det() == TBdetector::detid::tail || tmpId.det() == TBdetector::detid::muon)
          AuxWave->fillAux(tmpId, adc);

        if (tmpId.module() == 2 && tmpId.tower() == 5) {
          if (tmpId.isCeren())
            sipmCadc += peak;
          else
            sipmSadc += peak;
        }

        wavSingWave->fillWaveform(tmpId, achannel.waveform());

        if ( ievt == 0 )
          wavMapWave->fillWaveform(tmpId, achannel.waveform());
      }
    }

    disMapAccuWave->aPlot1D(8)->Fill(sipmCadc);
    disMapAccuWave->aPlot1D(21)->Fill(sipmSadc);

  }

  printf("\n");

  for (unsigned ievt = 0; ievt < nevt_fast; ievt++) {
    std::vector<TBmid<TBfastmode>> midsFast;
    midsFast.reserve(filesFast.size());

    TBmid<TBfastmode> midrefFast = readerFast.readFastmode(filesFast.at(0));
    int refevt_fast = midrefFast.evt();

    midsFast.emplace_back(midrefFast);
    anevtFast.setTCB(refevt_fast);

    for (unsigned int idx = 1; idx < filesFast.size(); idx++) {
      TBmid<TBfastmode> amid = readerFast.readFastmode(filesFast.at(idx));
      if (amid.evt()!=refevt_fast)
        throw std::runtime_error("TCB trig numbers are different! [Waveform]");

      midsFast.emplace_back(amid);
    }

    anevtFast.set(midsFast);

    float sipmCadc = 0;
    float sipmSadc = 0;
    std::vector<int> dwcTimingFast;

    for (int idx = 0; idx < anevtFast.size(); idx++) {
      auto amid = anevtFast.mid(idx);

      for (int jdx = 0; jdx < amid.channelsize(); jdx++) {
        const auto achannel = amid.channel(jdx);
        const auto cid = TBcid(amid.mid(),achannel.channel());

        auto tmpId = utility.find(cid);

        float adc = achannel.adc();
        HitMapAccuFast->fillADC(utility.find(cid), adc/(float)nevt_fast);
        disMapAccuFast->fillADC(utility.find(cid), adc);

        if (tmpId.det() == TBdetector::detid::preshower || tmpId.det() == TBdetector::detid::tail || tmpId.det() == TBdetector::detid::muon)
          AuxFast->fillAux(tmpId, adc);

        if (tmpId.module() == 2 && tmpId.tower() == 5) {
          if (tmpId.isCeren())
            sipmCadc += adc;
          else
            sipmSadc += adc;
        } else if (!tmpId.isModule() && (tmpId.det() == TBdetector::detid::DWC1digital || tmpId.det() == TBdetector::detid::DWC2digital)) {
          dwcTimingFast.push_back(achannel.timing());
        }
      }
    }

    disMapAccuFast->aPlot1D(8)->Fill(sipmCadc);
    disMapAccuFast->aPlot1D(21)->Fill(sipmSadc);
    DWCfast->fillDWC(dwcTimingFast);
  }

  TFile* plotRootFile = new TFile(outputName_.c_str(), "RECREATE");

  for (int i = 0; i < HitMapAccuWave->getPlotSize2D(); i++)
    plotRootFile->WriteTObject(HitMapAccuWave->aPlot2D(i));

  for (int i = 0; i < disMapAccuWave->getPlotSize1D(); i++)
    plotRootFile->WriteTObject(disMapAccuWave->aPlot1D(i));

  for (int i = 0; i < wavMapWave->getPlotSize1D(); i++)
    plotRootFile->WriteTObject(wavMapWave->aPlot1D(i));

  for (int i = 0; i < HitMapAccuFast->getPlotSize2D(); i++)
    plotRootFile->WriteTObject(HitMapAccuFast->aPlot2D(i));

  for (int i = 0; i < disMapAccuFast->getPlotSize1D(); i++)
    plotRootFile->WriteTObject(disMapAccuFast->aPlot1D(i));

  for (int i = 0; i < DWCfast->getPlotSize2D(); i++)
    plotRootFile->WriteTObject(DWCfast->aPlot2D(i));

  for (int i = 0; i < AuxFast->getPlotSize1D(); i++)
    plotRootFile->WriteTObject(AuxFast->aPlot1D(i));

  for (int i = 0; i < AuxWave->getPlotSize1D(); i++)
    plotRootFile->WriteTObject(AuxWave->aPlot1D(i));

  for (int i = 0; i < DWCfast->getPlotSize2D(); i++)
    plotRootFile->WriteTObject(DWCfast->aPlot2D(i));

  for (int i = 0; i < AuxFast->getPlotSize1D(); i++)
    plotRootFile->WriteTObject(AuxFast->aPlot1D(i));

  for (int i = 0; i < AuxWave->getPlotSize1D(); i++)
    plotRootFile->WriteTObject(AuxWave->aPlot1D(i));



  plotRootFile->Close();
}
