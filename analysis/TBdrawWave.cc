#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "TBevt.h"
#include "TButility.h"
#include "functions.cc"

#include "TROOT.h"
#include "TStyle.h"
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>

// This macro will draw single event waveform (without any PID cuts) from waveform ntuple
// How to execute
// On local or batch, run the following command :
// ./TBdrawWave.exe <run number> <max # of events to process>

int main(int argc, char** argv) {
    gStyle->SetOptFit(1);

    int runNum = std::stoi(argv[1]);
    int maxEntry = std::stoi(argv[2]);

    TButility utility = TButility();
    utility.loading("/gatbawi/dream/mapping/mapping_Aug2022TB.root");
    utility.loadped( ("/gatbawi/dream/ped/mean/Run" + std::to_string(runNum) + "_pedestalHist_mean.root").c_str() );

    TBcid pscid = utility.getcid(TBdetector::detid::preshower);
    TBcid mucid = utility.getcid(TBdetector::detid::muon);

    std::vector<TBcid> dwc1_cid;
    dwc1_cid.push_back(TBcid(1,17)); // Right
    dwc1_cid.push_back(TBcid(1,19)); // Left
    dwc1_cid.push_back(TBcid(1,21)); // Up
    dwc1_cid.push_back(TBcid(1,23)); // Down

    std::vector<TBcid> dwc2_cid;
    dwc2_cid.push_back(TBcid(1,25)); // R
    dwc2_cid.push_back(TBcid(1,27)); // L
    dwc2_cid.push_back(TBcid(1,29)); // U
    dwc2_cid.push_back(TBcid(1,31)); // D

    std::vector<TBcid> M1TC_cid;
    M1TC_cid.push_back(utility.getcid(TBdetector::detid::PMT, 1, 1, true));
    M1TC_cid.push_back(utility.getcid(TBdetector::detid::PMT, 1, 2, true));
    M1TC_cid.push_back(utility.getcid(TBdetector::detid::PMT, 1, 3, true));
    M1TC_cid.push_back(utility.getcid(TBdetector::detid::PMT, 1, 4, true));

    std::vector<TBcid> M1TS_cid;
    M1TS_cid.push_back(utility.getcid(TBdetector::detid::PMT, 1, 1, false));
    M1TS_cid.push_back(utility.getcid(TBdetector::detid::PMT, 1, 2, false));
    M1TS_cid.push_back(utility.getcid(TBdetector::detid::PMT, 1, 3, false));
    M1TS_cid.push_back(utility.getcid(TBdetector::detid::PMT, 1, 4, false));

    std::vector<TBcid> M2TC_cid;
    M2TC_cid.push_back(utility.getcid(TBdetector::detid::PMT, 2, 1, true));
    M2TC_cid.push_back(utility.getcid(TBdetector::detid::PMT, 2, 2, true));
    M2TC_cid.push_back(utility.getcid(TBdetector::detid::PMT, 2, 3, true));
    M2TC_cid.push_back(utility.getcid(TBdetector::detid::PMT, 2, 4, true));
    M2TC_cid.push_back(utility.getcid(TBdetector::detid::PMT, 2, 6, true));
    M2TC_cid.push_back(utility.getcid(TBdetector::detid::PMT, 2, 7, true));
    M2TC_cid.push_back(utility.getcid(TBdetector::detid::PMT, 2, 8, true));
    M2TC_cid.push_back(utility.getcid(TBdetector::detid::PMT, 2, 9, true));

    std::vector<TBcid> M2TS_cid;
    M2TS_cid.push_back(utility.getcid(TBdetector::detid::PMT, 2, 1, false));
    M2TS_cid.push_back(utility.getcid(TBdetector::detid::PMT, 2, 2, false));
    M2TS_cid.push_back(utility.getcid(TBdetector::detid::PMT, 2, 3, false));
    M2TS_cid.push_back(utility.getcid(TBdetector::detid::PMT, 2, 4, false));
    M2TS_cid.push_back(utility.getcid(TBdetector::detid::PMT, 2, 6, false));
    M2TS_cid.push_back(utility.getcid(TBdetector::detid::PMT, 2, 7, false));
    M2TS_cid.push_back(utility.getcid(TBdetector::detid::PMT, 2, 8, false));
    M2TS_cid.push_back(utility.getcid(TBdetector::detid::PMT, 2, 9, false));

    // Load data using TChain
    TChain* evtChain = getNtupleChain(runNum);
    TBevt<TBwaveform>* anEvt = new TBevt<TBwaveform>(); 
    evtChain->SetBranchAddress("TBevt", &anEvt);

    int totalEntry = evtChain->GetEntries();
    std::cout << "Total entries : " << totalEntry << std::endl;
    if ( (maxEntry > 0) && (maxEntry < totalEntry) ) {
        totalEntry = maxEntry;
        std::cout << "Will process maximum " << std::to_string(totalEntry) << " events" << std::endl;
    }

    std::string outFile = "/u/user/swkim/pre-exercise/dual-readout_TB/analysis/waveform/Waveform_Run_" + std::to_string(runNum) + ".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");
    outputRoot->cd();

    // Evt Loop
    for (int iEvt = 0; iEvt < totalEntry; iEvt++) {
        evtChain->GetEntry(iEvt);

        TH1F* psWaveformHist = new TH1F( ( "psWaveform_" + std::to_string(iEvt) ).c_str(), ("psWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        psWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* muWaveformHist = new TH1F( ( "muWaveform_" + std::to_string(iEvt) ).c_str(), ("muWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        muWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);

        TH1F* dwc1RWaveformHist = new TH1F( ( "dwc1RWaveform_" + std::to_string(iEvt) ).c_str(), ("dwc1RWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        dwc1RWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* dwc1LWaveformHist = new TH1F( ( "dwc1LWaveform_" + std::to_string(iEvt) ).c_str(), ("dwc1LWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        dwc1LWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* dwc1UWaveformHist = new TH1F( ( "dwc1UWaveform_" + std::to_string(iEvt) ).c_str(), ("dwc1UWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        dwc1UWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* dwc1DWaveformHist = new TH1F( ( "dwc1DWaveform_" + std::to_string(iEvt) ).c_str(), ("dwc1DWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        dwc1DWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);

        TH1F* dwc2RWaveformHist = new TH1F( ( "dwc2RWaveform_" + std::to_string(iEvt) ).c_str(), ("dwc2RWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        dwc2RWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* dwc2LWaveformHist = new TH1F( ( "dwc2LWaveform_" + std::to_string(iEvt) ).c_str(), ("dwc2LWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        dwc2LWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* dwc2UWaveformHist = new TH1F( ( "dwc2UWaveform_" + std::to_string(iEvt) ).c_str(), ("dwc2UWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        dwc2UWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* dwc2DWaveformHist = new TH1F( ( "dwc2DWaveform_" + std::to_string(iEvt) ).c_str(), ("dwc2DWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        dwc2DWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);

        TH1F* M1T1CWaveformHist = new TH1F( ( "M1T1CWaveform_" + std::to_string(iEvt) ).c_str(), ("M1T1CWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M1T1CWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M1T2CWaveformHist = new TH1F( ( "M1T2CWaveform_" + std::to_string(iEvt) ).c_str(), ("M1T2CWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M1T2CWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M1T3CWaveformHist = new TH1F( ( "M1T3CWaveform_" + std::to_string(iEvt) ).c_str(), ("M1T3CWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M1T3CWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M1T4CWaveformHist = new TH1F( ( "M1T4CWaveform_" + std::to_string(iEvt) ).c_str(), ("M1T4CWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M1T4CWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);

        TH1F* M1T1SWaveformHist = new TH1F( ( "M1T1SWaveform_" + std::to_string(iEvt) ).c_str(), ("M1T1SWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M1T1SWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M1T2SWaveformHist = new TH1F( ( "M1T2SWaveform_" + std::to_string(iEvt) ).c_str(), ("M1T2SWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M1T2SWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M1T3SWaveformHist = new TH1F( ( "M1T3SWaveform_" + std::to_string(iEvt) ).c_str(), ("M1T3SWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M1T3SWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M1T4SWaveformHist = new TH1F( ( "M1T4SWaveform_" + std::to_string(iEvt) ).c_str(), ("M1T4SWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M1T4SWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);

        TH1F* M2T1CWaveformHist = new TH1F( ( "M2T1CWaveform_" + std::to_string(iEvt) ).c_str(), ("M2T1CWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M2T1CWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M2T2CWaveformHist = new TH1F( ( "M2T2CWaveform_" + std::to_string(iEvt) ).c_str(), ("M2T2CWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M2T2CWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M2T3CWaveformHist = new TH1F( ( "M2T3CWaveform_" + std::to_string(iEvt) ).c_str(), ("M2T3CWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M2T3CWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M2T4CWaveformHist = new TH1F( ( "M2T4CWaveform_" + std::to_string(iEvt) ).c_str(), ("M2T4CWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M2T4CWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M2T6CWaveformHist = new TH1F( ( "M2T6CWaveform_" + std::to_string(iEvt) ).c_str(), ("M2T6CWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M2T6CWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M2T7CWaveformHist = new TH1F( ( "M2T7CWaveform_" + std::to_string(iEvt) ).c_str(), ("M2T7CWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M2T7CWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M2T8CWaveformHist = new TH1F( ( "M2T8CWaveform_" + std::to_string(iEvt) ).c_str(), ("M2T8CWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M2T8CWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M2T9CWaveformHist = new TH1F( ( "M2T9CWaveform_" + std::to_string(iEvt) ).c_str(), ("M2T9CWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M2T9CWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);

        TH1F* M2T1SWaveformHist = new TH1F( ( "M2T1SWaveform_" + std::to_string(iEvt) ).c_str(), ("M2T1SWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M2T1SWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M2T2SWaveformHist = new TH1F( ( "M2T2SWaveform_" + std::to_string(iEvt) ).c_str(), ("M2T2SWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M2T2SWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M2T3SWaveformHist = new TH1F( ( "M2T3SWaveform_" + std::to_string(iEvt) ).c_str(), ("M2T3SWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M2T3SWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M2T4SWaveformHist = new TH1F( ( "M2T4SWaveform_" + std::to_string(iEvt) ).c_str(), ("M2T4SWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M2T4SWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M2T6SWaveformHist = new TH1F( ( "M2T6SWaveform_" + std::to_string(iEvt) ).c_str(), ("M2T6SWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M2T6SWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M2T7SWaveformHist = new TH1F( ( "M2T7SWaveform_" + std::to_string(iEvt) ).c_str(), ("M2T7SWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M2T7SWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M2T8SWaveformHist = new TH1F( ( "M2T8SWaveform_" + std::to_string(iEvt) ).c_str(), ("M2T8SWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M2T8SWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
        TH1F* M2T9SWaveformHist = new TH1F( ( "M2T9SWaveform_" + std::to_string(iEvt) ).c_str(), ("M2T9SWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        M2T9SWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);

        TBwaveform psData = anEvt->data(pscid);
        TBwaveform muData = anEvt->data(mucid);

        std::vector<TBwaveform> dwc1_data;
        for (TBcid cid : dwc1_cid) {
            dwc1_data.push_back(anEvt->data(cid));
        }
        std::vector<TBwaveform> dwc2_data;
        for (TBcid cid : dwc2_cid) {
            dwc2_data.push_back(anEvt->data(cid));
        }

        std::vector<TBwaveform> M1TC_data;
        for (TBcid cid : M1TC_cid) {
            M1TC_data.push_back(anEvt->data(cid));
        }
        std::vector<TBwaveform> M1TS_data;
        for (TBcid cid : M1TS_cid) {
            M1TS_data.push_back(anEvt->data(cid));
        }

        std::vector<TBwaveform> M2TC_data;
        for (TBcid cid : M2TC_cid) {
            M2TC_data.push_back(anEvt->data(cid));
        }
        std::vector<TBwaveform> M2TS_data;
        for (TBcid cid : M2TS_cid) {
            M2TS_data.push_back(anEvt->data(cid));
        }

        std::vector<short> psWaveform = psData.waveform();
        std::vector<short> muWaveform = muData.waveform();

        std::vector< std::vector<short> > dwc1_waveform;
        for (TBwaveform data : dwc1_data) {
            dwc1_waveform.push_back(data.waveform());
        }
        std::vector< std::vector<short> > dwc2_waveform;
        for (TBwaveform data : dwc2_data) {
            dwc2_waveform.push_back(data.waveform());
        }

        std::vector< std::vector<short> > M1TC_waveform;
        for (TBwaveform data : M1TC_data) {
            M1TC_waveform.push_back(data.waveform());
        }
        std::vector< std::vector<short> > M1TS_waveform;
        for (TBwaveform data : M1TS_data) {
            M1TS_waveform.push_back(data.waveform());
        }

        std::vector< std::vector<short> > M2TC_waveform;
        for (TBwaveform data : M2TC_data) {
            M2TC_waveform.push_back(data.waveform());
        }
        std::vector< std::vector<short> > M2TS_waveform;
        for (TBwaveform data : M2TS_data) {
            M2TS_waveform.push_back(data.waveform());
        }

        for (int bin = 0; bin < 1000; bin++) {
            int waveformBin = bin + 1;

            psWaveformHist->Fill(bin, psWaveform[waveformBin]);
            muWaveformHist->Fill(bin, muWaveform[waveformBin]);
            
            dwc1RWaveformHist->Fill(bin, dwc1_waveform.at(0)[waveformBin]);
            dwc1LWaveformHist->Fill(bin, dwc1_waveform.at(1)[waveformBin]);
            dwc1UWaveformHist->Fill(bin, dwc1_waveform.at(2)[waveformBin]);
            dwc1DWaveformHist->Fill(bin, dwc1_waveform.at(3)[waveformBin]);

            dwc2RWaveformHist->Fill(bin, dwc2_waveform.at(0)[waveformBin]);
            dwc2LWaveformHist->Fill(bin, dwc2_waveform.at(1)[waveformBin]);
            dwc2UWaveformHist->Fill(bin, dwc2_waveform.at(2)[waveformBin]);
            dwc2DWaveformHist->Fill(bin, dwc2_waveform.at(3)[waveformBin]);

            M1T1CWaveformHist->Fill(bin, M1TC_waveform.at(0)[waveformBin]);
            M1T2CWaveformHist->Fill(bin, M1TC_waveform.at(1)[waveformBin]);
            M1T3CWaveformHist->Fill(bin, M1TC_waveform.at(2)[waveformBin]);
            M1T4CWaveformHist->Fill(bin, M1TC_waveform.at(3)[waveformBin]);

            M1T1SWaveformHist->Fill(bin, M1TS_waveform.at(0)[waveformBin]);
            M1T2SWaveformHist->Fill(bin, M1TS_waveform.at(1)[waveformBin]);
            M1T3SWaveformHist->Fill(bin, M1TS_waveform.at(2)[waveformBin]);
            M1T4SWaveformHist->Fill(bin, M1TS_waveform.at(3)[waveformBin]);

            M2T1CWaveformHist->Fill(bin, M2TC_waveform.at(0)[waveformBin]);
            M2T2CWaveformHist->Fill(bin, M2TC_waveform.at(1)[waveformBin]);
            M2T3CWaveformHist->Fill(bin, M2TC_waveform.at(2)[waveformBin]);
            M2T4CWaveformHist->Fill(bin, M2TC_waveform.at(3)[waveformBin]);
            M2T6CWaveformHist->Fill(bin, M2TC_waveform.at(4)[waveformBin]);
            M2T7CWaveformHist->Fill(bin, M2TC_waveform.at(5)[waveformBin]);
            M2T8CWaveformHist->Fill(bin, M2TC_waveform.at(6)[waveformBin]);
            M2T9CWaveformHist->Fill(bin, M2TC_waveform.at(7)[waveformBin]);

            M2T1SWaveformHist->Fill(bin, M2TS_waveform.at(0)[waveformBin]);
            M2T2SWaveformHist->Fill(bin, M2TS_waveform.at(1)[waveformBin]);
            M2T3SWaveformHist->Fill(bin, M2TS_waveform.at(2)[waveformBin]);
            M2T4SWaveformHist->Fill(bin, M2TS_waveform.at(3)[waveformBin]);
            M2T6SWaveformHist->Fill(bin, M2TS_waveform.at(4)[waveformBin]);
            M2T7SWaveformHist->Fill(bin, M2TS_waveform.at(5)[waveformBin]);
            M2T8SWaveformHist->Fill(bin, M2TS_waveform.at(6)[waveformBin]);
            M2T9SWaveformHist->Fill(bin, M2TS_waveform.at(7)[waveformBin]);
        }
        
        psWaveformHist->Write();
        muWaveformHist->Write();

        dwc1RWaveformHist->Write();
        dwc1LWaveformHist->Write();
        dwc1UWaveformHist->Write();
        dwc1DWaveformHist->Write();

        dwc2RWaveformHist->Write();
        dwc2LWaveformHist->Write();
        dwc2UWaveformHist->Write();
        dwc2DWaveformHist->Write();

        M1T1CWaveformHist->Write();
        M1T2CWaveformHist->Write();
        M1T3CWaveformHist->Write();
        M1T4CWaveformHist->Write();

        M1T1SWaveformHist->Write();
        M1T2SWaveformHist->Write();
        M1T3SWaveformHist->Write();
        M1T4SWaveformHist->Write();

        M2T1CWaveformHist->Write();
        M2T2CWaveformHist->Write();
        M2T3CWaveformHist->Write();
        M2T4CWaveformHist->Write();
        M2T6CWaveformHist->Write();
        M2T7CWaveformHist->Write();
        M2T8CWaveformHist->Write();
        M2T9CWaveformHist->Write();

        M2T1SWaveformHist->Write();
        M2T2SWaveformHist->Write();
        M2T3SWaveformHist->Write();
        M2T4SWaveformHist->Write();
        M2T6SWaveformHist->Write();
        M2T7SWaveformHist->Write();
        M2T8SWaveformHist->Write();
        M2T9SWaveformHist->Write();

        if ( (iEvt % 1000) == 0 ) printProgress(iEvt + 1, totalEntry);
    }

    outputRoot->Close();
}
