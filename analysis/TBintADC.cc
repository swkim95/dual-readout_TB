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

int main(int argc, char** argv) {
    gStyle->SetOptFit(1);

    int runNum = std::stoi(argv[1]);
    int maxEntry = std::stoi(argv[2]);

    // Load mapping & pedestal info
    TButility utility = TButility();
    utility.loading("/gatbawi/dream/mapping/mapping_Aug2022TB.root");
    utility.loadped( ("/gatbawi/dream/ped/mean/Run" + std::to_string(runNum) + "_pedestalHist_mean.root").c_str() );
    // Get PID info
    TFile* pidFile = TFile::Open(("/u/user/swkim/pre-exercise/dual-readout_TB/analysis/auxPID/auxPID_Run_" + std::to_string(runNum) + ".root").c_str());
    if (pidFile->IsOpen()) {
        std::cout << "Opened PID info : /u/user/swkim/pre-exercise/dual-readout_TB/analysis/auxPID/auxPID_Run_" + std::to_string(runNum) + ".root" << std::endl;
    }
    else {
        std::cout << "[ERROR] Failed to open PID info : /u/user/swkim/pre-exercise/dual-readout_TB/analysis/auxPID/auxPID_Run_" + std::to_string(runNum) + ".root" << std::endl;
        return 0;
    }
    // Get channel IDs
    TBcid pscid = utility.getcid(TBdetector::detid::preshower);
    TBcid mucid = utility.getcid(TBdetector::detid::muon);

    std::vector<TBcid> dwc1_cid;
    dwc1_cid.push_back(TBcid(1,17)); // R
    dwc1_cid.push_back(TBcid(1,19)); // L
    dwc1_cid.push_back(TBcid(1,21)); // U
    dwc1_cid.push_back(TBcid(1,23)); // D

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

    // Get pedestals
    float psPed = utility.retrievePed(pscid);
    float muPed = utility.retrievePed(mucid);

    std::vector<float> M1TC_ped;
    for(TBcid cid : M1TC_cid) {
        M1TC_ped.push_back(utility.retrievePed(cid));
    }
    std::vector<float> M1TS_ped;
    for(TBcid cid : M1TS_cid) {
        M1TS_ped.push_back(utility.retrievePed(cid));
    }

    std::vector<float> M2TC_ped;
    for(TBcid cid : M2TC_cid) {
        M2TC_ped.push_back(utility.retrievePed(cid));
    }
    std::vector<float> M2TS_ped;
    for(TBcid cid : M2TS_cid) {
        M2TS_ped.push_back(utility.retrievePed(cid));
    }

    // Preparing histograms
    TH1F* psIntHist = new TH1F("psIntADC", "psIntADC;IntADC;evt", 1000, -5000., 200000.);
    TH1F* psIntHist_PID = new TH1F("psIntADC_PID", "psIntADC_PID;IntADC;evt", 1000, -5000., 200000.);

    TH1F* muIntHist = new TH1F("muIntADC", "muIntADC;IntADC;evt", 1000, -10000., 50000.);
    TH1F* muIntHist_PID = new TH1F("muIntADC_PID", "muIntADC_PID;IntADC;evt", 1000, -10000., 50000.);
    // M1 C
    TH1F* M1T1C_Hist = new TH1F("M1T1C", "M1T1C;IntADC;evt", 1000, -10000, 50000);
    TH1F* M1T2C_Hist = new TH1F("M1T2C", "M1T2C;IntADC;evt", 1000, -10000, 50000);
    TH1F* M1T3C_Hist = new TH1F("M1T3C", "M1T3C;IntADC;evt", 1000, -10000, 50000);
    TH1F* M1T4C_Hist = new TH1F("M1T4C", "M1T4C;IntADC;evt", 1000, -10000, 50000);

    TH1F* M1T1C_Hist_PID = new TH1F("M1T1C_PID", "M1T1C_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M1T2C_Hist_PID = new TH1F("M1T2C_PID", "M1T2C_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M1T3C_Hist_PID = new TH1F("M1T3C_PID", "M1T3C_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M1T4C_Hist_PID = new TH1F("M1T4C_PID", "M1T4C_PID;IntADC;evt", 1000, -10000, 50000);
    // M1 S
    TH1F* M1T1S_Hist = new TH1F("M1T1S", "M1T1S;IntADC;evt", 1000, -10000, 50000);
    TH1F* M1T2S_Hist = new TH1F("M1T2S", "M1T2S;IntADC;evt", 1000, -10000, 50000);
    TH1F* M1T3S_Hist = new TH1F("M1T3S", "M1T3S;IntADC;evt", 1000, -10000, 50000);
    TH1F* M1T4S_Hist = new TH1F("M1T4S", "M1T4S;IntADC;evt", 1000, -10000, 50000);

    TH1F* M1T1S_Hist_PID = new TH1F("M1T1S_PID", "M1T1S_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M1T2S_Hist_PID = new TH1F("M1T2S_PID", "M1T2S_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M1T3S_Hist_PID = new TH1F("M1T3S_PID", "M1T3S_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M1T4S_Hist_PID = new TH1F("M1T4S_PID", "M1T4S_PID;IntADC;evt", 1000, -10000, 50000);
    // M2 C
    TH1F* M2T1C_Hist = new TH1F("M2T1C", "M2T1C;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T2C_Hist = new TH1F("M2T2C", "M2T2C;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T3C_Hist = new TH1F("M2T3C", "M2T3C;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T4C_Hist = new TH1F("M2T4C", "M2T4C;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T6C_Hist = new TH1F("M2T6C", "M2T6C;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T7C_Hist = new TH1F("M2T7C", "M2T7C;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T8C_Hist = new TH1F("M2T8C", "M2T8C;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T9C_Hist = new TH1F("M2T9C", "M2T9C;IntADC;evt", 1000, -10000, 50000);

    TH1F* M2T1C_Hist_PID = new TH1F("M2T1C_PID", "M2T1C_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T2C_Hist_PID = new TH1F("M2T2C_PID", "M2T2C_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T3C_Hist_PID = new TH1F("M2T3C_PID", "M2T3C_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T4C_Hist_PID = new TH1F("M2T4C_PID", "M2T4C_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T6C_Hist_PID = new TH1F("M2T6C_PID", "M2T6C_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T7C_Hist_PID = new TH1F("M2T7C_PID", "M2T7C_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T8C_Hist_PID = new TH1F("M2T8C_PID", "M2T8C_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T9C_Hist_PID = new TH1F("M2T9C_PID", "M2T9C_PID;IntADC;evt", 1000, -10000, 50000);
    // M2 S
    TH1F* M2T1S_Hist = new TH1F("M2T1S", "M2T1S;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T2S_Hist = new TH1F("M2T2S", "M2T2S;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T3S_Hist = new TH1F("M2T3S", "M2T3S;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T4S_Hist = new TH1F("M2T4S", "M2T4S;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T6S_Hist = new TH1F("M2T6S", "M2T6S;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T7S_Hist = new TH1F("M2T7S", "M2T7S;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T8S_Hist = new TH1F("M2T8S", "M2T8S;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T9S_Hist = new TH1F("M2T9S", "M2T9S;IntADC;evt", 1000, -10000, 50000);

    TH1F* M2T1S_Hist_PID = new TH1F("M2T1S_PID", "M2T1S_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T2S_Hist_PID = new TH1F("M2T2S_PID", "M2T2S_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T3S_Hist_PID = new TH1F("M2T3S_PID", "M2T3S_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T4S_Hist_PID = new TH1F("M2T4S_PID", "M2T4S_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T6S_Hist_PID = new TH1F("M2T6S_PID", "M2T6S_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T7S_Hist_PID = new TH1F("M2T7S_PID", "M2T7S_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T8S_Hist_PID = new TH1F("M2T8S_PID", "M2T8S_PID;IntADC;evt", 1000, -10000, 50000);
    TH1F* M2T9S_Hist_PID = new TH1F("M2T9S_PID", "M2T9S_PID;IntADC;evt", 1000, -10000, 50000);

    // Load data using TChain
    TChain* evtChain = getNtupleChain(runNum);
    TBevt<TBwaveform>* anEvt = new TBevt<TBwaveform>(); 
    evtChain->SetBranchAddress("TBevt", &anEvt);

    // Check total # of events
    int totalEntry = evtChain->GetEntries();
    std::cout << "Total entries : " << totalEntry << std::endl;
    if ( (maxEntry > 0) && (maxEntry < totalEntry) ) {
        totalEntry = maxEntry;
        std::cout << "Will process maximum " << std::to_string(totalEntry) << " events" << std::endl;
    }

    // Get DWC offset for DWC PID
    TH2D* dwc1_pos   = (TH2D*) pidFile->Get("dwc1_pos");
    TH2D* dwc2_pos   = (TH2D*) pidFile->Get("dwc2_pos");
    std::vector<float> dwc1_offset = getDWCoffset(dwc1_pos);
    std::vector<float> dwc2_offset = getDWCoffset(dwc2_pos);

    // Get PS, MC PID cut
    TF1* psFitFunc = (TF1*) pidFile->Get("psFitFunc");
    float psPIDcut = psFitFunc->GetParameter(1) * 2.5f; // This is not the correct way to set cut!! >> Will be updated...
    // TF1* muFitFunc = (TF1*) pidFile->Get("muFitFunc");
    // float muPIDmean = muFitFunc->GetParameter(1);
    // float muPIDsig = muFitFunc->GetParameter(2);
    // float muPIDcut = muPIDmean + muPIDsig;

    // Evt Loop
    for (int iEvt = 0; iEvt < totalEntry; iEvt++) {
        if(iEvt % 1000 == 0) printProgress(iEvt + 1, totalEntry);

        evtChain->GetEntry(iEvt);

        // Aux data & waveform
        TBwaveform psData = anEvt->data(pscid);
        TBwaveform muData = anEvt->data(mucid);
        std::vector<short> psWaveform = psData.waveform();
        std::vector<short> muWaveform = muData.waveform();
        // DWC data & waveform
        std::vector<TBwaveform> dwc1_data;
        for (TBcid cid : dwc1_cid) {
            dwc1_data.push_back(anEvt->data(cid));
        }
        std::vector<TBwaveform> dwc2_data;
        for (TBcid cid : dwc2_cid) {
            dwc2_data.push_back(anEvt->data(cid));
        }
        std::vector< std::vector<short> > dwc1_waveform;
        for (TBwaveform data : dwc1_data) {
            dwc1_waveform.push_back(data.waveform());
        }
        std::vector< std::vector<short> > dwc2_waveform;
        for (TBwaveform data : dwc2_data) {
            dwc2_waveform.push_back(data.waveform());
        }
        std::vector<float> dwc1_peakTime;
        for (std::vector<short> waveform : dwc1_waveform) {
            dwc1_peakTime.push_back(getPeakTime(waveform));
        }
        std::vector<float> dwc2_peakTime;
        for (std::vector<short> waveform : dwc2_waveform) {
            dwc2_peakTime.push_back(getPeakTime(waveform));
        }
        std::vector<float> dwc1_correctedPosition = getDWC1position(dwc1_peakTime, dwc1_offset.at(0), dwc1_offset.at(1));
        std::vector<float> dwc2_correctedPosition = getDWC2position(dwc2_peakTime, dwc2_offset.at(0), dwc2_offset.at(1));
        // M1 data & waveform
        std::vector<TBwaveform> M1TC_data;
        for (TBcid cid : M1TC_cid) {
            M1TC_data.push_back(anEvt->data(cid));
        }
        std::vector<TBwaveform> M1TS_data;
        for (TBcid cid : M1TS_cid) {
            M1TS_data.push_back(anEvt->data(cid));
        }
        std::vector< std::vector<short> > M1TC_waveform;
        for (TBwaveform data : M1TC_data) {
            M1TC_waveform.push_back(data.waveform());
        }
        std::vector< std::vector<short> > M1TS_waveform;
        for (TBwaveform data : M1TS_data) {
            M1TS_waveform.push_back(data.waveform());
        }
        // M2 data & waveform
        std::vector<TBwaveform> M2TC_data;
        for (TBcid cid : M2TC_cid) {
            M2TC_data.push_back(anEvt->data(cid));
        }
        std::vector<TBwaveform> M2TS_data;
        for (TBcid cid : M2TS_cid) {
            M2TS_data.push_back(anEvt->data(cid));
        }
        std::vector< std::vector<short> > M2TC_waveform;
        for (TBwaveform data : M2TC_data) {
            M2TC_waveform.push_back(data.waveform());
        }
        std::vector< std::vector<short> > M2TS_waveform;
        for (TBwaveform data : M2TS_data) {
            M2TS_waveform.push_back(data.waveform());
        }

        // PS Int. ADC
        float psIntADC = 0.f;
        for (int bin = 220; bin < 390; bin++) {
            int waveformBin = bin + 1;
            psIntADC += psPed - psWaveform[waveformBin];
        }
        // MC Int. ADC
        float muIntADC = 0.f;
        for (int bin = 840; bin < 960; bin++) {
            int waveformBin = bin + 1;
            muIntADC += muPed - muWaveform[waveformBin];
        }
        // M1 C Int. ADC
        float M1T1C_IntADC = 0.f;
        float M1T2C_IntADC = 0.f;
        float M1T3C_IntADC = 0.f;
        float M1T4C_IntADC = 0.f;
        for (int bin = 120; bin < 220; bin++) {
            int waveformBin = bin + 1;
            M1T1C_IntADC += M1TC_ped.at(0) - M1TC_waveform.at(0)[waveformBin];
            M1T2C_IntADC += M1TC_ped.at(1) - M1TC_waveform.at(1)[waveformBin];
            M1T3C_IntADC += M1TC_ped.at(2) - M1TC_waveform.at(2)[waveformBin];
            M1T4C_IntADC += M1TC_ped.at(3) - M1TC_waveform.at(3)[waveformBin];
        }
        // M1 S Int. ADC
        float M1T1S_IntADC = 0.f;
        float M1T2S_IntADC = 0.f;
        float M1T3S_IntADC = 0.f;
        float M1T4S_IntADC = 0.f;
        for (int bin = 120; bin < 230; bin++) {
            int waveformBin = bin + 1;
            M1T1S_IntADC += M1TS_ped.at(0) - M1TS_waveform.at(0)[waveformBin];
            M1T2S_IntADC += M1TS_ped.at(1) - M1TS_waveform.at(1)[waveformBin];
            M1T3S_IntADC += M1TS_ped.at(2) - M1TS_waveform.at(2)[waveformBin];
            M1T4S_IntADC += M1TS_ped.at(3) - M1TS_waveform.at(3)[waveformBin];
        }
        // M2 C Int. ADC
        float M2T1C_IntADC = 0.f;
        float M2T2C_IntADC = 0.f;
        float M2T3C_IntADC = 0.f;
        float M2T4C_IntADC = 0.f;
        float M2T6C_IntADC = 0.f;
        float M2T7C_IntADC = 0.f;
        float M2T8C_IntADC = 0.f;
        float M2T9C_IntADC = 0.f;
        for (int bin = 120; bin < 220; bin++) {
            int waveformBin = bin + 1;
            M2T1C_IntADC += M2TC_ped.at(0) - M2TC_waveform.at(0)[waveformBin];
            M2T2C_IntADC += M2TC_ped.at(1) - M2TC_waveform.at(1)[waveformBin];
            M2T3C_IntADC += M2TC_ped.at(2) - M2TC_waveform.at(2)[waveformBin];
            M2T4C_IntADC += M2TC_ped.at(3) - M2TC_waveform.at(3)[waveformBin];
            M2T6C_IntADC += M2TC_ped.at(4) - M2TC_waveform.at(4)[waveformBin];
            M2T7C_IntADC += M2TC_ped.at(5) - M2TC_waveform.at(5)[waveformBin];
            M2T8C_IntADC += M2TC_ped.at(6) - M2TC_waveform.at(6)[waveformBin];
            M2T9C_IntADC += M2TC_ped.at(7) - M2TC_waveform.at(7)[waveformBin];
        }
        // M2 S Int. ADC
        // M2 C Int. ADC
        float M2T1S_IntADC = 0.f;
        float M2T2S_IntADC = 0.f;
        float M2T3S_IntADC = 0.f;
        float M2T4S_IntADC = 0.f;
        float M2T6S_IntADC = 0.f;
        float M2T7S_IntADC = 0.f;
        float M2T8S_IntADC = 0.f;
        float M2T9S_IntADC = 0.f;
        for (int bin = 120; bin < 230; bin++) {
            int waveformBin = bin + 1;
            M2T1S_IntADC += M2TS_ped.at(0) - M2TS_waveform.at(0)[waveformBin];
            M2T2S_IntADC += M2TS_ped.at(1) - M2TS_waveform.at(1)[waveformBin];
            M2T3S_IntADC += M2TS_ped.at(2) - M2TS_waveform.at(2)[waveformBin];
            M2T4S_IntADC += M2TS_ped.at(3) - M2TS_waveform.at(3)[waveformBin];
            M2T6S_IntADC += M2TS_ped.at(4) - M2TS_waveform.at(4)[waveformBin];
            M2T7S_IntADC += M2TS_ped.at(5) - M2TS_waveform.at(5)[waveformBin];
            M2T8S_IntADC += M2TS_ped.at(6) - M2TS_waveform.at(6)[waveformBin];
            M2T9S_IntADC += M2TS_ped.at(7) - M2TS_waveform.at(7)[waveformBin];
        }
        // PS, MC Int. ADC plot before PID
        psIntHist->Fill(psIntADC);
        muIntHist->Fill(muIntADC);

        M1T1C_Hist->Fill(M1T1C_IntADC);
        M1T2C_Hist->Fill(M1T2C_IntADC);
        M1T3C_Hist->Fill(M1T3C_IntADC);
        M1T4C_Hist->Fill(M1T4C_IntADC);

        M1T1S_Hist->Fill(M1T1S_IntADC);
        M1T2S_Hist->Fill(M1T2S_IntADC);
        M1T3S_Hist->Fill(M1T3S_IntADC);
        M1T4S_Hist->Fill(M1T4S_IntADC);

        M2T1C_Hist->Fill(M2T1C_IntADC);
        M2T2C_Hist->Fill(M2T2C_IntADC);
        M2T3C_Hist->Fill(M2T3C_IntADC);
        M2T4C_Hist->Fill(M2T4C_IntADC);
        M2T6C_Hist->Fill(M2T6C_IntADC);
        M2T7C_Hist->Fill(M2T7C_IntADC);
        M2T8C_Hist->Fill(M2T8C_IntADC);
        M2T9C_Hist->Fill(M2T9C_IntADC);

        M2T1S_Hist->Fill(M2T1S_IntADC);
        M2T2S_Hist->Fill(M2T2S_IntADC);
        M2T3S_Hist->Fill(M2T3S_IntADC);
        M2T4S_Hist->Fill(M2T4S_IntADC);
        M2T6S_Hist->Fill(M2T6S_IntADC);
        M2T7S_Hist->Fill(M2T7S_IntADC);
        M2T8S_Hist->Fill(M2T8S_IntADC);
        M2T9S_Hist->Fill(M2T9S_IntADC);
        // For Aux. PID
        if (! dwcCorrelationPID(dwc1_correctedPosition, dwc2_correctedPosition, 1.5f) ) continue;
        // if (muIntADC < (muPIDCut + muPIDsig)) continue;
        if (psIntADC < psPIDcut) continue;

        // PS, MC Int. ADC plot after PID
        psIntHist_PID->Fill(psIntADC);
        muIntHist_PID->Fill(muIntADC);

        M1T1C_Hist_PID->Fill(M1T1C_IntADC);
        M1T2C_Hist_PID->Fill(M1T2C_IntADC);
        M1T3C_Hist_PID->Fill(M1T3C_IntADC);
        M1T4C_Hist_PID->Fill(M1T4C_IntADC);

        M1T1S_Hist_PID->Fill(M1T1S_IntADC);
        M1T2S_Hist_PID->Fill(M1T2S_IntADC);
        M1T3S_Hist_PID->Fill(M1T3S_IntADC);
        M1T4S_Hist_PID->Fill(M1T4S_IntADC);

        M2T1C_Hist_PID->Fill(M2T1C_IntADC);
        M2T2C_Hist_PID->Fill(M2T2C_IntADC);
        M2T3C_Hist_PID->Fill(M2T3C_IntADC);
        M2T4C_Hist_PID->Fill(M2T4C_IntADC);
        M2T6C_Hist_PID->Fill(M2T6C_IntADC);
        M2T7C_Hist_PID->Fill(M2T7C_IntADC);
        M2T8C_Hist_PID->Fill(M2T8C_IntADC);
        M2T9C_Hist_PID->Fill(M2T9C_IntADC);

        M2T1S_Hist_PID->Fill(M2T1S_IntADC);
        M2T2S_Hist_PID->Fill(M2T2S_IntADC);
        M2T3S_Hist_PID->Fill(M2T3S_IntADC);
        M2T4S_Hist_PID->Fill(M2T4S_IntADC);
        M2T6S_Hist_PID->Fill(M2T6S_IntADC);
        M2T7S_Hist_PID->Fill(M2T7S_IntADC);
        M2T8S_Hist_PID->Fill(M2T8S_IntADC);
        M2T9S_Hist_PID->Fill(M2T9S_IntADC);
    }

    TCanvas* c = new TCanvas();

    std::string outFile = "/u/user/swkim/pre-exercise/dual-readout_TB/analysis/intADC/intADC_Run_" + std::to_string(runNum) + ".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");
    outputRoot->cd();

    psIntHist->Write();
    psIntHist->Draw();
    c->Update();
    float psHist_min = gPad->GetUymin();
    float psHist_max = gPad->GetUymax();
    psIntHist_PID->GetYaxis()->SetRangeUser(psHist_min, psHist_max);
    psIntHist_PID->Write();

    muIntHist->Write();
    muIntHist->Draw();
    c->Update();
    float muHist_min = gPad->GetUymin();
    float muHist_max = gPad->GetUymax();
    muIntHist_PID->GetYaxis()->SetRangeUser(muHist_min, muHist_max);
    muIntHist_PID->Write();

    M1T1C_Hist->Write();
    M1T2C_Hist->Write();
    M1T3C_Hist->Write();
    M1T4C_Hist->Write();

    M1T1S_Hist->Write();
    M1T2S_Hist->Write();
    M1T3S_Hist->Write();
    M1T4S_Hist->Write();

    M1T1C_Hist->Draw();
    c->Update();
    float M1T1C_min = gPad->GetUymin();
    float M1T1C_max = gPad->GetUymax();
    M1T1C_Hist_PID->GetYaxis()->SetRangeUser(M1T1C_min, M1T1C_max);
    M1T1C_Hist_PID->Write();

    M1T2C_Hist->Draw();
    c->Update();
    float M1T2C_min = gPad->GetUymin();
    float M1T2C_max = gPad->GetUymax();
    M1T2C_Hist_PID->GetYaxis()->SetRangeUser(M1T2C_min, M1T2C_max);
    M1T2C_Hist_PID->Write();

    M1T3C_Hist->Draw();
    c->Update();
    float M1T3C_min = gPad->GetUymin();
    float M1T3C_max = gPad->GetUymax();
    M1T3C_Hist_PID->GetYaxis()->SetRangeUser(M1T3C_min, M1T3C_max);
    M1T3C_Hist_PID->Write();

    M1T4C_Hist->Draw();
    c->Update();
    float M1T4C_min = gPad->GetUymin();
    float M1T4C_max = gPad->GetUymax();
    M1T4C_Hist_PID->GetYaxis()->SetRangeUser(M1T4C_min, M1T4C_max);
    M1T4C_Hist_PID->Write();

    M1T1S_Hist->Draw();
    c->Update();
    float M1T1S_min = gPad->GetUymin();
    float M1T1S_max = gPad->GetUymax();
    M1T1S_Hist_PID->GetYaxis()->SetRangeUser(M1T1S_min, M1T1S_max);
    M1T1S_Hist_PID->Write();

    M1T2S_Hist->Draw();
    c->Update();
    float M1T2S_min = gPad->GetUymin();
    float M1T2S_max = gPad->GetUymax();
    M1T2S_Hist_PID->GetYaxis()->SetRangeUser(M1T2S_min, M1T2S_max);
    M1T2S_Hist_PID->Write();

    M1T3S_Hist->Draw();
    c->Update();
    float M1T3S_min = gPad->GetUymin();
    float M1T3S_max = gPad->GetUymax();
    M1T3S_Hist_PID->GetYaxis()->SetRangeUser(M1T3S_min, M1T3S_max);
    M1T3S_Hist_PID->Write();

    M1T4S_Hist->Draw();
    c->Update();
    float M1T4S_min = gPad->GetUymin();
    float M1T4S_max = gPad->GetUymax();
    M1T4S_Hist_PID->GetYaxis()->SetRangeUser(M1T4S_min, M1T4S_max);
    M1T4S_Hist_PID->Write();
    // M2 C
    M2T1C_Hist->Write();
    M2T2C_Hist->Write();
    M2T3C_Hist->Write();
    M2T4C_Hist->Write();
    M2T6C_Hist->Write();
    M2T7C_Hist->Write();
    M2T8C_Hist->Write();
    M2T9C_Hist->Write();
    // M2 S
    M2T1S_Hist->Write();
    M2T2S_Hist->Write();
    M2T3S_Hist->Write();
    M2T4S_Hist->Write();
    M2T6S_Hist->Write();
    M2T7S_Hist->Write();
    M2T8S_Hist->Write();
    M2T9S_Hist->Write();
    // M2 C PID
    M2T1C_Hist->Draw();
    c->Update();
    float M2T1C_min = gPad->GetUymin();
    float M2T1C_max = gPad->GetUymax();
    M2T1C_Hist_PID->GetYaxis()->SetRangeUser(M2T1C_min, M2T1C_max);
    M2T1C_Hist_PID->Write();

    M2T2C_Hist->Draw();
    c->Update();
    float M2T2C_min = gPad->GetUymin();
    float M2T2C_max = gPad->GetUymax();
    M2T2C_Hist_PID->GetYaxis()->SetRangeUser(M2T2C_min, M2T2C_max);
    M2T2C_Hist_PID->Write();

    M2T3C_Hist->Draw();
    c->Update();
    float M2T3C_min = gPad->GetUymin();
    float M2T3C_max = gPad->GetUymax();
    M2T3C_Hist_PID->GetYaxis()->SetRangeUser(M2T3C_min, M2T3C_max);
    M2T3C_Hist_PID->Write();

    M2T4C_Hist->Draw();
    c->Update();
    float M2T4C_min = gPad->GetUymin();
    float M2T4C_max = gPad->GetUymax();
    M2T4C_Hist_PID->GetYaxis()->SetRangeUser(M2T4C_min, M2T4C_max);
    M2T4C_Hist_PID->Write();

    M2T6C_Hist->Draw();
    c->Update();
    float M2T6C_min = gPad->GetUymin();
    float M2T6C_max = gPad->GetUymax();
    M2T6C_Hist_PID->GetYaxis()->SetRangeUser(M2T6C_min, M2T6C_max);
    M2T6C_Hist_PID->Write();

    M2T7C_Hist->Draw();
    c->Update();
    float M2T7C_min = gPad->GetUymin();
    float M2T7C_max = gPad->GetUymax();
    M2T7C_Hist_PID->GetYaxis()->SetRangeUser(M2T7C_min, M2T7C_max);
    M2T7C_Hist_PID->Write();

    M2T8C_Hist->Draw();
    c->Update();
    float M2T8C_min = gPad->GetUymin();
    float M2T8C_max = gPad->GetUymax();
    M2T8C_Hist_PID->GetYaxis()->SetRangeUser(M2T8C_min, M2T8C_max);
    M2T8C_Hist_PID->Write();

    M2T9C_Hist->Draw();
    c->Update();
    float M2T9C_min = gPad->GetUymin();
    float M2T9C_max = gPad->GetUymax();
    M2T9C_Hist_PID->GetYaxis()->SetRangeUser(M2T9C_min, M2T9C_max);
    M2T9C_Hist_PID->Write();

    // M2 S PID
    M2T1S_Hist->Draw();
    c->Update();
    float M2T1S_min = gPad->GetUymin();
    float M2T1S_max = gPad->GetUymax();
    M2T1S_Hist_PID->GetYaxis()->SetRangeUser(M2T1S_min, M2T1S_max);
    M2T1S_Hist_PID->Write();

    M2T2S_Hist->Draw();
    c->Update();
    float M2T2S_min = gPad->GetUymin();
    float M2T2S_max = gPad->GetUymax();
    M2T2S_Hist_PID->GetYaxis()->SetRangeUser(M2T2S_min, M2T2S_max);
    M2T2S_Hist_PID->Write();

    M2T3S_Hist->Draw();
    c->Update();
    float M2T3S_min = gPad->GetUymin();
    float M2T3S_max = gPad->GetUymax();
    M2T3S_Hist_PID->GetYaxis()->SetRangeUser(M2T3S_min, M2T3S_max);
    M2T3S_Hist_PID->Write();

    M2T4S_Hist->Draw();
    c->Update();
    float M2T4S_min = gPad->GetUymin();
    float M2T4S_max = gPad->GetUymax();
    M2T4S_Hist_PID->GetYaxis()->SetRangeUser(M2T4S_min, M2T4S_max);
    M2T4S_Hist_PID->Write();

    M2T6S_Hist->Draw();
    c->Update();
    float M2T6S_min = gPad->GetUymin();
    float M2T6S_max = gPad->GetUymax();
    M2T6S_Hist_PID->GetYaxis()->SetRangeUser(M2T6S_min, M2T6S_max);
    M2T6S_Hist_PID->Write();

    M2T7S_Hist->Draw();
    c->Update();
    float M2T7S_min = gPad->GetUymin();
    float M2T7S_max = gPad->GetUymax();
    M2T7S_Hist_PID->GetYaxis()->SetRangeUser(M2T7S_min, M2T7S_max);
    M2T7S_Hist_PID->Write();

    M2T8S_Hist->Draw();
    c->Update();
    float M2T8S_min = gPad->GetUymin();
    float M2T8S_max = gPad->GetUymax();
    M2T8S_Hist_PID->GetYaxis()->SetRangeUser(M2T8S_min, M2T8S_max);
    M2T8S_Hist_PID->Write();

    M2T9S_Hist->Draw();
    c->Update();
    float M2T9S_min = gPad->GetUymin();
    float M2T9S_max = gPad->GetUymax();
    M2T9S_Hist_PID->GetYaxis()->SetRangeUser(M2T9S_min, M2T9S_max);
    M2T9S_Hist_PID->Write();

    outputRoot->Close();
}
