#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <cmath>

#include "TBevt.h"
#include "TButility.h"
#include "TBload.h"
#include "TBhist.h"

#include "functions.cc"

#include "TROOT.h"
#include "TStyle.h"
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>

int main(int argc, char** argv) {

  gStyle->SetOptFit(1);

  int runNum = std::stoi(argv[1]);

  TBload<TBfastmode>* loaderFast = new TBload<TBfastmode>(runNum);
  TBload<TBwaveform>* loaderWave = new TBload<TBwaveform>(runNum);

  int maxEvents = std::stoi(argv[2]); 


  TButility utility = TButility();
  utility.loading("mapping_data_MCPPMT_positiveSignal_v3.csv");
  utility.loadped("Run624_ped.csv");

  TBcid pscid = utility.getcid(TBdetector::detid::preshower);
  TBcid mucid = utility.getcid(TBdetector::detid::muon);
  TBcid tccid = utility.getcid(TBdetector::detid::tail);

  std::vector<TBcid> dwc1;
  dwc1.push_back(TBcid(1,17));
  dwc1.push_back(TBcid(1,19));
  dwc1.push_back(TBcid(1,21));
  dwc1.push_back(TBcid(1,23));

  std::vector<TBcid> dwc2;
  dwc2.push_back(TBcid(1,25));
  dwc2.push_back(TBcid(1,27));
  dwc2.push_back(TBcid(1,29));
  dwc2.push_back(TBcid(1,31));

  std::vector<TBcid> M1TC;
  M1TC.push_back(utility.getcid(TBdetector::detid::PMT, 1, 1, true));
  M1TC.push_back(utility.getcid(TBdetector::detid::PMT, 1, 2, true));
  M1TC.push_back(utility.getcid(TBdetector::detid::PMT, 1, 3, true));
  M1TC.push_back(utility.getcid(TBdetector::detid::PMT, 1, 4, true));

  std::vector<TBcid> M1TS;
  M1TS.push_back(utility.getcid(TBdetector::detid::PMT, 1, 1, false));
  M1TS.push_back(utility.getcid(TBdetector::detid::PMT, 1, 2, false));
  M1TS.push_back(utility.getcid(TBdetector::detid::PMT, 1, 3, false));
  M1TS.push_back(utility.getcid(TBdetector::detid::PMT, 1, 4, false));

  std::vector<TBcid> M2TC;
  M2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 1, true));
  M2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 2, true));
  M2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 3, true));
  M2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 4, true));
  M2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 6, true));
  M2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 7, true));
  M2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 8, true));
  M2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 9, true));

  std::vector<TBcid> M2TS;
  M2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 1, false));
  M2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 2, false));
  M2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 3, false));
  M2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 4, false));
  M2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 6, false));
  M2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 7, false));
  M2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 8, false));
  M2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 9, false));

  pscid.print();
  mucid.print();
  tccid.print();

  TBhist* psADC = new TBhist("preshower", "int. ADC", "events", 1000, 0., 160000.);
  psADC->getHist()->SetLineWidth(2); psADC->getHist()->SetLineColor(kRed);
  TBhist* muADC = new TBhist("muon", "int. ADC", "events", 1000, -1000., 4000.);
  muADC->getHist()->SetLineWidth(2); muADC->getHist()->SetLineColor(kRed);

  TBhist* psADC_raw = new TBhist("preshower_raw", "int. ADC", "events", 1000, 0., 160000.);
  psADC_raw->getHist()->SetLineWidth(2); psADC_raw->getHist()->SetLineColor(kRed);
  TBhist* muADC_raw = new TBhist("muon_raw", "int. ADC", "events", 1000, -1000., 4000.);
  muADC_raw->getHist()->SetLineWidth(2); muADC_raw->getHist()->SetLineColor(kRed);

  TBhist* fast_dwc1R = new TBhist("dwc1R (Fast)", "ns", "events", 400, 0., 400.);
  TBhist* fast_dwc1L = new TBhist("dwc1L (Fast)", "ns", "events", 400, 0., 400.);
  TBhist* fast_dwc1U = new TBhist("dwc1U (Fast)", "ns", "events", 400, 0., 400.);
  TBhist* fast_dwc1D = new TBhist("dwc1D (Fast)", "ns", "events", 400, 0., 400.);

  TBhist* fast_dwc2R = new TBhist("dwc2R (Fast)", "ns", "events", 400, 0., 400.);
  TBhist* fast_dwc2L = new TBhist("dwc2L (Fast)", "ns", "events", 400, 0., 400.);
  TBhist* fast_dwc2U = new TBhist("dwc2U (Fast)", "ns", "events", 400, 0., 400.);
  TBhist* fast_dwc2D = new TBhist("dwc2D (Fast)", "ns", "events", 400, 0., 400.);

  TBhist* fast_dwc1RmL = new TBhist("dwc1RmL (Fast)", "ns", "events", 800, -400., 400.);
  TBhist* fast_dwc1UmD = new TBhist("dwc1UmD (Fast)", "ns", "events", 800, -400., 400.);
  TBhist* fast_dwc2RmL = new TBhist("dwc2RmL (Fast)", "ns", "events", 800, -400., 400.);
  TBhist* fast_dwc2UmD = new TBhist("dwc2UmD (Fast)", "ns", "events", 800, -400., 400.);

  TBhist* wave_dwc1R = new TBhist("dwc1R (Wave)", "ns", "events", 400, 0., 400.);
  TBhist* wave_dwc1L = new TBhist("dwc1L (Wave)", "ns", "events", 400, 0., 400.);
  TBhist* wave_dwc1U = new TBhist("dwc1U (Wave)", "ns", "events", 400, 0., 400.);
  TBhist* wave_dwc1D = new TBhist("dwc1D (Wave)", "ns", "events", 400, 0., 400.);

  TBhist* wave_dwc2R = new TBhist("dwc2R (Wave)", "ns", "events", 400, 0., 400.);
  TBhist* wave_dwc2L = new TBhist("dwc2L (Wave)", "ns", "events", 400, 0., 400.);
  TBhist* wave_dwc2U = new TBhist("dwc2U (Wave)", "ns", "events", 400, 0., 400.);
  TBhist* wave_dwc2D = new TBhist("dwc2D (Wave)", "ns", "events", 400, 0., 400.);

  TBhist* wave_dwc1RmL = new TBhist("dwc1RmL (Wave)", "ns", "events", 800, -400., 400.);
  TBhist* wave_dwc1UmD = new TBhist("dwc1UmD (Wave)", "ns", "events", 800, -400., 400.);
  TBhist* wave_dwc2RmL = new TBhist("dwc2RmL (Wave)", "ns", "events", 800, -400., 400.);
  TBhist* wave_dwc2UmD = new TBhist("dwc2UmD (Wave)", "ns", "events", 800, -400., 400.);




  TH2I* psWave = new TH2I("psWave", ";;", 1024, 0., 1023., 4096, 0., 4095.);
  TH2I* muWave = new TH2I("muWave", ";;", 1024, 0., 1023., 4096, 0., 4095.);
  TH2I* tcWave = new TH2I("tcWave", ";;", 1024, 0., 1023., 4096, 0., 4095.);
  TH2I* M1T2C = new TH2I("M1T2C", ";;", 1024, 0., 1023., 4096, 0., 4095.);
  TH2I* M1T2S = new TH2I("M1T2S", ";;", 1024, 0., 1023., 4096, 0., 4095.);

  TH2I* psWave_raw = new TH2I("psWave_raw", ";;", 1024, 0., 1023., 4096, 0., 4095.);
  TH2I* muWave_raw = new TH2I("muWave_raw", ";;", 1024, 0., 1023., 4096, 0., 4095.);
  TH2I* tcWave_raw = new TH2I("tcWave_raw", ";;", 1024, 0., 1023., 4096, 0., 4095.);
  TH2I* M1T2C_raw = new TH2I("M1T2C_raw", ";;", 1024, 0., 1023., 4096, 0., 4095.);
  TH2I* M1T2S_raw = new TH2I("M1T2S_raw", ";;", 1024, 0., 1023., 4096, 0., 4095.);

  TH2I* M1T2C_refine = new TH2I("M1T2C_refine", ";;", 150, 0., 150., 4096, 0., 4095.);
  TH2I* M1T2S_refine = new TH2I("M1T2S_refine", ";;", 150, 0., 150., 4096, 0., 4095.);

  if ( maxEvents == -1 )
    maxEvents = loaderFast->getEntries();

  if ( maxEvents > loaderFast->getEntries() )
    throw std::runtime_error("Events number is larger than entries !");

  std::chrono::time_point time_begin = std::chrono::system_clock::now(); //delete
  printf("\n");

  loaderWave->loading();
  for ( int ievt = 0; ievt < maxEvents; ievt++ ) {

    if ( ievt > 0 && ievt % 8 == 0 ) {
      std::chrono::duration time_taken = std::chrono::system_clock::now() - time_begin; //delete
      float percent_done = 1. * ievt / (float)maxEvents;
      std::chrono::duration time_left = time_taken * ( 1 / percent_done - 1 );
      std::chrono::minutes minutes_left = std::chrono::duration_cast<std::chrono::minutes>(time_left);
      std::chrono::seconds seconds_left = std::chrono::duration_cast<std::chrono::seconds>(time_left - minutes_left);
      std::cout << "\r\033[F"<< " " << ievt << " events  "<< minutes_left.count() << ":";
      printf("%02d left   %.1f%%            \n",int(seconds_left.count()),percent_done*100);
    }

    loaderFast->loading();

    if ( loaderFast->getEvent()->tcb() == loaderWave->getEvent()->tcb() ) {

      auto aFastEvt = loaderFast->getEvent();
      auto aWaveEvt = loaderWave->getEvent();

      // loaderWave->loading();
    
      // std::cout << ievt << " "
      //           << loaderFast->currenLoader() << " "
      //           << loaderWave->currenLoader() << " "
      //           << aFastEvt->tcb() << " "
      //           << aWaveEvt->tcb() << " " 
      //           << std::endl;



      TBfastmode aFast_ps = aFastEvt->data(pscid);
      TBfastmode aFast_mu = aFastEvt->data(mucid);
      TBfastmode aFast_tc = aFastEvt->data(tccid);

      TBwaveform aWave_ps = aWaveEvt->data(pscid);
      TBwaveform aWave_mu = aWaveEvt->data(mucid);
      TBwaveform aWave_tc = aWaveEvt->data(tccid);

      TBwaveform aWave_M1T2C = aWaveEvt->data(M1TC.at(1));
      TBwaveform aWave_M1T2S = aWaveEvt->data(M1TS.at(1));

      std::vector<TBfastmode> DWC1_fast;
      for ( int i = 0; i < dwc1.size(); i++ )
        DWC1_fast.push_back(aFastEvt->data(dwc1.at(i)));

      std::vector<TBfastmode> DWC2_fast;
      for ( int i = 0; i < dwc2.size(); i++ )
        DWC2_fast.push_back(aFastEvt->data(dwc2.at(i)));

      std::vector<TBwaveform> DWC1_wave;
      for ( int i = 0; i < dwc1.size(); i++ )
        DWC1_wave.push_back(aWaveEvt->data(dwc1.at(i)));

      std::vector<TBwaveform> DWC2_wave;
      for ( int i = 0; i < dwc2.size(); i++ )
        DWC2_wave.push_back(aWaveEvt->data(dwc2.at(i)));

      // std::vector<TBfastmode> M2TC_data;
      // std::vector<TBfastmode> M2TS_data;
      // for ( int i = 0; i < 8; i++ ) {
      //   M2TC_data.push_back(anEvt->data(M2TC.at(i)));
      //   M2TS_data.push_back(anEvt->data(M2TS.at(i)));
      // }

      // std::vector<TBfastmode> M1TC_data;
      // std::vector<TBfastmode> M1TS_data;
      // for ( int i = 0; i < M1TC.size(); i++ ) {
      //   M1TC_data.push_back(anEvt->data(M1TC.at(i)));
      //   M1TS_data.push_back(anEvt->data(M1TS.at(i)));
      // }

      std::vector<float> dwc_time_fast;
      std::vector<float> dwc_time_wave;

      for ( int i = 0; i < 4; i++ )
        dwc_time_fast.push_back((float)DWC1_fast.at(i).timing() * 25./1000.);

      for ( int i = 0; i < 4; i++ )
        dwc_time_fast.push_back((float)DWC2_fast.at(i).timing() * 25./1000.);

      for ( int i = 0; i < 4; i++ ) {
        // dwc_time_wave.push_back(getDWCtimeWave(DWC1_wave.at(i), utility.retrievePed(dwc1.at(i))));
        dwc_time_wave.push_back(getDWCtimeWavePeak(DWC1_wave.at(i).waveform()));
      }

      for ( int i = 0; i < 4; i++ ) {
        // dwc_time_wave.push_back(getDWCtimeWave(DWC2_wave.at(i), utility.retrievePed(dwc2.at(i))));
        dwc_time_wave.push_back(getDWCtimeWavePeak(DWC2_wave.at(i).waveform()));
      }

      fast_dwc1R->Fill(dwc_time_fast.at(0));
      fast_dwc1L->Fill(dwc_time_fast.at(1));
      fast_dwc1U->Fill(dwc_time_fast.at(2));
      fast_dwc1D->Fill(dwc_time_fast.at(3));

      fast_dwc2R->Fill(dwc_time_fast.at(4));
      fast_dwc2L->Fill(dwc_time_fast.at(5));
      fast_dwc2U->Fill(dwc_time_fast.at(6));
      fast_dwc2D->Fill(dwc_time_fast.at(7));

      fast_dwc1RmL->Fill(dwc_time_fast.at(0) - dwc_time_fast.at(1));
      fast_dwc1UmD->Fill(dwc_time_fast.at(2) - dwc_time_fast.at(3));
      fast_dwc2RmL->Fill(dwc_time_fast.at(4) - dwc_time_fast.at(5));
      fast_dwc2UmD->Fill(dwc_time_fast.at(6) - dwc_time_fast.at(7));

      wave_dwc1R->Fill(dwc_time_wave.at(0));
      wave_dwc1L->Fill(dwc_time_wave.at(1));
      wave_dwc1U->Fill(dwc_time_wave.at(2));
      wave_dwc1D->Fill(dwc_time_wave.at(3));

      wave_dwc2R->Fill(dwc_time_wave.at(4));
      wave_dwc2L->Fill(dwc_time_wave.at(5));
      wave_dwc2U->Fill(dwc_time_wave.at(6));
      wave_dwc2D->Fill(dwc_time_wave.at(7));

      wave_dwc1RmL->Fill(dwc_time_wave.at(0) - dwc_time_wave.at(1));
      wave_dwc1UmD->Fill(dwc_time_wave.at(2) - dwc_time_wave.at(3));
      wave_dwc2RmL->Fill(dwc_time_wave.at(4) - dwc_time_wave.at(5));
      wave_dwc2UmD->Fill(dwc_time_wave.at(6) - dwc_time_wave.at(7));

      // auto dwc_pos = getDWCposition(dwc_time);
      // auto centerPos = getPositionFromDWC(dwc_pos);
      // float lecDiff = getLengthDiff(dwc_pos);
      // float angleDiff = 180./const_pi() * std::atan( lecDiff / 1675. );

      // float psFastADC = aFast_ps.adc();
      // float muFastADC = aFast_mu.adc();
      // float tcFastADC = aFast_tc.adc();

      // psADC_beforeCut->Fill(psADC);

      // auto wave_ps = aWave_ps.waveform();
      // auto wave_mu = aWave_mu.waveform();
      // auto wave_tc = aWave_tc.waveform();
      // auto wave_m1t2c = aWave_M1T2C.waveform();
      // auto wave_m1t2s = aWave_M1T2S.waveform();

      // for ( int i = 0; i < 1024; i++ ) {
      //   psWave_raw->Fill(i, wave_ps.at(i));
      //   muWave_raw->Fill(i, wave_mu.at(i));
      //   tcWave_raw->Fill(i, wave_tc.at(i));
      //   M1T2C_raw->Fill(i, wave_m1t2c.at(i));
      //   M1T2S_raw->Fill(i, wave_m1t2s.at(i));
      // }

      // psADC_raw->Fill(psFastADC);
      // muADC_raw->Fill(muFastADC);

      // if ( !inDWCaxisAlign(dwc_pos, 2) &&
      //      !inVetoWithAlign(centerPos) &&
      //      !muCut(muFastADC) &&
      //      !psCut(psFastADC, 5.0) ) {

      //   loaderWave->loading();
      //   continue;
      // }

      // if ( inDWCaxisAlign(dwc_pos, 2) ) {
      //   if ( inVetoWithAlign(centerPos) ) {
      //     if ( muCut(muFastADC) ) {
      //       if ( psCut(psFastADC, 5.0) ) {
      //         psADC->Fill(psFastADC);
      //         muADC->Fill(muFastADC);

      //         for ( int i = 0; i < 1024; i++ ) {
      //           psWave->Fill(i, wave_ps.at(i));
      //           muWave->Fill(i, wave_mu.at(i));
      //           tcWave->Fill(i, wave_tc.at(i));
      //           M1T2C->Fill(i, wave_m1t2c.at(i));
      //           M1T2S->Fill(i, wave_m1t2s.at(i));
      //         }

      //         auto ceren_peak = getPeakRegion(wave_m1t2c);
      //         auto scint_peak = getPeakRegion(wave_m1t2s);

      //         for ( int i = 0; i < 150; i++ ) {
      //           M1T2C_refine->Fill(i, ceren_peak.at(i));
      //           M1T2S_refine->Fill(i, scint_peak.at(i));
      //         }

      //       }
      //     }
      //   }
      // }

      // loaderWave->loading();

      // psADC->Fill(psFastADC);
      // muADC->Fill(muFastADC);

  
      // if ( !inVetoWithAlign(centerPos) ) continue;
      // if ( !muCut(muFastADC) ) continue;
      // if ( !psCut(psFastADC, 5.0) ) continue;

      // for ( int i = 0; i < 1024; i++ ) {
      //   psWave->Fill(i, wave_ps.at(i));
      //   muWave->Fill(i, wave_mu.at(i));
      //   tcWave->Fill(i, wave_tc.at(i));
      //   M1T2C->Fill(i, wave_m1t2c.at(i));
      //   M1T2S->Fill(i, wave_m1t2s.at(i));
      // }

      loaderWave->loading();
    }
  }
  loaderFast->close();
  loaderWave->close();


  // TCanvas* c = new TCanvas("", "");

  // c->cd(); psADC_beforeCut->getHist()->Draw(); c->SaveAs("preshower_before_cut.png");
  // c->cd(); psADC_afterCut->getHist()->Draw(); c->SaveAs("preshower_after_cut.png");

  TFile* rootOutput = new TFile("./230125/Run624_DWC_peak.root", "RECREATE");
  rootOutput->cd();
  // psWave_raw->Write();
  // muWave_raw->Write();
  // tcWave_raw->Write();
  // M1T2C_raw->Write();
  // M1T2S_raw->Write();
  // psWave->Write();
  // muWave->Write();
  // tcWave->Write();
  // M1T2C->Write();
  // M1T2S->Write();
  // psADC->getHist()->Write();
  // muADC->getHist()->Write();
  // psADC_raw->getHist()->Write();
  // muADC_raw->getHist()->Write();
  // M1T2C_refine->Write();
  // M1T2S_refine->Write();

  fast_dwc1R->getHist()->Write();
  fast_dwc1L->getHist()->Write();
  fast_dwc1U->getHist()->Write();
  fast_dwc1D->getHist()->Write();
  fast_dwc2R->getHist()->Write();
  fast_dwc2L->getHist()->Write();
  fast_dwc2U->getHist()->Write();
  fast_dwc2D->getHist()->Write();
  fast_dwc1RmL->getHist()->Write();
  fast_dwc1UmD->getHist()->Write();
  fast_dwc2RmL->getHist()->Write();
  fast_dwc2UmD->getHist()->Write();

  wave_dwc1R->getHist()->Write();
  wave_dwc1L->getHist()->Write();
  wave_dwc1U->getHist()->Write();
  wave_dwc1D->getHist()->Write();
  wave_dwc2R->getHist()->Write();
  wave_dwc2L->getHist()->Write();
  wave_dwc2U->getHist()->Write();
  wave_dwc2D->getHist()->Write();
  wave_dwc1RmL->getHist()->Write();
  wave_dwc1UmD->getHist()->Write();
  wave_dwc2RmL->getHist()->Write();
  wave_dwc2UmD->getHist()->Write();

  rootOutput->Close();

  return 0;
}
