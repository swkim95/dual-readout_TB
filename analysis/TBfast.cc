#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <cmath>

#include "TBevt.h"
#include "TButility.h"

#include "TROOT.h"
#include "TStyle.h"
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>

constexpr double const_pi() { 
  return std::atan(1) * 4;
}

float peakADC(std::vector<short> waveform) {
  float ped = 0;
  for ( int i = 1; i < 101; i++ )
    ped += (float)waveform.at(i)/100.;

  
  return ped - (float)*std::min_element(waveform.begin()+1, waveform.end()-23);
}

float getPed(std::vector<short> waveform) {
  float ped = 0;
  for ( int i = 1; i < 101; i++ )
    ped += (float)waveform.at(i)/100.;

  return ped;
}

TH1D* plotTH1(TBwaveform mid) {
  
  auto waveform = mid.waveform();

  TH1D* wave = new TH1D("", ";;", 1000, 0., 1000.);

  for ( int i = 1; i < 1001; i++ ) {
    wave->SetBinContent(i + 1, waveform.at(i));
    wave->SetBinContent(i + 1, waveform.at(i));
  }
  wave->SetStats(0);


  return wave;
}

float getTime(float bin) {
  return 400. * (bin / 1000.);
}

float getLinearInterpolate(float thres, float i, float j, float fi, float fj) {
  // std::cout << i + (j - i) * (thres - fi) / (fj - fi) << std::endl;
  return i + (j - i) * (thres - fi) / (fj - fi);
}

float getLeadingEdge(std::vector<float> waveform) {

  float LedingThres = (float)*std::max_element(waveform.begin()+1, waveform.end()-23) * 0.1;
  // std::cout << LedingThres << std::endl;
  
  for ( int i = 1; i <= 1000; i++ )
    if ( waveform.at(i) > LedingThres ) {
      // std::cout << i-1 << " "
      //           << waveform.at(i-1) << " "
      //           << i << " "
      //           << waveform.at(i) << " "
                // << std::endl;
      return getTime(getLinearInterpolate(LedingThres, i-1, i, waveform.at(i-1), waveform.at(i)));
    }

  // throw std::runtime_error("getLeadingEdge : no leading edge!");
  return 0;
}

std::vector<float> getPosition(std::vector<TBwaveform> dwcSet, float xCoeff, float xConst, float yCoeff, float yConst) {

  std::vector<float> time;
  for ( int i = 0; i < dwcSet.size(); i++ )
    time.push_back(getLeadingEdge(dwcSet.at(i).pedcorrectedWaveform(getPed(dwcSet.at(i).waveform()))));



  return time;

}

static float dwc1horizontalSlope = -0.1740806676;
static float dwc1horizontalOffset = -0.1680572999;
static float dwc1VerticalSlope = -0.17424779576;
static float dwc1VerticalOffset = -0.053701300;

static float dwc2horizontalSlope = -0.17257273;
static float dwc2horizontalOffset = -0.579927452;
static float dwc2VerticalSlope = -0.1741203164;
static float dwc2VerticalOffset = -0.278179655;

static float dwc1Xoffset = 2.46;
static float dwc1Yoffset = -3.191;
static float dwc2Xoffset = 4.144;
static float dwc2Yoffset = -10.54;

std::vector<float> getDWCposition(std::vector<int> dwcTime) {

  std::vector<float> dec_pos;

  dec_pos.push_back( -((float)(dwcTime.at(0) - dwcTime.at(1))*25./1000. * dwc1horizontalSlope + dwc1horizontalOffset) + dwc1Xoffset );
  dec_pos.push_back( (float)(dwcTime.at(2) - dwcTime.at(3))*25./1000. * dwc1VerticalSlope + dwc1VerticalOffset + dwc1Yoffset );
  dec_pos.push_back( -((float)(dwcTime.at(4) - dwcTime.at(5))*25./1000. * dwc2horizontalSlope + dwc2horizontalOffset) + dwc2Xoffset );
  dec_pos.push_back( (float)(dwcTime.at(6) - dwcTime.at(7))*25./1000. * dwc2VerticalSlope + dwc2VerticalOffset + dwc2Yoffset );

  return dec_pos;
}

std::vector<float> getPositionFromDWC(std::vector<float> dwcPos) {
  std::vector<float> centerLen;
  centerLen.push_back(std::sqrt( dwcPos.at(0)*dwcPos.at(0) + dwcPos.at(1)*dwcPos.at(1) ));
  centerLen.push_back(std::sqrt( dwcPos.at(2)*dwcPos.at(2) + dwcPos.at(3)*dwcPos.at(3) ));

  return centerLen;
}

float getLengthDiff(std::vector<float> dwcPos) {
  return std::sqrt( (dwcPos.at(0) - dwcPos.at(2))*(dwcPos.at(0) - dwcPos.at(2)) + (dwcPos.at(1) - dwcPos.at(3))*(dwcPos.at(1) - dwcPos.at(3)) );
}

bool inVetoWithAlign(std::vector<float> centerPos) {
  return (centerPos.at(0) <= 10) && (centerPos.at(1) <= 10);
}

bool inDWCaxisAlign(std::vector<float> dwcPos, double thres) {
  return ( std::abs(dwcPos.at(0) - dwcPos.at(2)) < thres && std::abs(dwcPos.at(1) - dwcPos.at(3)) < thres );
}


int getPID(double PS, double MU) {
  float muMean = 495.9;
  float muSigma = 444.6;
  float psMean = 8208;
  float psSigma = 1183;

  if ( MU < muMean + muSigma ) {
    if ( PS < psMean * 2.5 ) {
      return 211;
    } else {
      return 11;
    }
  } else {
    return 13;
  }

  return -1;
}




// 20GeV : 682
// 30GeV : 680
// 40GeV : 679
// 60GeV : 678
// 80GeV : 674
// 100GeV : 675
// 125GeV : 676

int main(int argc, char** argv) {

  gStyle->SetOptFit(1);

  TString baseDir = "/pnfs/knu.ac.kr/data/cms/store/user/khwang/Aug2022TB/ntuple/fastmode/";
  TString runNum = argv[1];
  TString filename = baseDir + "ntuple_Run_" + runNum + "_Fast.root";

  // TString filename = argv[1];
  int maxEvents = std::stoi(argv[2]); 
  TString outputname = argv[3];
  int takingCut = std::stoi(argv[4]);

  // float muADC_FullRange_onTree;
  // float muADC_FastEmul_onTree;
  // float muADC_Peak_onTree;

  // float psADC_FullRange_onTree;
  // float psADC_FastEmul_onTree;
  // float psADC_Peak_onTree;

  // TFile* outputFile = new TFile(runNum + "_" + outputname + ".root", "RECREATE");

  // TTree* dataTree = new TTree("data", "data");
  // dataTree->Branch("muADC_FullRange_onTree", &muADC_FullRange_onTree);
  // dataTree->Branch("muADC_FastEmul_onTree", &muADC_FastEmul_onTree);
  // dataTree->Branch("muADC_Peak_onTree", &muADC_Peak_onTree);
  // dataTree->Branch("psADC_FullRange_onTree", &psADC_FullRange_onTree);
  // dataTree->Branch("psADC_FastEmul_onTree", &psADC_FastEmul_onTree);
  // dataTree->Branch("psADC_Peak_onTree", &psADC_Peak_onTree);

  // TH1D* muPeak = new TH1D("", ";;", 1024, -1024., 40960./4.);
  // TH1D* psPeak = new TH1D("", ";;", 1024, -1024., 40960.*4.);
  // TH1D* psCutMuPeak = new TH1D("", ";;", 1024, -1024., 40960.*2.);

  TH1D* M1T1C_origin = new TH1D("", ";;", 1024, -1024., 30720.); M1T1C_origin->SetLineWidth(2); M1T1C_origin->SetLineColor(kBlue);
  TH1D* M1T2C_origin = new TH1D("", ";;", 1024, -1024., 30720.); M1T2C_origin->SetLineWidth(2); M1T2C_origin->SetLineColor(kBlue);
  TH1D* M1T3C_origin = new TH1D("", ";;", 1024, -1024., 30720.); M1T3C_origin->SetLineWidth(2); M1T3C_origin->SetLineColor(kBlue);
  TH1D* M1T4C_origin = new TH1D("", ";;", 1024, -1024., 30720.); M1T4C_origin->SetLineWidth(2); M1T4C_origin->SetLineColor(kBlue);

  TH1D* M1T1S_origin = new TH1D("", ";;", 1024, -1024., 30720.); M1T1S_origin->SetLineWidth(2); M1T1S_origin->SetLineColor(kRed);
  TH1D* M1T2S_origin = new TH1D("", ";;", 1024, -1024., 30720.); M1T2S_origin->SetLineWidth(2); M1T2S_origin->SetLineColor(kRed);
  TH1D* M1T3S_origin = new TH1D("", ";;", 1024, -1024., 30720.); M1T3S_origin->SetLineWidth(2); M1T3S_origin->SetLineColor(kRed);
  TH1D* M1T4S_origin = new TH1D("", ";;", 1024, -1024., 30720.); M1T4S_origin->SetLineWidth(2); M1T4S_origin->SetLineColor(kRed);

  TH1D* M2T1C_origin = new TH1D("", ";;", 1024, -1024., 30720.); M2T1C_origin->SetLineWidth(2); M2T1C_origin->SetLineColor(kBlue);
  TH1D* M2T2C_origin = new TH1D("", ";;", 1024, -1024., 30720.); M2T2C_origin->SetLineWidth(2); M2T2C_origin->SetLineColor(kBlue);
  TH1D* M2T3C_origin = new TH1D("", ";;", 1024, -1024., 30720.); M2T3C_origin->SetLineWidth(2); M2T3C_origin->SetLineColor(kBlue);
  TH1D* M2T4C_origin = new TH1D("", ";;", 1024, -1024., 30720.); M2T4C_origin->SetLineWidth(2); M2T4C_origin->SetLineColor(kBlue);
  TH1D* M2T6C_origin = new TH1D("", ";;", 1024, -1024., 30720.); M2T6C_origin->SetLineWidth(2); M2T6C_origin->SetLineColor(kBlue);
  TH1D* M2T7C_origin = new TH1D("", ";;", 1024, -1024., 30720.); M2T7C_origin->SetLineWidth(2); M2T7C_origin->SetLineColor(kBlue);
  TH1D* M2T8C_origin = new TH1D("", ";;", 1024, -1024., 30720.); M2T8C_origin->SetLineWidth(2); M2T8C_origin->SetLineColor(kBlue);
  TH1D* M2T9C_origin = new TH1D("", ";;", 1024, -1024., 30720.); M2T9C_origin->SetLineWidth(2); M2T9C_origin->SetLineColor(kBlue);

  TH1D* M2T1S_origin = new TH1D("", ";;", 1024, -1024., 30720.); M2T1S_origin->SetLineWidth(2); M2T1S_origin->SetLineColor(kRed);
  TH1D* M2T2S_origin = new TH1D("", ";;", 1024, -1024., 30720.); M2T2S_origin->SetLineWidth(2); M2T2S_origin->SetLineColor(kRed);
  TH1D* M2T3S_origin = new TH1D("", ";;", 1024, -1024., 30720.); M2T3S_origin->SetLineWidth(2); M2T3S_origin->SetLineColor(kRed);
  TH1D* M2T4S_origin = new TH1D("", ";;", 1024, -1024., 30720.); M2T4S_origin->SetLineWidth(2); M2T4S_origin->SetLineColor(kRed);
  TH1D* M2T6S_origin = new TH1D("", ";;", 1024, -1024., 30720.); M2T6S_origin->SetLineWidth(2); M2T6S_origin->SetLineColor(kRed);
  TH1D* M2T7S_origin = new TH1D("", ";;", 1024, -1024., 30720.); M2T7S_origin->SetLineWidth(2); M2T7S_origin->SetLineColor(kRed);
  TH1D* M2T8S_origin = new TH1D("", ";;", 1024, -1024., 30720.); M2T8S_origin->SetLineWidth(2); M2T8S_origin->SetLineColor(kRed);
  TH1D* M2T9S_origin = new TH1D("", ";;", 1024, -1024., 30720.); M2T9S_origin->SetLineWidth(2); M2T9S_origin->SetLineColor(kRed);

  TH2D* psdVSmoduleC= new TH2D("", ";;", 1024, -1024., 40960.*4., 3072, -3072., 30720.*3.);
  TH2D* psdVSmoduleS= new TH2D("", ";;", 1024, -1024., 40960.*4., 3072, -3072., 30720.*3.);

  TH1D* muFast = new TH1D("", ";;", 1024, -1024., 40960.); muFast->SetLineWidth(2); muFast->SetLineColor(kBlack);
  TH1D* psFast = new TH1D("", ";;", 1024, -1024., 40960.*4.); psFast->SetLineWidth(2); psFast->SetLineColor(kBlack);
  TH1D* tcFast = new TH1D("", ";;", 1024, -1024., 40960.); tcFast->SetLineWidth(2); tcFast->SetLineColor(kBlack);

  TH2D* dwc1pos = new TH2D("", ";;", 120, -60, 60, 120, -60, 60);
  TH2D* dwc2pos = new TH2D("", ";;", 120, -60, 60, 120, -60, 60);
  TH2D* dwc1vs2x = new TH2D("", ";;", 120, -60, 60, 120, -60, 60);
  TH2D* dwc1vx2y = new TH2D("", ";;", 120, -60, 60, 120, -60, 60);

  TH1D* dwc1_X = new TH1D("", ";;", 120, -60, 60); dwc1_X->SetLineWidth(2); dwc1_X->SetLineColor(kBlack);
  TH1D* dwc1_Y = new TH1D("", ";;", 120, -60, 60); dwc1_Y->SetLineWidth(2); dwc1_Y->SetLineColor(kBlack);
  TH1D* dwc2_X = new TH1D("", ";;", 120, -60, 60); dwc2_X->SetLineWidth(2); dwc2_X->SetLineColor(kBlack);
  TH1D* dwc2_Y = new TH1D("", ";;", 120, -60, 60); dwc2_Y->SetLineWidth(2); dwc2_Y->SetLineColor(kBlack);

  TH1D* dwc_center_diff = new TH1D("", ";;", 180, 0., 60.); dwc_center_diff->SetLineWidth(2); dwc_center_diff->SetLineColor(kBlack);
  TH1D* dwc1_centerPos = new TH1D("", ";;", 180, 0., 60.); dwc1_centerPos->SetLineWidth(2); dwc1_centerPos->SetLineColor(kBlack);
  TH1D* dwc2_centerPos = new TH1D("", ";;", 180, 0., 60.); dwc2_centerPos->SetLineWidth(2); dwc2_centerPos->SetLineColor(kBlack);
  TH1D* dwc_center_diff_angle = new TH1D("", ";;", 150, 0., 1.5); dwc_center_diff_angle->SetLineWidth(2); dwc_center_diff_angle->SetLineColor(kBlack);

  TH1D* dwc_X_diff = new TH1D("", ";;", 360, -60., 60.); dwc_X_diff->SetLineWidth(2); dwc_X_diff->SetLineColor(kBlack);
  TH1D* dwc_Y_diff = new TH1D("", ";;", 360, -60., 60.); dwc_Y_diff->SetLineWidth(2); dwc_Y_diff->SetLineColor(kBlack);

  TH1D* dwc_X_diff_abs = new TH1D("", ";;", 180, 0., 60.); dwc_X_diff_abs->SetLineWidth(2); dwc_X_diff_abs->SetLineColor(kBlack);
  TH1D* dwc_Y_diff_abs = new TH1D("", ";;", 180, 0., 60.); dwc_Y_diff_abs->SetLineWidth(2); dwc_Y_diff_abs->SetLineColor(kBlack);


  // float muMean = 495.9;
  // float muSigma = 444.6;
  // float psMean = 8208;
  // float psSigma = 1183;

  // // TH1D* DWC1D1 = new TH1D("", ";;", 1000, 0., 400.);
  // // TH1D* DWC1D2 = new TH1D("", ";;", 1000, 0., 400.);
  // // TH1D* DWC1D3 = new TH1D("", ";;", 1000, 0., 400.);
  // // TH1D* DWC1D4 = new TH1D("", ";;", 1000, 0., 400.);
  // // TH1D* DWC2D1 = new TH1D("", ";;", 1000, 0., 400.);
  // // TH1D* DWC2D2 = new TH1D("", ";;", 1000, 0., 400.);
  // // TH1D* DWC2D3 = new TH1D("", ";;", 1000, 0., 400.);
  // // TH1D* DWC2D4 = new TH1D("", ";;", 1000, 0., 400.);

  // // TH1D* DWC1RL = new TH1D("", ";;", 1000, -400, 400.);
  // // TH1D* DWC1DU = new TH1D("", ";;", 1000, -400, 400.);

  // // TH1D* DWC2RL = new TH1D("", ";;", 1000, -400, 400.);
  // // TH1D* DWC3DU = new TH1D("", ";;", 1000, -400, 400.);

  TFile* afile = new TFile(filename);
  TTree* atree = (TTree*)afile->Get("events");

  TBevt<TBfastmode>* anevt = new TBevt<TBfastmode>();
  atree->SetBranchAddress("TBevt",&anevt);

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



  pscid.print();
  mucid.print();
  tccid.print();

  unsigned channelsize = 32;

  std::chrono::time_point time_begin = std::chrono::system_clock::now(); //delete
  printf("\n");

  if ( maxEvents == -1 )
    maxEvents = atree->GetEntries();

  if ( maxEvents > atree->GetEntries() )
    throw std::runtime_error("Events number is larger than entries !");

  TCanvas* c = new TCanvas("","");

  for (unsigned ievt = 0; ievt < maxEvents; ievt++) {
  // for (unsigned ievt = 0; ievt < 1; ievt++) {


    if ( ievt > 0 && ievt % 8 == 0 ) {
      std::chrono::duration time_taken = std::chrono::system_clock::now() - time_begin; //delete
      float percent_done = 1. * ievt / (float)maxEvents;
      std::chrono::duration time_left = time_taken * ( 1 / percent_done - 1 );
      std::chrono::minutes minutes_left = std::chrono::duration_cast<std::chrono::minutes>(time_left);
      std::chrono::seconds seconds_left = std::chrono::duration_cast<std::chrono::seconds>(time_left - minutes_left);
      std::cout << "\r\033[F"<< " " << ievt << " events  "<< minutes_left.count() << ":";
      printf("%02d left   %.1f%%            \n",int(seconds_left.count()),percent_done*100);
    }

    atree->GetEntry(ievt);

    TBfastmode adata_ps = anevt->data(pscid);
    TBfastmode adata_mu = anevt->data(mucid);
    TBfastmode adata_tc = anevt->data(tccid);

    std::vector<TBfastmode> DWC1_fast;
    for ( int i = 0; i < dwc1.size(); i++ )
      DWC1_fast.push_back(anevt->data(dwc1.at(i)));

    std::vector<TBfastmode> DWC2_fast;
    for ( int i = 0; i < dwc2.size(); i++ )
      DWC2_fast.push_back(anevt->data(dwc2.at(i)));

    std::vector<TBfastmode> M2TC_data;
    std::vector<TBfastmode> M2TS_data;
    for ( int i = 0; i < 8; i++ ) {
      M2TC_data.push_back(anevt->data(M2TC.at(i)));
      M2TS_data.push_back(anevt->data(M2TS.at(i)));
    }

    std::vector<TBfastmode> M1TC_data;
    std::vector<TBfastmode> M1TS_data;
    for ( int i = 0; i < M1TC.size(); i++ ) {
      M1TC_data.push_back(anevt->data(M1TC.at(i)));
      M1TS_data.push_back(anevt->data(M1TS.at(i)));
    }

    // // DWC1_2 -> Check !

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

    // psdVSmoduleC->Fill(psADC, M1TC_data.at(1).adc());
    // psdVSmoduleS->Fill(psADC, M1TS_data.at(1).adc());

  //   if ( takingCut == 1) {
  //     if ( !inVetoWithAlign(centerPos) ) continue;
  //     if ( !inDWCaxisAlign(dwc_pos, 2) ) continue;
  //     if ( getPID(psADC, muADC) != 11 ) continue;
  // }

    psdVSmoduleC->Fill(psADC, M1TC_data.at(1).adc());
    psdVSmoduleS->Fill(psADC, M1TS_data.at(1).adc());


    // dwc_X_diff->Fill(dwc_pos.at(0) - dwc_pos.at(2));
    // dwc_Y_diff->Fill(dwc_pos.at(1) - dwc_pos.at(3));

    // dwc_X_diff_abs->Fill(std::abs(dwc_pos.at(0) - dwc_pos.at(2)));
    // dwc_Y_diff_abs->Fill(std::abs(dwc_pos.at(1) - dwc_pos.at(3)));

    // dwc_center_diff->Fill(lecDiff);
    // dwc_center_diff_angle->Fill(angleDiff);
    // dwc1_centerPos->Fill(centerPos.at(0));
    // dwc2_centerPos->Fill(centerPos.at(1));

    // dwc1pos->Fill(dwc_pos.at(0), dwc_pos.at(1));
    // dwc2pos->Fill(dwc_pos.at(2), dwc_pos.at(3));
    // dwc1vs2x->Fill(dwc_pos.at(0), dwc_pos.at(2));
    // dwc1vx2y->Fill(dwc_pos.at(1), dwc_pos.at(3));

    // dwc1_X->Fill(dwc_pos.at(0));
    // dwc1_Y->Fill(dwc_pos.at(1));
    // dwc2_X->Fill(dwc_pos.at(2));
    // dwc2_Y->Fill(dwc_pos.at(3));

    // M1T1C_origin->Fill(M1TC_data.at(0).adc());
    // M1T2C_origin->Fill(M1TC_data.at(1).adc());
    // M1T3C_origin->Fill(M1TC_data.at(2).adc());
    // M1T4C_origin->Fill(M1TC_data.at(3).adc());

    // M1T1S_origin->Fill(M1TS_data.at(0).adc());
    // M1T2S_origin->Fill(M1TS_data.at(1).adc());
    // M1T3S_origin->Fill(M1TS_data.at(2).adc());
    // M1T4S_origin->Fill(M1TS_data.at(3).adc());

    // M2T1C_origin->Fill(M2TC_data.at(0).adc());
    // M2T2C_origin->Fill(M2TC_data.at(1).adc());
    // M2T3C_origin->Fill(M2TC_data.at(2).adc());
    // M2T4C_origin->Fill(M2TC_data.at(3).adc());
    // M2T6C_origin->Fill(M2TC_data.at(4).adc());
    // M2T7C_origin->Fill(M2TC_data.at(5).adc());
    // M2T8C_origin->Fill(M2TC_data.at(6).adc());
    // M2T9C_origin->Fill(M2TC_data.at(7).adc());

    // M2T1S_origin->Fill(M2TS_data.at(0).adc());
    // M2T2S_origin->Fill(M2TS_data.at(1).adc());
    // M2T3S_origin->Fill(M2TS_data.at(2).adc());
    // M2T4S_origin->Fill(M2TS_data.at(3).adc());
    // M2T6S_origin->Fill(M2TS_data.at(4).adc());
    // M2T7S_origin->Fill(M2TS_data.at(5).adc());
    // M2T8S_origin->Fill(M2TS_data.at(6).adc());
    // M2T9S_origin->Fill(M2TS_data.at(7).adc());

    // dataTree->Fill();

    // muFast->Fill(muADC);
    // psFast->Fill(psADC);
    // tcFast->Fill(tcADC);
  } // event loop

  // c->cd(); DWC1D1->Draw("Hist"); c->SaveAs(runNum + "_DWC1_R_" + outputname + ".png");
  // c->cd(); DWC1D2->Draw("Hist"); c->SaveAs(runNum + "_DWC1_L_" + outputname + ".png");
  // c->cd(); DWC1D3->Draw("Hist"); c->SaveAs(runNum + "_DWC1_U_" + outputname + ".png");
  // c->cd(); DWC1D4->Draw("Hist"); c->SaveAs(runNum + "_DWC1_D_" + outputname + ".png");
  // c->cd(); DWC2D1->Draw("Hist"); c->SaveAs(runNum + "_DWC2_R_" + outputname + ".png");
  // c->cd(); DWC2D2->Draw("Hist"); c->SaveAs(runNum + "_DWC2_L_" + outputname + ".png");
  // c->cd(); DWC2D3->Draw("Hist"); c->SaveAs(runNum + "_DWC2_U_" + outputname + ".png");
  // c->cd(); DWC2D4->Draw("Hist"); c->SaveAs(runNum + "_DWC2_D_" + outputname + ".png");

  // c->cd(); DWC1RL->Draw("Hist"); c->SaveAs(runNum + "_DWC1_RL_" + outputname + ".png");
  // c->cd(); DWC1DU->Draw("Hist"); c->SaveAs(runNum + "_DWC1_UD_" + outputname + ".png");
  // c->cd(); DWC2RL->Draw("Hist"); c->SaveAs(runNum + "_DWC2_RL_" + outputname + ".png");
  // c->cd(); DWC3DU->Draw("Hist"); c->SaveAs(runNum + "_DWC2_UD_" + outputname + ".png");
 
  // afile->Close();

  // outputFile->cd();
  // dataTree->Write();
  // outputFile->Close();

  // TCanvas* c = new TCanvas("","");

  // c->cd(); muPeak->Draw("Hist"); c->SaveAs(runNum + "_MU_" + outputname + ".png");
  // c->cd(); psPeak->Draw("Hist"); c->SaveAs(runNum + "_PS_" + outputname + ".png");
  // c->cd(); psCutMuPeak->Draw("Hist"); c->SaveAs(runNum + "_MU_" + outputname + ".png");

  // TString addonName = "angleDiff_under0p01_mu_underMeanPlusOneSigma_ps_over2p5mip";
  // TString addonName = "angleDiff_under0p01_mu_underMeanPlusOneSigma_ps_over1p5mip";
  // TString addonName = "angleDiff_under0p01_mu_underMeanPlusOneSigma";
  // TString addonName = "angleDiff_under0p01";
  TString addonName = "origin";

  if (takingCut == 1)
    addonName = addonName + "_cut";

  // TString addonName = "angleDiff_under0p4_mu_underMeanPlusOneSigma_ps_over2p5mip";
  // TString addonName = "angleDiff_under0p4_mu_underMeanPlusOneSigma_ps_over1p5mip";
  // TString addonName = "angleDiff_under0p4_mu_underMeanPlusOneSigma";

  // TString addonName = "inVeto";
  // TString addonName = "inVeto_angleDiff_under0p42_MuCut";
  // TString addonName = "inVeto_angleDiff_under0p42_MuCut_PSover2p5mip";
  // TString addonName = "inVeto_angleDiff_under0p42_MuCut_PSover2p5mip_TClower5000ADC";

  // c->cd(); dwc_center_diff->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_DWC_center_diff_modified_" + outputname + ".png");
  // c->cd(); dwc_center_diff_angle->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_DWC_center_diffAngle_modified_" + outputname + ".png");
  // c->cd(); dwc1_centerPos->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_DWC1_centerPos_" + outputname + ".png");
  // c->cd(); dwc2_centerPos->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_DWC2_centerPos_" + outputname + ".png");

  // c->cd(); dwc_X_diff->Draw("colz"); c->SaveAs("./221108_pngs/" + addonName + "_DWC_X_diff_modified_" + outputname + ".png");
  // c->cd(); dwc_Y_diff->Draw("colz"); c->SaveAs("./221108_pngs/" + addonName + "_DWC_Y_diff_modified_" + outputname + ".png");

  // c->cd(); dwc_X_diff_abs->Draw("colz"); c->SaveAs("./221108_pngs/" + addonName + "_DWC_X_diff_abs_modified_" + outputname + ".png");
  // c->cd(); dwc_Y_diff_abs->Draw("colz"); c->SaveAs("./221108_pngs/" + addonName + "_DWC_Y_diff_abs_modified_" + outputname + ".png");

  // c->cd(); dwc1pos->Draw("colz"); c->SaveAs("./221108_pngs/" + addonName + "_DWC1_XvsY_modified_" + outputname + ".png");
  // c->cd(); dwc2pos->Draw("colz"); c->SaveAs("./221108_pngs/" + addonName + "_DWC2_XvsY_modified_" + outputname + ".png");
  // c->cd(); dwc1vs2x->Draw("colz"); c->SaveAs("./221108_pngs/" + addonName + "_DWC_X_1vs2_modified_" + outputname + ".png");
  // c->cd(); dwc1vx2y->Draw("colz"); c->SaveAs("./221108_pngs/" + addonName + "_DWC_Y_1vs2_modified_" + outputname + ".png");

  // c->cd(); dwc1_X->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_DWC1_X_modified_" + outputname + ".png");
  // c->cd(); dwc1_Y->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_DWC1_Y_modified_" + outputname + ".png");
  // c->cd(); dwc2_X->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_DWC2_X_modified_" + outputname + ".png");
  // c->cd(); dwc2_Y->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_DWC2_Y_modified_" + outputname + ".png");

  // c->cd(); muFast->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_MU_Fast_" + outputname + ".png");
  // c->cd(); psFast->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_PS_Fast_" + outputname + ".png");
  // c->cd(); tcFast->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_TC_Fast_" + outputname + ".png");

  // c->cd(); muFast->Draw("Hist"); grE_Mu->Draw("sames+L"); c->SaveAs("./221108_pngs/" + addonName + "_MU_Fast_Fit_" + outputname + ".png");
  // c->cd(); psFast->Draw("Hist"); grE_Ps->Draw("sames+L"); c->SaveAs("./221108_pngs/" + addonName + "_PS_Fast_Fit_" + outputname + ".png");

  // c->cd(); M1T1C_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M1T1C_Fast_" + outputname + ".png");
  // c->cd(); M1T2C_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M1T2C_Fast_" + outputname + ".png");
  // c->cd(); M1T3C_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M1T3C_Fast_" + outputname + ".png");
  // c->cd(); M1T4C_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M1T4C_Fast_" + outputname + ".png");

  // c->cd(); M1T1S_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M1T1S_Fast_" + outputname + ".png");
  // c->cd(); M1T2S_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M1T2S_Fast_" + outputname + ".png");
  // c->cd(); M1T3S_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M1T3S_Fast_" + outputname + ".png");
  // c->cd(); M1T4S_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M1T4S_Fast_" + outputname + ".png");

  // c->cd(); M2T1C_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M2T1C_Fast_" + outputname + ".png");
  // c->cd(); M2T2C_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M2T2C_Fast_" + outputname + ".png");
  // c->cd(); M2T3C_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M2T3C_Fast_" + outputname + ".png");
  // c->cd(); M2T4C_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M2T4C_Fast_" + outputname + ".png");
  // c->cd(); M2T6C_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M2T6C_Fast_" + outputname + ".png");
  // c->cd(); M2T7C_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M2T7C_Fast_" + outputname + ".png");
  // c->cd(); M2T8C_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M2T8C_Fast_" + outputname + ".png");
  // c->cd(); M2T9C_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M2T9C_Fast_" + outputname + ".png");

  // c->cd(); M2T1S_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M2T1S_Fast_" + outputname + ".png");
  // c->cd(); M2T2S_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M2T2S_Fast_" + outputname + ".png");
  // c->cd(); M2T3S_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M2T3S_Fast_" + outputname + ".png");
  // c->cd(); M2T4S_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M2T4S_Fast_" + outputname + ".png");
  // c->cd(); M2T6S_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M2T6S_Fast_" + outputname + ".png");
  // c->cd(); M2T7S_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M2T7S_Fast_" + outputname + ".png");
  // c->cd(); M2T8S_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M2T8S_Fast_" + outputname + ".png");
  // c->cd(); M2T9S_origin->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_M2T9S_Fast_" + outputname + ".png");

  c->cd(); psdVSmoduleC->Draw("colz"); c->SaveAs("./221108_pngs/" + addonName + "_psdVSmoduleC_" + outputname + ".png");
  c->cd(); psdVSmoduleS->Draw("colz"); c->SaveAs("./221108_pngs/" + addonName + "_psdVSmoduleS_" + outputname + ".png");

  // c->SetLogy(); 
  // c->cd(); muFast->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_MU_Fast_log_" + outputname + ".png");
  // c->cd(); psFast->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_PS_Fast_log_" + outputname + ".png");
  // c->cd(); tcFast->Draw("Hist"); c->SaveAs("./221108_pngs/" + addonName + "_TC_Fast_log_" + outputname + ".png");

  // c->cd(); muFast->Draw("Hist"); grE_Mu->Draw("sames+L"); c->SaveAs("./221108_pngs/" + addonName + "_MU_Fast_fit_log_" + outputname + ".png");
  // c->cd(); psFast->Draw("Hist"); grE_Ps->Draw("sames+L"); c->SaveAs("./221108_pngs/" + addonName + "_PS_Fast_fit_log_" + outputname + ".png");

  return 0;
}
