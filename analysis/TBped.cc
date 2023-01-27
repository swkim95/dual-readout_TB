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

  TBload<TBwaveform>* loaderWave = new TBload<TBwaveform>(runNum);

  int maxEvents = std::stoi(argv[2]); 

  TButility utility = TButility();
  utility.loading("mapping_data_MCPPMT_positiveSignal_v3.csv");
  utility.loadped("ped_400.csv");

  if ( maxEvents == -1 )
    maxEvents = loaderWave->getEntries();

  if ( maxEvents > loaderWave->getEntries() )
    throw std::runtime_error("Events number is larger than entries !");

  std::chrono::time_point time_begin = std::chrono::system_clock::now(); //delete
  printf("\n");

  std::vector< std::vector<TH1F*> > pedCollection;

  for ( int i = 0; i < 15; i++ ) {

    std::vector<TH1F*> tmpVec;
    for ( int j = 0; j < 32; j++ )
      tmpVec.push_back(new TH1F((TString)("mid" + std::to_string(i+1) + "_" + "ch" + std::to_string(j+1)), ";;", 1500, 3000., 4500.));

    pedCollection.push_back(tmpVec);

  }

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

    loaderWave->loading();
    auto aWaveEvt = loaderWave->getEvent();

    for ( int iMid = 0; iMid < 15; iMid++ ) {
      auto midCol = aWaveEvt->mid(iMid);

      for ( int iCh = 0; iCh < 32; iCh++ ) {
        auto ch = midCol.channel(iCh);
        auto wave = ch.waveform();

        float ped = 0;
        for ( int iTmp = 0; iTmp < 100; iTmp++ ) {
          // ped += (float)wave.at(iTmp + 10) / 100.;
          pedCollection.at(iMid).at(iCh)->Fill((float)wave.at(iTmp + 10));
        }

        // pedCollection.at(iMid).at(iCh)->Fill(ped);

      }
    }

  }
  loaderWave->close();

  std::vector< std::vector<float> > pedMeanCol;
  for ( int i = 0; i < 15; i++ ) {
    std::vector<float> tmpVec;

    for ( int j = 0; j < 32; j++ )
      tmpVec.push_back(0.);

    pedMeanCol.push_back(tmpVec);

  }

  TFile* rootOutput = new TFile("ped.root", "RECREATE");
  rootOutput->cd();
  for ( int iMid = 0; iMid < 15; iMid++ ) {
    for ( int iCh = 0; iCh < 32; iCh++ ) {

      TF1* pedFit = new TF1((TString)("fit_mid" + std::to_string(iMid+1) + "_" + "ch" + std::to_string(iCh+1)), "gaus", 3000., 4000.); 
      pedFit->SetLineColor(kBlack);
      pedCollection.at(iMid).at(iCh)->SetOption("p"); 
      pedCollection.at(iMid).at(iCh)->Fit(pedFit,"R+&same");
      pedMeanCol.at(iMid).at(iCh) = pedFit->GetParameter("Mean");

      pedCollection.at(iMid).at(iCh)->Write();
      pedFit->Write();

    }
  }
  rootOutput->Close();

  for ( int iMid = 0; iMid < 15; iMid++ ) {
    for ( int iCh = 0; iCh < 32; iCh++ ) {
      std::cout << iMid + 1 << " "
                << iCh + 1 << " "
                << pedMeanCol.at(iMid).at(iCh) << std::endl;
    }
  }

  return 0;
}