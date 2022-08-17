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

#include "TFile.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"

TBmonit::TBmonit()
: fastList_(0), waveList_(0), outputName_("output.root") {}

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

void TBmonit::MonitPlots() {

  TButility utility = TButility();
  utility.loading(mappingpath_);
  utility.loadped(pedestalpath_);

  FILE* fp = fopen((waveList_.at(0)).c_str(), "rb");
  fseek(fp, 0L, SEEK_END);
  int file_size = ftell(fp);
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

  auto HitMapAccuWave = new TBplot(1000, 1000, DistMaxADC_, DistBinADC_, "HitMapAccuWave", TBplotbase::kind::hitmap);
  auto disMapAccuWave = new TBplot(1000, 500, DistMaxADC_, DistBinADC_, "disMapAccuWave", TBplotbase::kind::distribution);
  auto wavMapWave = new TBplot(1000, 500, DistMaxADC_, DistBinADC_, "wavMapWave", TBplotbase::kind::waveform);
  auto disMapAccuFast = new TBplot(1000, 500, DistMaxADC_, DistBinADC_, "disMapAccuFast", TBplotbase::kind::distribution);
  auto HitMapAccuFast = new TBplot(1000, 1000, DistMaxADC_, DistBinADC_, "HitMapAccuFast", TBplotbase::kind::hitmap);

  std::vector<float> FastXaxis;
  std::vector<long long> FastYaxis;
  std::vector<float> FastYaxis_;
  std::vector<float> WaveXaxis;
  std::vector<long long> WaveYaxis;
  std::vector<float> WaveYaxis_;

  for (unsigned ievt = 0; ievt < nevt_wave; ievt++) {
    std::vector<TBmid<TBwaveform>> midsWave;
    midsWave.reserve(filesWave.size());

    TBmid<TBwaveform> midrefWave = readerWave.readWaveform(filesWave.at(0));
    int refevt_wave = midrefWave.evt();

    midsWave.emplace_back(midrefWave);
    anevtWave.setTCB(refevt_wave);
    // WaveXaxis.push_back(refevt_wave);
    // WaveYaxis.push_back(midrefWave.tcb_trig_time()/1000000.);

    for (unsigned int idx = 1; idx < filesWave.size(); idx++) {
      TBmid<TBwaveform> amid = readerWave.readWaveform(filesWave.at(idx));
      if (amid.evt()!=refevt_wave)
        throw std::runtime_error("TCB trig numbers are different! [Waveform]");

      midsWave.emplace_back(amid);
    }

    anevtWave.set(midsWave);

    float sipmCadc = 0;
    float sipmSadc = 0;
    for (int idx = 0; idx < anevtWave.size(); idx++) {
      auto amid = anevtWave.mid(idx);

      for (int jdx = 0; jdx < amid.channelsize(); jdx++) {
        const auto achannel = amid.channel(jdx);
        const auto cid = TBcid(amid.mid(),achannel.channel());

        float ped = utility.retrievePed(cid);
        float adc = achannel.pedcorrectedADC(ped);

        auto tmpId = utility.find(cid);

        HitMapAccuWave->fillADC(tmpId, adc/(float)nevt_wave);
        disMapAccuWave->fillADC(tmpId, adc);

        if (tmpId.module() == 2 && tmpId.tower() == 5) {
          if (tmpId.isCeren()) {
            sipmCadc += adc;
          } else {
            sipmSadc += adc;
          }
        }

        if ( ievt == 0 )
          wavMapWave->fillWaveform(utility.find(cid), achannel.waveform());
      }
    }

    disMapAccuWave->aPlot1D(8)->Fill(sipmCadc);
    disMapAccuWave->aPlot1D(21)->Fill(sipmSadc);

  }

  for (unsigned ievt = 0; ievt < nevt_fast; ievt++) {
    std::vector<TBmid<TBfastmode>> midsFast;
    midsFast.reserve(filesFast.size());
  
    TBmid<TBfastmode> midrefFast = readerFast.readFastmode(filesFast.at(0));
    int refevt_fast = midrefFast.evt();
  
    midsFast.emplace_back(midrefFast);
    anevtFast.setTCB(refevt_fast);
    // FastXaxis.push_back(refevt_fast);
    // FastYaxis.push_back(midrefFast.tcb_trig_time()/1000000.);
  
    for (unsigned int idx = 1; idx < filesFast.size(); idx++) {
      TBmid<TBfastmode> amid = readerFast.readFastmode(filesFast.at(idx));
      if (amid.evt()!=refevt_fast)
        throw std::runtime_error("TCB trig numbers are different! [Waveform]");
  
      midsFast.emplace_back(amid);
    }
  
    anevtFast.set(midsFast);

    float sipmCadc = 0;
    float sipmSadc = 0;
    for (int idx = 0; idx < anevtFast.size(); idx++) {
      auto amid = anevtFast.mid(idx);
  
      for (int jdx = 0; jdx < amid.channelsize(); jdx++) {
        const auto achannel = amid.channel(jdx);
        const auto cid = TBcid(amid.mid(),achannel.channel());

        auto tmpId = utility.find(cid);
  
        float adc = achannel.adc();
        HitMapAccuFast->fillADC(utility.find(cid), adc/(float)nevt_fast);
        disMapAccuFast->fillADC(utility.find(cid), adc);

        if (tmpId.module() == 2 && tmpId.tower() == 5) {
          if (tmpId.isCeren()) {
            sipmCadc += adc;
          } else {
            sipmSadc += adc;
          }
        }
      }
    }

    disMapAccuFast->aPlot1D(8)->Fill(sipmCadc);
    disMapAccuFast->aPlot1D(21)->Fill(sipmSadc);
  }
    



  //   while (false) { // TODO error at std::runtime_error("TCB trig numbers are different! [Waveform]")
  //     if( refevt_fast == nevt_fast -1 )
  //       break;
    
  //     std::vector<TBmid<TBfastmode>> midsFast;
  //     midsFast.reserve(filesFast.size());
    
  //     TBmid<TBfastmode> midrefFast = readerFast.readFastmode(filesFast.at(0));
  //     int refevt_fast = midrefFast.evt();
    
  //     midsFast.emplace_back(midrefFast);
  //     anevtFast.setTCB(refevt_fast);
  //     FastXaxis.push_back(refevt_fast);
  //     FastYaxis.push_back(midrefFast.tcb_trig_time()/1000000.);
    
  //     for (unsigned int idx = 1; idx < filesFast.size(); idx++) {
  //       TBmid<TBfastmode> amid = readerFast.readFastmode(filesFast.at(idx));
  //       if (amid.evt()!=refevt_fast)
  //         throw std::runtime_error("TCB trig numbers are different! [Waveform]");
    
  //       midsFast.emplace_back(amid);
  //     }
    
  //     anevtFast.set(midsFast);
    
  //     for (int idx = 0; idx < anevtFast.size(); idx++) {
  //       auto amid = anevtFast.mid(idx);
    
  //       for (int jdx = 0; jdx < amid.channelsize(); jdx++) {
  //         const auto achannel = amid.channel(jdx);
  //         const auto cid = TBcid(amid.mid(),achannel.channel());
    
  //         float adc = achannel.adc();
  //         HitMapAccuFast->fillADC(utility.find(cid), adc/(float)nevt_fast);
  //         disMapAccuFast->fillADC(utility.find(cid), adc);
  //       }
  //     }
    
  //     if (refevt_fast == refevt_wave)
  //       break;
  //   }
	  
  //   for (unsigned int idx = 0; idx < fastList_.size(); idx++) {
  //     auto aMidWave = anevtWave.mid(idx);
  //     auto aMidFast = anevtFast.mid(idx);
    
  //     for (unsigned int jdx = 0; jdx < 32; jdx++) {
  //       const auto aChWave = aMidWave.channel(jdx);
  //       const auto cidWave = TBcid(aMidWave.mid(),aChWave.channel());
  //       float pedWave = utility.retrievePed(cidWave);
  //       float adcWave = aChWave.pedcorrectedADC(pedWave);
    
  //       const auto aChFast = aMidFast.channel(jdx);
  //       const auto cidFast = TBcid(aMidFast.mid(),aChFast.channel());
  //       float adcFast = aChFast.adc();
    
  //       comparisonWAVEvsFAST->Fill(adcWave, adcFast);
  //     }
  //   }
  // }

  // std::vector<float> FastYaxis__;
  // long long refTime = FastYaxis.at(0);
  // for ( int i = 0; i < FastXaxis.size(); i++ ) {
  //   FastYaxis__.push_back((float)(FastYaxis.at(i)-refTime)/1000000.);
  //   if (i == 0) FastYaxis_.push_back(0);
  //   if (i != 0) FastYaxis_.push_back(FastYaxis__.at(i) - FastYaxis__.at(i-1));
  // }
  //
  // std::vector<float> WaveYaxis__;
  // refTime = WaveYaxis.at(0);
  // for ( int i = 0; i < WaveXaxis.size(); i++ ) {
  //   WaveYaxis__.push_back((float)(WaveYaxis.at(i)-refTime)/1000000.);
  //   if (i == 0) WaveYaxis_.push_back(0);
  //   if (i != 0) WaveYaxis_.push_back(WaveYaxis__.at(i) - WaveYaxis__.at(i-1));
  // }
  //
  // TGraph* WaveAccuTime = new TGraph(WaveXaxis.size(), &(WaveXaxis[0]), &(WaveYaxis__[0]));
  // WaveAccuTime->SetLineWidth(1); WaveAccuTime->SetLineColor(kBlue+1);
  // TGraph* WaveDiffTime = new TGraph(WaveXaxis.size(), &(WaveXaxis[0]), &(WaveYaxis_[0]));
  // WaveDiffTime->SetLineWidth(1); WaveDiffTime->SetLineColor(kBlue+1);
  // TGraph* FastAccuTime = new TGraph(FastXaxis.size(), &(FastXaxis[0]), &(FastYaxis__[0]));
  // FastAccuTime->SetLineWidth(1); FastAccuTime->SetLineColor(kRed+1);
  // TGraph* FastDiffTime = new TGraph(FastXaxis.size(), &(FastXaxis[0]), &(FastYaxis_[0]));
  // FastDiffTime->SetLineWidth(1); FastDiffTime->SetLineColor(kRed+1);
  //
  // FastAccuTime->SetTitle("Accumulate time");
  // FastAccuTime->GetXaxis()->SetTitle("Evt");
  // FastAccuTime->GetYaxis()->SetTitle("ms");
  // WaveDiffTime->SetTitle("Passing time per event");
  // WaveDiffTime->GetXaxis()->SetTitle("Evt");
  // WaveDiffTime->GetYaxis()->SetTitle("ms");
  //
  // TCanvas* c_ = new TCanvas("Time", "Time", 1000, 500);
  // c_->cd();
  // TPad* LPad = new TPad("LPad", "LPad", 0.01, 0.01, 0.49, 0.99);
  // padSet(LPad, 0.12);
  //
  // c_->cd();
  // TPad* RPad = new TPad("RPad", "RPad", 0.51, 0.01, 0.99, 0.99);
  // padSet(RPad, 0.12);
  //
  // c_->cd();
  // LPad->cd();
  // FastAccuTime->Draw("al");
  // WaveAccuTime->Draw("l&sames");
  //
  // c_->cd();
  // RPad->cd();
  // WaveDiffTime->Draw("al");
  // FastDiffTime->Draw("l&sames");
  // c_->SaveAs("Timing.png");
  //
  // TCanvas* c_Comp = new TCanvas("comparison","comparison", 500, 500);
  // c_Comp->cd();
  // TPad* CompPad = new TPad("comparison", "comparison", 0.01, 0.01, 0.99, 0.99);
  // padSet(CompPad, 0.12);
  //
  // c_Comp->cd();
  // CompPad->cd();
  // comparisonWAVEvsFAST->Draw("colz");
  // c_Comp->SaveAs("Comparison_WAVE_vs_FAST_integratedADC.png");

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

  plotRootFile->Close();
}
