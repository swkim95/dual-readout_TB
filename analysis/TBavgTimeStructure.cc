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
    // Get DWC info
    TFile* dwcFile = TFile::Open(("./dwc/dwc_Run_" + std::to_string(runNum) + ".root").c_str());
    // Get channel IDs
    TBcid pscid = utility.getcid(TBdetector::detid::preshower);
    TBcid mucid = utility.getcid(TBdetector::detid::muon);
    TBcid tccid = utility.getcid(TBdetector::detid::tail);

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
    float tcPed = utility.retrievePed(tccid);

    std::vector<float> dwc1_ped;
    for(TBcid cid : dwc1_cid) {
        dwc1_ped.push_back(utility.retrievePed(cid));
    }
    std::vector<float> dwc2_ped;
    for(TBcid cid : dwc2_cid) {
        dwc2_ped.push_back(utility.retrievePed(cid));
    }
    
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
    TH1F* psHist = new TH1F("psHist", "psHist;bin;ADC", 1000, 0, 1000);
    TH1F* psPedHist = new TH1F("psPedHist", "psPedHist;bin;Ped-ADC", 1000, 0, 1000);
    TH2F* ps2DHist = new TH2F("ps2DHist", "ps2DHist", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* ps2DPedHist = new TH2F("ps2DPedHist", "ps2DPedHist", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* muHist = new TH1F("muHist", "muHist", 1000, 0, 1000);
    TH1F* muPedHist = new TH1F("muPedHist", "muPedHist", 1000, 0, 1000);
    TH2F* mu2DHist = new TH2F("mu2DHist", "mu2DHist", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* mu2DPedHist = new TH2F("mu2DPedHist", "mu2DPedHist", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* tcHist = new TH1F("tcHist", "tcHist", 1000, 0, 1000);
    TH1F* tcPedHist = new TH1F("tcPedHist", "tcPedHist", 1000, 0, 1000);
    TH2F* tc2DHist = new TH2F("tc2DHist", "tc2DHist", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* tc2DPedHist = new TH2F("tc2DPedHist", "tc2DPedHist", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* dwc1RHist = new TH1F("dwc1RHist", "dwc1RHist", 1000, 0, 1000);
    TH1F* dwc1RPedHist = new TH1F("dwc1RPedHist", "dwc1RPedHist", 1000, 0, 1000);

    TH1F* dwc1LHist = new TH1F("dwc1LHist", "dwc1LHist", 1000, 0, 1000);
    TH1F* dwc1LPedHist = new TH1F("dwc1LPedHist", "dwc1LPedHist", 1000, 0, 1000);

    TH1F* dwc1UHist = new TH1F("dwc1UHist", "dwc1UHist", 1000, 0, 1000);
    TH1F* dwc1UPedHist = new TH1F("dwc1UPedHist", "dwc1UPedHist", 1000, 0, 1000);

    TH1F* dwc1DHist = new TH1F("dwc1DHist", "dwc1DHist", 1000, 0, 1000);
    TH1F* dwc1DPedHist = new TH1F("dwc1DPedHist", "dwc1DPedHist", 1000, 0, 1000);

    TH1F* dwc2RHist = new TH1F("dwc2RHist", "dwc2RHist", 1000, 0, 1000);
    TH1F* dwc2RPedHist = new TH1F("dwc2RPedHist", "dwc2RPedHist", 1000, 0, 1000);

    TH1F* dwc2LHist = new TH1F("dwc2LHist", "dwc2LHist", 1000, 0, 1000);
    TH1F* dwc2LPedHist = new TH1F("dwc2LPedHist", "dwc2LPedHist", 1000, 0, 1000);

    TH1F* dwc2UHist = new TH1F("dwc2UHist", "dwc2UHist", 1000, 0, 1000);
    TH1F* dwc2UPedHist = new TH1F("dwc2UPedHist", "dwc2UPedHist", 1000, 0, 1000);

    TH1F* dwc2DHist = new TH1F("dwc2DHist", "dwc2DHist", 1000, 0, 1000);
    TH1F* dwc2DPedHist = new TH1F("dwc2DPedHist", "dwc2DPedHist", 1000, 0, 1000);
    // M1 C
    TH1F* M1T1CHist = new TH1F("M1T1CHist", "M1T1CHist;bin;ADC", 1000, 0, 1000);
    TH1F* M1T1CPedHist = new TH1F("M1T1CPedHist", "M1T1CPedHist;bin;ADC", 1000, 0, 1000);
    TH2F* M1T1C2DHist = new TH2F("M1T1C2DHist", "M1T1C2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M1T1C2DPedHist = new TH2F("M1T1C2DPedHist", "M1T1C2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);
    
    TH1F* M1T2CHist = new TH1F("M1T2CHist", "M1T2CHist", 1000, 0, 1000);
    TH1F* M1T2CPedHist = new TH1F("M1T2CPedHist", "M1T2CPedHist", 1000, 0, 1000);
    TH2F* M1T2C2DHist = new TH2F("M1T2C2DHist", "M1T2C2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M1T2C2DPedHist = new TH2F("M1T2C2DPedHist", "M1T2C2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* M1T3CHist = new TH1F("M1T3CHist", "M1T3CHist", 1000, 0, 1000);
    TH1F* M1T3CPedHist = new TH1F("M1T3CPedHist", "M1T3CPedHist", 1000, 0, 1000);
    TH2F* M1T3C2DHist = new TH2F("M1T3C2DHist", "M1T3C2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M1T3C2DPedHist = new TH2F("M1T3C2DPedHist", "M1T3C2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* M1T4CHist = new TH1F("M1T4CHist", "M1T4CHist", 1000, 0, 1000);
    TH1F* M1T4CPedHist = new TH1F("M1T4CPedHist", "M1T4CPedHist", 1000, 0, 1000);
    TH2F* M1T4C2DHist = new TH2F("M1T4C2DHist", "M1T4C2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M1T4C2DPedHist = new TH2F("M1T4C2DPedHist", "M1T4C2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);
    // M1 S
    TH1F* M1T1SHist = new TH1F("M1T1SHist", "M1T1SHist", 1000, 0, 1000);
    TH1F* M1T1SPedHist = new TH1F("M1T1SPedHist", "M1T1SPedHist", 1000, 0, 1000);
    TH2F* M1T1S2DHist = new TH2F("M1T1S2DHist", "M1T1S2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M1T1S2DPedHist = new TH2F("M1T1S2DPedHist", "M1T1S2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* M1T2SHist = new TH1F("M1T2SHist", "M1T2SHist", 1000, 0, 1000);
    TH1F* M1T2SPedHist = new TH1F("M1T2SPedHist", "M1T2SPedHist", 1000, 0, 1000);
    TH2F* M1T2S2DHist = new TH2F("M1T2S2DHist", "M1T2S2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M1T2S2DPedHist = new TH2F("M1T2S2DPedHist", "M1T2S2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* M1T3SHist = new TH1F("M1T3SHist", "M1T3SHist", 1000, 0, 1000);
    TH1F* M1T3SPedHist = new TH1F("M1T3SPedHist", "M1T3SPedHist", 1000, 0, 1000);
    TH2F* M1T3S2DHist = new TH2F("M1T3S2DHist", "M1T3S2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M1T3S2DPedHist = new TH2F("M1T3S2DPedHist", "M1T3S2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* M1T4SHist = new TH1F("M1T4SHist", "M1T4SHist", 1000, 0, 1000);
    TH1F* M1T4SPedHist = new TH1F("M1T4SPedHist", "M1T4SPedHist", 1000, 0, 1000);
    TH2F* M1T4S2DHist = new TH2F("M1T4S2DHist", "M1T4S2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M1T4S2DPedHist = new TH2F("M1T4S2DPedHist", "M1T4S2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);
    // M2 C
    TH1F* M2T1CHist = new TH1F("M2T1CHist", "M2T1CHist;bin;ADC", 1000, 0, 1000);
    TH1F* M2T1CPedHist = new TH1F("M2T1CPedHist", "M2T1CPedHist;bin;ADC", 1000, 0, 1000);
    TH2F* M2T1C2DHist = new TH2F("M2T1C2DHist", "M2T1C2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M2T1C2DPedHist = new TH2F("M2T1C2DPedHist", "M2T1C2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);
    
    TH1F* M2T2CHist = new TH1F("M2T2CHist", "M2T2CHist", 1000, 0, 1000);
    TH1F* M2T2CPedHist = new TH1F("M2T2CPedHist", "M2T2CPedHist", 1000, 0, 1000);
    TH2F* M2T2C2DHist = new TH2F("M2T2C2DHist", "M2T2C2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M2T2C2DPedHist = new TH2F("M2T2C2DPedHist", "M2T2C2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* M2T3CHist = new TH1F("M2T3CHist", "M2T3CHist", 1000, 0, 1000);
    TH1F* M2T3CPedHist = new TH1F("M2T3CPedHist", "M2T3CPedHist", 1000, 0, 1000);
    TH2F* M2T3C2DHist = new TH2F("M2T3C2DHist", "M2T3C2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M2T3C2DPedHist = new TH2F("M2T3C2DPedHist", "M2T3C2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* M2T4CHist = new TH1F("M2T4CHist", "M2T4CHist", 1000, 0, 1000);
    TH1F* M2T4CPedHist = new TH1F("M2T4CPedHist", "M2T4CPedHist", 1000, 0, 1000);
    TH2F* M2T4C2DHist = new TH2F("M2T4C2DHist", "M2T4C2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M2T4C2DPedHist = new TH2F("M2T4C2DPedHist", "M2T4C2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* M2T6CHist = new TH1F("M2T6CHist", "M2T6CHist;bin;ADC", 1000, 0, 1000);
    TH1F* M2T6CPedHist = new TH1F("M2T6CPedHist", "M2T6CPedHist;bin;ADC", 1000, 0, 1000);
    TH2F* M2T6C2DHist = new TH2F("M2T6C2DHist", "M2T6C2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M2T6C2DPedHist = new TH2F("M2T6C2DPedHist", "M2T6C2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);
    
    TH1F* M2T7CHist = new TH1F("M2T7CHist", "M2T7CHist", 1000, 0, 1000);
    TH1F* M2T7CPedHist = new TH1F("M2T7CPedHist", "M2T7CPedHist", 1000, 0, 1000);
    TH2F* M2T7C2DHist = new TH2F("M2T7C2DHist", "M2T7C2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M2T7C2DPedHist = new TH2F("M2T7C2DPedHist", "M2T7C2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* M2T8CHist = new TH1F("M2T8CHist", "M2T8CHist", 1000, 0, 1000);
    TH1F* M2T8CPedHist = new TH1F("M2T8CPedHist", "M2T8CPedHist", 1000, 0, 1000);
    TH2F* M2T8C2DHist = new TH2F("M2T8C2DHist", "M2T8C2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M2T8C2DPedHist = new TH2F("M2T8C2DPedHist", "M2T8C2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* M2T9CHist = new TH1F("M2T9CHist", "M2T9CHist", 1000, 0, 1000);
    TH1F* M2T9CPedHist = new TH1F("M2T9CPedHist", "M2T9CPedHist", 1000, 0, 1000);
    TH2F* M2T9C2DHist = new TH2F("M2T9C2DHist", "M2T9C2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M2T9C2DPedHist = new TH2F("M2T9C2DPedHist", "M2T9C2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);
    // M2 S
    TH1F* M2T1SHist = new TH1F("M2T1SHist", "M2T1SHist;bin;ADC", 1000, 0, 1000);
    TH1F* M2T1SPedHist = new TH1F("M2T1SPedHist", "M2T1SPedHist;bin;ADC", 1000, 0, 1000);
    TH2F* M2T1S2DHist = new TH2F("M2T1S2DHist", "M2T1S2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M2T1S2DPedHist = new TH2F("M2T1S2DPedHist", "M2T1S2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);
    
    TH1F* M2T2SHist = new TH1F("M2T2SHist", "M2T2SHist", 1000, 0, 1000);
    TH1F* M2T2SPedHist = new TH1F("M2T2SPedHist", "M2T2SPedHist", 1000, 0, 1000);
    TH2F* M2T2S2DHist = new TH2F("M2T2S2DHist", "M2T2S2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M2T2S2DPedHist = new TH2F("M2T2S2DPedHist", "M2T2S2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* M2T3SHist = new TH1F("M2T3SHist", "M2T3SHist", 1000, 0, 1000);
    TH1F* M2T3SPedHist = new TH1F("M2T3SPedHist", "M2T3SPedHist", 1000, 0, 1000);
    TH2F* M2T3S2DHist = new TH2F("M2T3S2DHist", "M2T3S2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M2T3S2DPedHist = new TH2F("M2T3S2DPedHist", "M2T3S2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* M2T4SHist = new TH1F("M2T4SHist", "M2T4SHist", 1000, 0, 1000);
    TH1F* M2T4SPedHist = new TH1F("M2T4SPedHist", "M2T4SPedHist", 1000, 0, 1000);
    TH2F* M2T4S2DHist = new TH2F("M2T4S2DHist", "M2T4S2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M2T4S2DPedHist = new TH2F("M2T4S2DPedHist", "M2T4S2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* M2T6SHist = new TH1F("M2T6SHist", "M2T6SHist;bin;ADC", 1000, 0, 1000);
    TH1F* M2T6SPedHist = new TH1F("M2T6SPedHist", "M2T6SPedHist;bin;ADC", 1000, 0, 1000);
    TH2F* M2T6S2DHist = new TH2F("M2T6S2DHist", "M2T6S2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M2T6S2DPedHist = new TH2F("M2T6S2DPedHist", "M2T6S2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);
    
    TH1F* M2T7SHist = new TH1F("M2T7SHist", "M2T7SHist", 1000, 0, 1000);
    TH1F* M2T7SPedHist = new TH1F("M2T7SPedHist", "M2T7SPedHist", 1000, 0, 1000);
    TH2F* M2T7S2DHist = new TH2F("M2T7S2DHist", "M2T7S2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M2T7S2DPedHist = new TH2F("M2T7S2DPedHist", "M2T7S2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* M2T8SHist = new TH1F("M2T8SHist", "M2T8SHist", 1000, 0, 1000);
    TH1F* M2T8SPedHist = new TH1F("M2T8SPedHist", "M2T8SPedHist", 1000, 0, 1000);
    TH2F* M2T8S2DHist = new TH2F("M2T8S2DHist", "M2T8S2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M2T8S2DPedHist = new TH2F("M2T8S2DPedHist", "M2T8S2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* M2T9SHist = new TH1F("M2T9SHist", "M2T9SHist", 1000, 0, 1000);
    TH1F* M2T9SPedHist = new TH1F("M2T9SPedHist", "M2T9SPedHist", 1000, 0, 1000);
    TH2F* M2T9S2DHist = new TH2F("M2T9S2DHist", "M2T9S2DHist;bin;ADC", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* M2T9S2DPedHist = new TH2F("M2T9S2DPedHist", "M2T9S2DPedHist;bin;ADC", 1000, 0, 1000, 5096, -1000, 4096);

    TH2D* dwc1_correctedPos = new TH2D("dwc1_correctedPos", "dwc1_correctedPos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* dwc2_correctedPos = new TH2D("dwc2_correctedPos", "dwc2_correctedPos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* dwc_corrected_x_corr = new TH2D("dwc_corrected_x_corr", "dwc_corrected_x_corr;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* dwc_corrected_y_corr = new TH2D("dwc_corrected_y_corr", "dwc_corrected_y_corr;mm;mm;events", 480, -120., 120., 480, -120., 120.);

    TH2D* dwc1_pos_PID = new TH2D("dwc1_pos_PID", "dwc1_pos_PID;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* dwc2_pos_PID = new TH2D("dwc2_pos_PID", "dwc2_pos_PID;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* dwc1_correctedPos_PID = new TH2D("dwc1_correctedPos_PID", "dwc1_correctedPos_PID;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* dwc2_correctedPos_PID = new TH2D("dwc2_correctedPos_PID", "dwc2_correctedPos_PID;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    
    TH2D* dwc_x_corr_PID = new TH2D("dwc_x_corr_PID", "dwc_x_corr_PID;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* dwc_y_corr_PID = new TH2D("dwc_y_corr_PID", "dwc_y_corr_PID;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* dwc_corrected_x_corr_PID = new TH2D("dwc_corrected_x_corr_PID", "dwc_corrected_x_corr_PID;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* dwc_corrected_y_corr_PID = new TH2D("dwc_corrected_y_corr_PID", "dwc_corrected_y_corr_PID;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    
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

    // DWC offset for DWC correlation cut
    TH2D* dwc1_pos   = (TH2D*) dwcFile->Get("dwc1_pos");
    TH2D* dwc2_pos   = (TH2D*) dwcFile->Get("dwc2_pos");
    TH2D* dwc_x_corr = (TH2D*) dwcFile->Get("dwc_x_corr");
    TH2D* dwc_y_corr = (TH2D*) dwcFile->Get("dwc_y_corr");
    std::vector<float> dwc1_offset = getDWCoffset(dwc1_pos);
    std::vector<float> dwc2_offset = getDWCoffset(dwc2_pos);

    // Evt Loop
    for (int iEvt = 0; iEvt < totalEntry; iEvt++) {
        if( iEvt % 1000 == 0 ) printProgress(iEvt + 1, totalEntry);

        evtChain->GetEntry(iEvt);
        TBwaveform psData = anEvt->data(pscid);
        TBwaveform muData = anEvt->data(mucid);
        TBwaveform tcData = anEvt->data(tccid);

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
        std::vector<short> tcWaveform = tcData.waveform();

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

        std::vector<float> dwc1_peakTime;
        for (std::vector<short> waveform : dwc1_waveform) {
            dwc1_peakTime.push_back(getPeakTime(waveform));
        }
        std::vector<float> dwc2_peakTime;
        for (std::vector<short> waveform : dwc2_waveform) {
            dwc2_peakTime.push_back(getPeakTime(waveform));
        }

        std::vector<float> dwc1_position = getDWC1position(dwc1_peakTime);
        std::vector<float> dwc2_position = getDWC2position(dwc2_peakTime);
        std::vector<float> dwc1_correctedPosition = getDWC1position(dwc1_peakTime, dwc1_offset.at(0), dwc1_offset.at(1));
        std::vector<float> dwc2_correctedPosition = getDWC2position(dwc2_peakTime, dwc2_offset.at(0), dwc2_offset.at(1));

        dwc1_correctedPos->Fill(dwc1_correctedPosition.at(0), dwc1_correctedPosition.at(1));
        dwc2_correctedPos->Fill(dwc2_correctedPosition.at(0), dwc2_correctedPosition.at(1));
        dwc_corrected_x_corr->Fill(dwc1_correctedPosition.at(0), dwc2_correctedPosition.at(0));
        dwc_corrected_y_corr->Fill(dwc1_correctedPosition.at(1), dwc2_correctedPosition.at(1));
    
        // For DWC PID
        if (! dwcCorrelationPID(dwc1_correctedPosition, dwc2_correctedPosition, 2.f) ) continue;

        dwc1_pos_PID->Fill(dwc1_position.at(0), dwc1_position.at(1));
        dwc2_pos_PID->Fill(dwc2_position.at(0), dwc2_position.at(1));
        dwc1_correctedPos_PID->Fill(dwc1_correctedPosition.at(0), dwc1_correctedPosition.at(1));
        dwc2_correctedPos_PID->Fill(dwc2_correctedPosition.at(0), dwc2_correctedPosition.at(1));

        dwc_x_corr_PID->Fill(dwc1_position.at(0), dwc2_position.at(0));
        dwc_y_corr_PID->Fill(dwc1_position.at(1), dwc2_position.at(1));
        dwc_corrected_x_corr_PID->Fill(dwc1_correctedPosition.at(0), dwc2_correctedPosition.at(0));
        dwc_corrected_y_corr_PID->Fill(dwc1_correctedPosition.at(1), dwc2_correctedPosition.at(1));


        for (int bin = 0; bin < 1000; bin++) {
            int waveformBin = bin + 1;
            float ps_PedCorrectedWave = psPed - psWaveform[waveformBin];
            float mu_PedCorrectedWave = muPed - muWaveform[waveformBin];
            float tc_PedCorrectedWave = tcPed - tcWaveform[waveformBin];

            float dwc1R_PedCorrectedWave = dwc1_ped.at(0) - dwc1_waveform.at(0)[waveformBin];
            float dwc1L_PedCorrectedWave = dwc1_ped.at(1) - dwc1_waveform.at(1)[waveformBin];
            float dwc1U_PedCorrectedWave = dwc1_ped.at(2) - dwc1_waveform.at(2)[waveformBin];
            float dwc1D_PedCorrectedWave = dwc1_ped.at(3) - dwc1_waveform.at(3)[waveformBin];

            float dwc2R_PedCorrectedWave = dwc2_ped.at(0) - dwc2_waveform.at(0)[waveformBin];
            float dwc2L_PedCorrectedWave = dwc2_ped.at(1) - dwc2_waveform.at(1)[waveformBin];
            float dwc2U_PedCorrectedWave = dwc2_ped.at(2) - dwc2_waveform.at(2)[waveformBin];
            float dwc2D_PedCorrectedWave = dwc2_ped.at(3) - dwc2_waveform.at(3)[waveformBin];

            float M1T1C_PedCorrectedWave = M1TC_ped.at(0) - M1TC_waveform.at(0)[waveformBin];
            float M1T2C_PedCorrectedWave = M1TC_ped.at(1) - M1TC_waveform.at(1)[waveformBin];
            float M1T3C_PedCorrectedWave = M1TC_ped.at(2) - M1TC_waveform.at(2)[waveformBin];
            float M1T4C_PedCorrectedWave = M1TC_ped.at(3) - M1TC_waveform.at(3)[waveformBin];

            float M1T1S_PedCorrectedWave = M1TS_ped.at(0) - M1TS_waveform.at(0)[waveformBin];
            float M1T2S_PedCorrectedWave = M1TS_ped.at(1) - M1TS_waveform.at(1)[waveformBin];
            float M1T3S_PedCorrectedWave = M1TS_ped.at(2) - M1TS_waveform.at(2)[waveformBin];
            float M1T4S_PedCorrectedWave = M1TS_ped.at(3) - M1TS_waveform.at(3)[waveformBin];

            float M2T1C_PedCorrectedWave = M2TC_ped.at(0) - M2TC_waveform.at(0)[waveformBin];
            float M2T2C_PedCorrectedWave = M2TC_ped.at(1) - M2TC_waveform.at(1)[waveformBin];
            float M2T3C_PedCorrectedWave = M2TC_ped.at(2) - M2TC_waveform.at(2)[waveformBin];
            float M2T4C_PedCorrectedWave = M2TC_ped.at(3) - M2TC_waveform.at(3)[waveformBin];
            float M2T6C_PedCorrectedWave = M2TC_ped.at(4) - M2TC_waveform.at(4)[waveformBin];
            float M2T7C_PedCorrectedWave = M2TC_ped.at(5) - M2TC_waveform.at(5)[waveformBin];
            float M2T8C_PedCorrectedWave = M2TC_ped.at(6) - M2TC_waveform.at(6)[waveformBin];
            float M2T9C_PedCorrectedWave = M2TC_ped.at(7) - M2TC_waveform.at(7)[waveformBin];

            float M2T1S_PedCorrectedWave = M2TS_ped.at(0) - M2TS_waveform.at(0)[waveformBin];
            float M2T2S_PedCorrectedWave = M2TS_ped.at(1) - M2TS_waveform.at(1)[waveformBin];
            float M2T3S_PedCorrectedWave = M2TS_ped.at(2) - M2TS_waveform.at(2)[waveformBin];
            float M2T4S_PedCorrectedWave = M2TS_ped.at(3) - M2TS_waveform.at(3)[waveformBin];
            float M2T6S_PedCorrectedWave = M2TS_ped.at(4) - M2TS_waveform.at(4)[waveformBin];
            float M2T7S_PedCorrectedWave = M2TS_ped.at(5) - M2TS_waveform.at(5)[waveformBin];
            float M2T8S_PedCorrectedWave = M2TS_ped.at(6) - M2TS_waveform.at(6)[waveformBin];
            float M2T9S_PedCorrectedWave = M2TS_ped.at(7) - M2TS_waveform.at(7)[waveformBin];

            psHist        ->Fill(bin, psWaveform[waveformBin]);
            psPedHist     ->Fill(bin, ps_PedCorrectedWave);
            ps2DHist      ->Fill(bin, psWaveform[waveformBin]);
            ps2DPedHist   ->Fill(bin, ps_PedCorrectedWave) ;

            muHist        ->Fill(bin, muWaveform[waveformBin]);
            muPedHist     ->Fill(bin, mu_PedCorrectedWave);
            mu2DHist      ->Fill(bin, muWaveform[waveformBin]);
            mu2DPedHist   ->Fill(bin, mu_PedCorrectedWave);

            tcHist        ->Fill(bin, tcWaveform[waveformBin]);
            tcPedHist     ->Fill(bin, tc_PedCorrectedWave);
            tc2DHist      ->Fill(bin, tcWaveform[waveformBin] );
            tc2DPedHist   ->Fill(bin, tc_PedCorrectedWave );

            dwc1RHist     ->Fill(bin, dwc1_waveform.at(0)[waveformBin]);
            dwc1RPedHist  ->Fill(bin, dwc1R_PedCorrectedWave);

            dwc1LHist     ->Fill(bin, dwc1_waveform.at(1)[waveformBin]);
            dwc1LPedHist  ->Fill(bin, dwc1L_PedCorrectedWave);

            dwc1UHist     ->Fill(bin, dwc1_waveform.at(2)[waveformBin]);
            dwc1UPedHist  ->Fill(bin, dwc1U_PedCorrectedWave);

            dwc1DHist     ->Fill(bin, dwc1_waveform.at(3)[waveformBin]);
            dwc1DPedHist  ->Fill(bin, dwc1D_PedCorrectedWave);

            dwc2RHist     ->Fill(bin, dwc2_waveform.at(0)[waveformBin]);
            dwc2RPedHist  ->Fill(bin, dwc2R_PedCorrectedWave);

            dwc2LHist     ->Fill(bin, dwc2_waveform.at(1)[waveformBin]);
            dwc2LPedHist  ->Fill(bin, dwc2L_PedCorrectedWave);

            dwc2UHist     ->Fill(bin, dwc2_waveform.at(2)[waveformBin]);
            dwc2UPedHist  ->Fill(bin, dwc2U_PedCorrectedWave);

            dwc2DHist     ->Fill(bin, dwc2_waveform.at(3)[waveformBin]);
            dwc2DPedHist  ->Fill(bin, dwc2D_PedCorrectedWave);
            // M1 C
            M1T1CHist     ->Fill(bin, M1TC_waveform.at(0)[waveformBin]);
            M1T1CPedHist  ->Fill(bin, M1T1C_PedCorrectedWave);
            M1T1C2DHist   ->Fill(bin, M1TC_waveform.at(0)[waveformBin]);
            M1T1C2DPedHist->Fill(bin, M1T1C_PedCorrectedWave);  

            M1T2CHist     ->Fill(bin, M1TC_waveform.at(1)[waveformBin]);
            M1T2CPedHist  ->Fill(bin, M1T2C_PedCorrectedWave);
            M1T2C2DHist   ->Fill(bin, M1TC_waveform.at(1)[waveformBin]);
            M1T2C2DPedHist->Fill(bin, M1T2C_PedCorrectedWave);

            M1T3CHist     ->Fill(bin, M1TC_waveform.at(2)[waveformBin]);
            M1T3CPedHist  ->Fill(bin, M1T3C_PedCorrectedWave);
            M1T3C2DHist   ->Fill(bin, M1TC_waveform.at(2)[waveformBin]);
            M1T3C2DPedHist->Fill(bin, M1T3C_PedCorrectedWave);

            M1T4CHist     ->Fill(bin, M1TC_waveform.at(3)[waveformBin]);
            M1T4CPedHist  ->Fill(bin, M1T4C_PedCorrectedWave);
            M1T4C2DHist   ->Fill(bin, M1TC_waveform.at(3)[waveformBin]);
            M1T4C2DPedHist->Fill(bin, M1T4C_PedCorrectedWave);   
            // M1 S
            M1T1SHist     ->Fill(bin, M1TS_waveform.at(0)[waveformBin]);
            M1T1SPedHist  ->Fill(bin, M1T1S_PedCorrectedWave);
            M1T1S2DHist   ->Fill(bin, M1TS_waveform.at(0)[waveformBin]);
            M1T1S2DPedHist->Fill(bin, M1T1S_PedCorrectedWave);  

            M1T2SHist     ->Fill(bin, M1TS_waveform.at(1)[waveformBin]);
            M1T2SPedHist  ->Fill(bin, M1T2S_PedCorrectedWave);
            M1T2S2DHist   ->Fill(bin, M1TS_waveform.at(1)[waveformBin]);
            M1T2S2DPedHist->Fill(bin, M1T2S_PedCorrectedWave);

            M1T3SHist     ->Fill(bin, M1TS_waveform.at(2)[waveformBin]);
            M1T3SPedHist  ->Fill(bin, M1T3S_PedCorrectedWave);
            M1T3S2DHist   ->Fill(bin, M1TS_waveform.at(2)[waveformBin]);
            M1T3S2DPedHist->Fill(bin, M1T3S_PedCorrectedWave);

            M1T4SHist     ->Fill(bin, M1TS_waveform.at(3)[waveformBin]);
            M1T4SPedHist  ->Fill(bin, M1T4S_PedCorrectedWave);
            M1T4S2DHist   ->Fill(bin, M1TS_waveform.at(3)[waveformBin]);
            M1T4S2DPedHist->Fill(bin, M1T4S_PedCorrectedWave);
            // M2 C
            M2T1CHist     ->Fill(bin, M2TC_waveform.at(0)[waveformBin]);
            M2T1CPedHist  ->Fill(bin, M2T1C_PedCorrectedWave);
            M2T1C2DHist   ->Fill(bin, M2TC_waveform.at(0)[waveformBin]);
            M2T1C2DPedHist->Fill(bin, M2T1C_PedCorrectedWave);  

            M2T2CHist     ->Fill(bin, M2TC_waveform.at(1)[waveformBin]);
            M2T2CPedHist  ->Fill(bin, M2T2C_PedCorrectedWave);
            M2T2C2DHist   ->Fill(bin, M2TC_waveform.at(1)[waveformBin]);
            M2T2C2DPedHist->Fill(bin, M2T2C_PedCorrectedWave);

            M2T3CHist     ->Fill(bin, M2TC_waveform.at(2)[waveformBin]);
            M2T3CPedHist  ->Fill(bin, M2T3C_PedCorrectedWave);
            M2T3C2DHist   ->Fill(bin, M2TC_waveform.at(2)[waveformBin]);
            M2T3C2DPedHist->Fill(bin, M2T3C_PedCorrectedWave);

            M2T4CHist     ->Fill(bin, M2TC_waveform.at(3)[waveformBin]);
            M2T4CPedHist  ->Fill(bin, M2T4C_PedCorrectedWave);
            M2T4C2DHist   ->Fill(bin, M2TC_waveform.at(3)[waveformBin]);
            M2T4C2DPedHist->Fill(bin, M2T4C_PedCorrectedWave);  

            M2T6CHist     ->Fill(bin, M2TC_waveform.at(4)[waveformBin]);
            M2T6CPedHist  ->Fill(bin, M2T6C_PedCorrectedWave);
            M2T6C2DHist   ->Fill(bin, M2TC_waveform.at(4)[waveformBin]);
            M2T6C2DPedHist->Fill(bin, M2T6C_PedCorrectedWave);  

            M2T7CHist     ->Fill(bin, M2TC_waveform.at(5)[waveformBin]);
            M2T7CPedHist  ->Fill(bin, M2T7C_PedCorrectedWave);
            M2T7C2DHist   ->Fill(bin, M2TC_waveform.at(5)[waveformBin]);
            M2T7C2DPedHist->Fill(bin, M2T7C_PedCorrectedWave);

            M2T8CHist     ->Fill(bin, M2TC_waveform.at(6)[waveformBin]);
            M2T8CPedHist  ->Fill(bin, M2T8C_PedCorrectedWave);
            M2T8C2DHist   ->Fill(bin, M2TC_waveform.at(6)[waveformBin]);
            M2T8C2DPedHist->Fill(bin, M2T8C_PedCorrectedWave);

            M2T9CHist     ->Fill(bin, M2TC_waveform.at(7)[waveformBin]);
            M2T9CPedHist  ->Fill(bin, M2T9C_PedCorrectedWave);
            M2T9C2DHist   ->Fill(bin, M2TC_waveform.at(7)[waveformBin]);
            M2T9C2DPedHist->Fill(bin, M2T9C_PedCorrectedWave);
            // M2 S
            M2T1SHist     ->Fill(bin, M2TS_waveform.at(0)[waveformBin]);
            M2T1SPedHist  ->Fill(bin, M2T1S_PedCorrectedWave);
            M2T1S2DHist   ->Fill(bin, M2TS_waveform.at(0)[waveformBin]);
            M2T1S2DPedHist->Fill(bin, M2T1S_PedCorrectedWave);  

            M2T2SHist     ->Fill(bin, M2TS_waveform.at(1)[waveformBin]);
            M2T2SPedHist  ->Fill(bin, M2T2S_PedCorrectedWave);
            M2T2S2DHist   ->Fill(bin, M2TS_waveform.at(1)[waveformBin]);
            M2T2S2DPedHist->Fill(bin, M2T2S_PedCorrectedWave);

            M2T3SHist     ->Fill(bin, M2TS_waveform.at(2)[waveformBin]);
            M2T3SPedHist  ->Fill(bin, M2T3S_PedCorrectedWave);
            M2T3S2DHist   ->Fill(bin, M2TS_waveform.at(2)[waveformBin]);
            M2T3S2DPedHist->Fill(bin, M2T3S_PedCorrectedWave);

            M2T4SHist     ->Fill(bin, M2TS_waveform.at(3)[waveformBin]);
            M2T4SPedHist  ->Fill(bin, M2T4S_PedCorrectedWave);
            M2T4S2DHist   ->Fill(bin, M2TS_waveform.at(3)[waveformBin]);
            M2T4S2DPedHist->Fill(bin, M2T4S_PedCorrectedWave);  

            M2T6SHist     ->Fill(bin, M2TS_waveform.at(4)[waveformBin]);
            M2T6SPedHist  ->Fill(bin, M2T6S_PedCorrectedWave);
            M2T6S2DHist   ->Fill(bin, M2TS_waveform.at(4)[waveformBin]);
            M2T6S2DPedHist->Fill(bin, M2T6S_PedCorrectedWave);  

            M2T7SHist     ->Fill(bin, M2TS_waveform.at(5)[waveformBin]);
            M2T7SPedHist  ->Fill(bin, M2T7S_PedCorrectedWave);
            M2T7S2DHist   ->Fill(bin, M2TS_waveform.at(5)[waveformBin]);
            M2T7S2DPedHist->Fill(bin, M2T7S_PedCorrectedWave);

            M2T8SHist     ->Fill(bin, M2TS_waveform.at(6)[waveformBin]);
            M2T8SPedHist  ->Fill(bin, M2T8S_PedCorrectedWave);
            M2T8S2DHist   ->Fill(bin, M2TS_waveform.at(6)[waveformBin]);
            M2T8S2DPedHist->Fill(bin, M2T8S_PedCorrectedWave);

            M2T9SHist     ->Fill(bin, M2TS_waveform.at(7)[waveformBin]);
            M2T9SPedHist  ->Fill(bin, M2T9S_PedCorrectedWave);
            M2T9S2DHist   ->Fill(bin, M2TS_waveform.at(7)[waveformBin]);
            M2T9S2DPedHist->Fill(bin, M2T9S_PedCorrectedWave);
        }
    }

    float survivedEntries = (float) psHist->GetEntries() / 1000.;
    float scale = 1. / survivedEntries;

    std::string outFile = "./avgTimeStructure/avg_Run_" + std::to_string(runNum) + ".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");
    outputRoot->cd();

    psHist->Scale( scale );
    psHist->GetYaxis()->SetRangeUser(-100, 4096);
    psHist->Write();
    psPedHist->Scale( scale );
    psPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    psPedHist->Write();
    ps2DHist->Write();
    ps2DPedHist->Write();

    muHist->Scale( scale );
    muHist->GetYaxis()->SetRangeUser(-100, 4096);
    muHist->Write();
    muPedHist->Scale( scale );
    muPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    muPedHist->Write();
    mu2DHist->Write();
    mu2DPedHist->Write();

    tcHist->Scale( scale );
    tcHist->GetYaxis()->SetRangeUser(-100, 4096);
    tcHist->Write();
    tcPedHist->Scale( scale );
    tcPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    tcPedHist->Write();
    tc2DHist->Write();
    tc2DPedHist->Write();

    dwc1RHist->Scale( scale );
    dwc1RHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc1RHist->Write();
    dwc1RPedHist->Scale( scale );
    dwc1RPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc1RPedHist->Write();

    dwc1LHist->Scale( scale );
    dwc1LHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc1LHist->Write();
    dwc1LPedHist->Scale( scale );
    dwc1LPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc1LPedHist->Write();

    dwc1UHist->Scale( scale );
    dwc1UHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc1UHist->Write();
    dwc1UPedHist->Scale( scale );
    dwc1UPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc1UPedHist->Write();

    dwc1DHist->Scale( scale );
    dwc1DHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc1DHist->Write();
    dwc1DPedHist->Scale( scale );
    dwc1DPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc1DPedHist->Write();

    dwc2RHist->Scale( scale );
    dwc2RHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc2RHist->Write();
    dwc2RPedHist->Scale( scale );
    dwc2RPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc2RPedHist->Write();

    dwc2LHist->Scale( scale );
    dwc2LHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc2LHist->Write();
    dwc2LPedHist->Scale( scale );
    dwc2LPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc2LPedHist->Write();

    dwc2UHist->Scale( scale );
    dwc2UHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc2UHist->Write();
    dwc2UPedHist->Scale( scale );
    dwc2UPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc2UPedHist->Write();

    dwc2DHist->Scale( scale );
    dwc2DHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc2DHist->Write();
    dwc2DPedHist->Scale( scale );
    dwc2DPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc2DPedHist->Write();
    // M1 C
    M1T1CHist->Scale( scale );
    M1T1CHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T1CHist->Write();
    M1T1CPedHist->Scale( scale );
    M1T1CPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T1CPedHist->Write();
    M1T1C2DHist->Write();
    M1T1C2DPedHist->Write();

    M1T2CHist->Scale( scale );
    M1T2CHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T2CHist->Write();
    M1T2CPedHist->Scale( scale );
    M1T2CPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T2CPedHist->Write();
    M1T2C2DHist->Write();
    M1T2C2DPedHist->Write();

    M1T3CHist->Scale( scale );
    M1T3CHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T3CHist->Write();
    M1T3CPedHist->Scale( scale );
    M1T3CPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T3CPedHist->Write();
    M1T3C2DHist->Write();
    M1T3C2DPedHist->Write();

    M1T4CHist->Scale( scale );
    M1T4CHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T4CHist->Write();
    M1T4CPedHist->Scale( scale );
    M1T4CPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T4CPedHist->Write();
    M1T4C2DHist->Write();
    M1T4C2DPedHist->Write();
    // M1 S
    M1T1SHist->Scale( scale );
    M1T1SHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T1SHist->Write();
    M1T1SPedHist->Scale( scale );
    M1T1SPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T1SPedHist->Write();
    M1T1S2DHist->Write();
    M1T1S2DPedHist->Write();

    M1T2SHist->Scale( scale );
    M1T2SHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T2SHist->Write();
    M1T2SPedHist->Scale( scale );
    M1T2SPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T2SPedHist->Write();
    M1T2S2DHist->Write();
    M1T2S2DPedHist->Write();

    M1T3SHist->Scale( scale );
    M1T3SHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T3SHist->Write();
    M1T3SPedHist->Scale( scale );
    M1T3SPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T3SPedHist->Write();
    M1T3S2DHist->Write();
    M1T3S2DPedHist->Write();

    M1T4SHist->Scale( scale );
    M1T4SHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T4SHist->Write();
    M1T4SPedHist->Scale( scale );
    M1T4SPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T4SPedHist->Write();
    M1T4S2DHist->Write();
    M1T4S2DPedHist->Write();
    // M2 C
    M2T1CHist->Scale( scale );
    M2T1CHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T1CHist->Write();
    M2T1CPedHist->Scale( scale );
    M2T1CPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T1CPedHist->Write();
    M2T1C2DHist->Write();
    M2T1C2DPedHist->Write();

    M2T2CHist->Scale( scale );
    M2T2CHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T2CHist->Write();
    M2T2CPedHist->Scale( scale );
    M2T2CPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T2CPedHist->Write();
    M2T2C2DHist->Write();
    M2T2C2DPedHist->Write();

    M2T3CHist->Scale( scale );
    M2T3CHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T3CHist->Write();
    M2T3CPedHist->Scale( scale );
    M2T3CPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T3CPedHist->Write();
    M2T3C2DHist->Write();
    M2T3C2DPedHist->Write();

    M2T4CHist->Scale( scale );
    M2T4CHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T4CHist->Write();
    M2T4CPedHist->Scale( scale );
    M2T4CPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T4CPedHist->Write();
    M2T4C2DHist->Write();
    M2T4C2DPedHist->Write();

    M2T6CHist->Scale( scale );
    M2T6CHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T6CHist->Write();
    M2T6CPedHist->Scale( scale );
    M2T6CPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T6CPedHist->Write();
    M2T6C2DHist->Write();
    M2T6C2DPedHist->Write();

    M2T7CHist->Scale( scale );
    M2T7CHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T7CHist->Write();
    M2T7CPedHist->Scale( scale );
    M2T7CPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T7CPedHist->Write();
    M2T7C2DHist->Write();
    M2T7C2DPedHist->Write();

    M2T8CHist->Scale( scale );
    M2T8CHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T8CHist->Write();
    M2T8CPedHist->Scale( scale );
    M2T8CPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T8CPedHist->Write();
    M2T8C2DHist->Write();
    M2T8C2DPedHist->Write();

    M2T9CHist->Scale( scale );
    M2T9CHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T9CHist->Write();
    M2T9CPedHist->Scale( scale );
    M2T9CPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T9CPedHist->Write();
    M2T9C2DHist->Write();
    M2T9C2DPedHist->Write();
    // M2 S
    M2T1SHist->Scale( scale );
    M2T1SHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T1SHist->Write();
    M2T1SPedHist->Scale( scale );
    M2T1SPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T1SPedHist->Write();
    M2T1S2DHist->Write();
    M2T1S2DPedHist->Write();

    M2T2SHist->Scale( scale );
    M2T2SHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T2SHist->Write();
    M2T2SPedHist->Scale( scale );
    M2T2SPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T2SPedHist->Write();
    M2T2S2DHist->Write();
    M2T2S2DPedHist->Write();

    M2T3SHist->Scale( scale );
    M2T3SHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T3SHist->Write();
    M2T3SPedHist->Scale( scale );
    M2T3SPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T3SPedHist->Write();
    M2T3S2DHist->Write();
    M2T3S2DPedHist->Write();

    M2T4SHist->Scale( scale );
    M2T4SHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T4SHist->Write();
    M2T4SPedHist->Scale( scale );
    M2T4SPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T4SPedHist->Write();
    M2T4S2DHist->Write();
    M2T4S2DPedHist->Write();

    M2T6SHist->Scale( scale );
    M2T6SHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T6SHist->Write();
    M2T6SPedHist->Scale( scale );
    M2T6SPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T6SPedHist->Write();
    M2T6S2DHist->Write();
    M2T6S2DPedHist->Write();

    M2T7SHist->Scale( scale );
    M2T7SHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T7SHist->Write();
    M2T7SPedHist->Scale( scale );
    M2T7SPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T7SPedHist->Write();
    M2T7S2DHist->Write();
    M2T7S2DPedHist->Write();

    M2T8SHist->Scale( scale );
    M2T8SHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T8SHist->Write();
    M2T8SPedHist->Scale( scale );
    M2T8SPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T8SPedHist->Write();
    M2T8S2DHist->Write();
    M2T8S2DPedHist->Write();

    M2T9SHist->Scale( scale );
    M2T9SHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T9SHist->Write();
    M2T9SPedHist->Scale( scale );
    M2T9SPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    M2T9SPedHist->Write();
    M2T9S2DHist->Write();
    M2T9S2DPedHist->Write();
    
    dwc1_pos->Write();
    dwc2_pos->Write();
    dwc1_correctedPos->Write();
    dwc2_correctedPos->Write();

    dwc1_pos_PID->Write();
    dwc2_pos_PID->Write();
    dwc1_correctedPos_PID->Write();
    dwc2_correctedPos_PID->Write();

    dwc_x_corr->Write();
    dwc_y_corr->Write();
    dwc_corrected_x_corr->Write();
    dwc_corrected_y_corr->Write();

    dwc_x_corr_PID->Write();
    dwc_y_corr_PID->Write();
    dwc_corrected_x_corr_PID->Write();
    dwc_corrected_y_corr_PID->Write();

    outputRoot->Close();
}
