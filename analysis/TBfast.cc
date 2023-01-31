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
  TBload<TBfastmode>* loader = new TBload<TBfastmode>(runNum);

  int maxEvents = std::stoi(argv[2]); 
  double psThresCut = std::stof(argv[3]);

  TButility utility = TButility();
  utility.loading("mapping_data_MCPPMT_positiveSignal_v3.csv");
  utility.loadped("ped_400.csv");

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

  TBhist* psADC_beforeCut = new TBhist("preshower before cut", "int. ADC", "events", 1000, 0., 10000.);
  psADC_beforeCut->getHist()->SetLineWidth(2); psADC_beforeCut->getHist()->SetLineColor(kRed);
  TBhist* psADC_afterCut = new TBhist("preshower after cut", "int. ADC", "events", 1000, 0., 10000.);
  psADC_afterCut->getHist()->SetLineWidth(2); psADC_afterCut->getHist()->SetLineColor(kRed);

  if ( maxEvents == -1 )
    maxEvents = loader->getEntries();

  if ( maxEvents > loader->getEntries() )
    throw std::runtime_error("Events number is larger than entries !");

  std::chrono::time_point time_begin = std::chrono::system_clock::now(); //delete
  printf("\n");

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

    loader->loading();
    auto anEvt = loader->getEvent();

    TBfastmode adata_ps = anEvt->data(pscid);
    TBfastmode adata_mu = anEvt->data(mucid);
    TBfastmode adata_tc = anEvt->data(tccid);

    std::vector<TBfastmode> DWC1_fast;
    for ( int i = 0; i < dwc1.size(); i++ )
      DWC1_fast.push_back(anEvt->data(dwc1.at(i)));

    std::vector<TBfastmode> DWC2_fast;
    for ( int i = 0; i < dwc2.size(); i++ )
      DWC2_fast.push_back(anEvt->data(dwc2.at(i)));

    std::vector<TBfastmode> M2TC_data;
    std::vector<TBfastmode> M2TS_data;
    for ( int i = 0; i < 8; i++ ) {
      M2TC_data.push_back(anEvt->data(M2TC.at(i)));
      M2TS_data.push_back(anEvt->data(M2TS.at(i)));
    }

    std::vector<TBfastmode> M1TC_data;
    std::vector<TBfastmode> M1TS_data;
    for ( int i = 0; i < M1TC.size(); i++ ) {
      M1TC_data.push_back(anEvt->data(M1TC.at(i)));
      M1TS_data.push_back(anEvt->data(M1TS.at(i)));
    }

    std::vector<int> dwc_time;

    for ( int i = 0; i < 4; i++ )
      dwc_time.push_back(DWC1_fast.at(i).timing());

    for ( int i = 0; i < 4; i++ )
      dwc_time.push_back(DWC2_fast.at(i).timing());

    auto dwc_pos = getDWCposition(dwc_time);
    auto centerPos = getPositionFromDWC(dwc_pos);
    float lecDiff = getLengthDiff(dwc_pos);
    float angleDiff = 180./const_pi() * std::atan( lecDiff / 1675. );

    float muADC = adata_mu.adc();
    float psADC = adata_ps.adc();
    float tcADC = adata_tc.adc();

    psADC_beforeCut->Fill(psADC);

    if ( !inDWCaxisAlign(dwc_pos, 2) ) continue;
    if ( !inVetoWithAlign(centerPos) ) continue;
    if ( !muCut(muADC) ) continue;
    if ( !psCut(psADC, psThresCut) ) continue;
  
    psADC_afterCut->Fill(psADC);

  }
  loader->close();


  TCanvas* c = new TCanvas("", "");

  c->cd(); psADC_beforeCut->getHist()->Draw(); c->SaveAs("preshower_before_cut.png");
  c->cd(); psADC_afterCut->getHist()->Draw(); c->SaveAs("preshower_after_cut.png");

  TFile* rootOutput = new TFile("outout.root", "RECREATE");
  rootOutput->cd();
  psADC_beforeCut->getHist()->Write();
  psADC_afterCut->getHist()->Write();
  rootOutput->Close();

  return 0;
}
