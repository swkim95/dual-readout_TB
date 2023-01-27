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
#include <TGraph.h>

int main(int argc, char** argv) {

  gStyle->SetOptFit(1);

  int runNum = std::stoi(argv[1]);

  TBload<TBwaveform>* loaderWave = new TBload<TBwaveform>(runNum);

  int maxEvents = std::stoi(argv[2]); 


  TButility utility = TButility();
  utility.loading("mapping_data_MCPPMT_positiveSignal_v3.csv");
  utility.loadped("Run624_ped.csv");

  std::vector<TBcid> dwc1;
  dwc1.push_back(TBcid(1,17));

  TH1I* DWC1R = new TH1I("DWC1 R", "DWC1 R;ns;ADC", 1000, 0., 400.);
  DWC1R->SetStats(0);
  DWC1R->SetLineWidth(2);
  DWC1R->SetLineColor(kBlack);

  if ( maxEvents == -1 )
    maxEvents = loaderWave->getEntries();

  if ( maxEvents > loaderWave->getEntries() )
    throw std::runtime_error("Events number is larger than entries !");


  TCanvas* c = new TCanvas("", "");
  for ( int ievt = 0; ievt < maxEvents; ievt++ ) {

    loaderWave->loading();
    auto aWaveEvt = loaderWave->getEvent();
    DWC1R->Reset();

    TBwaveform DWC1R_wave = aWaveEvt->data(dwc1.at(0));
    auto DWC1R_waveform = DWC1R_wave.pedcorrectedWaveform(utility.retrievePed(dwc1.at(0)));

    float pedeint = utility.retrievePed(dwc1.at(0));

    float leTime = getDWCtimeWave(DWC1R_wave, pedeint);
    float peakTime = getTime(std::max_element(DWC1R_waveform.begin()+1, DWC1R_waveform.end()-23) - DWC1R_waveform.begin() -1);

    float leADC = (float)*std::max_element(DWC1R_waveform.begin()+1, DWC1R_waveform.end()-23) * 0.1;
    float peakADC = (float)*std::max_element(DWC1R_waveform.begin()+1, DWC1R_waveform.end()-23);

    for ( int i = 0; i < 1000; i++ ) {
      DWC1R->SetBinContent(i+1, DWC1R_waveform.at(i+1));
      DWC1R->SetBinError(i+1, 1e-10);
    }

    TGraph* leHori = new TGraph(); leHori->SetLineWidth(2); leHori->SetLineColor(kGreen+1);
    leHori->SetPoint(0, 0., leADC);
    leHori->SetPoint(1, 400., leADC);

    TGraph* peakHori = new TGraph(); peakHori->SetLineWidth(2); peakHori->SetLineColor(kOrange+2);
    peakHori->SetPoint(0, 0., peakADC);
    peakHori->SetPoint(1, 400., peakADC);

    TGraph* leVert = new TGraph(); leVert->SetLineWidth(2); leVert->SetLineColor(kGreen+1);
    leVert->SetPoint(0, leTime, -1000.);
    leVert->SetPoint(1, leTime, 5000.);

    TGraph* peakVert = new TGraph(); peakVert->SetLineWidth(2); peakVert->SetLineColor(kOrange+2);
    peakVert->SetPoint(0, peakTime, -1000.);
    peakVert->SetPoint(1, peakTime, 5000.);

    TGraph* pedLine = new TGraph(); pedLine->SetLineWidth(2); pedLine->SetLineColor(kRed);
    pedLine->SetPoint(0, 0., 0.);
    pedLine->SetPoint(1, 400., 0.);



    c->cd();

    DWC1R->Draw("Hist");
    pedLine->Draw("l&sames");
    leHori->Draw("l&sames");
    peakHori->Draw("l&sames");
    leVert->Draw("l&sames");
    peakVert->Draw("l&sames");

    c->SaveAs((TString)("./230125/pngs/waveform/" + std::to_string(ievt) + ".png"));

  }
  loaderWave->close();


  return 0;
}
