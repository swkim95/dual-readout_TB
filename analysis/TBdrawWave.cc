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

    std::string pedPath = "/gatbawi/dream/ped/mean/Run" + std::to_string(runNum) + "_pedestalHist_mean.root";

    TButility utility = TButility();
    utility.loading("/gatbawi/dream/mapping/mapping_Aug2022TB.root");
    utility.loadped(pedPath);

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

    TH1F* psHist = new TH1F("psHist", "psHist", 1000, 0, 1000);
    psHist->GetYaxis()->SetRangeUser(-100, 4096);
    TH1F* psPedHist = new TH1F("psPedHist", "psPedHist", 1000, 0, 1000);
    psPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    TH2F* ps2DHist = new TH2F("ps2DHist", "ps2DHist", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* ps2DPedHist = new TH2F("ps2DPedHist", "ps2DPedHist", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* muHist = new TH1F("muHist", "muHist", 1000, 0, 1000);
    muHist->GetYaxis()->SetRangeUser(-100, 4096);
    TH1F* muPedHist = new TH1F("muPedHist", "muPedHist", 1000, 0, 1000);
    muPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    TH2F* mu2DHist = new TH2F("mu2DHist", "mu2DHist", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* mu2DPedHist = new TH2F("mu2DPedHist", "mu2DPedHist", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* tcHist = new TH1F("tcHist", "tcHist", 1000, 0, 1000);
    tcHist->GetYaxis()->SetRangeUser(-100, 4096);
    TH1F* tcPedHist = new TH1F("tcPedHist", "tcPedHist", 1000, 0, 1000);
    tcPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    TH2F* tc2DHist = new TH2F("tc2DHist", "tc2DHist", 1000, 0, 1000, 4096, 0, 4096);
    TH2F* tc2DPedHist = new TH2F("tc2DPedHist", "tc2DPedHist", 1000, 0, 1000, 5096, -1000, 4096);

    TH1F* dwc1RHist = new TH1F("dwc1RHist", "dwc1RHist", 1000, 0, 1000);
    TH1F* dwc1RPedHist = new TH1F("dwc1RPedHist", "dwc1RPedHist", 1000, 0, 1000);
    dwc1RHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc1RPedHist->GetYaxis()->SetRangeUser(-100, 4096);    

    TH1F* dwc1LHist = new TH1F("dwc1LHist", "dwc1LHist", 1000, 0, 1000);
    TH1F* dwc1LPedHist = new TH1F("dwc1LPedHist", "dwc1LPedHist", 1000, 0, 1000);
    dwc1LHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc1LPedHist->GetYaxis()->SetRangeUser(-100, 4096);    

    TH1F* dwc1UHist = new TH1F("dwc1UHist", "dwc1UHist", 1000, 0, 1000);
    TH1F* dwc1UPedHist = new TH1F("dwc1UPedHist", "dwc1UPedHist", 1000, 0, 1000);
    dwc1UHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc1UPedHist->GetYaxis()->SetRangeUser(-100, 4096);    

    TH1F* dwc1DHist = new TH1F("dwc1DHist", "dwc1DHist", 1000, 0, 1000);
    TH1F* dwc1DPedHist = new TH1F("dwc1DPedHist", "dwc1DPedHist", 1000, 0, 1000);
    dwc1DHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc1DPedHist->GetYaxis()->SetRangeUser(-100, 4096);    

    TH1F* dwc2RHist = new TH1F("dwc2RHist", "dwc2RHist", 1000, 0, 1000);
    TH1F* dwc2RPedHist = new TH1F("dwc2RPedHist", "dwc2RPedHist", 1000, 0, 1000);
    dwc2RHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc2RPedHist->GetYaxis()->SetRangeUser(-100, 4096);    

    TH1F* dwc2LHist = new TH1F("dwc2LHist", "dwc2LHist", 1000, 0, 1000);
    TH1F* dwc2LPedHist = new TH1F("dwc2LPedHist", "dwc2LPedHist", 1000, 0, 1000);
    dwc2LHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc2LPedHist->GetYaxis()->SetRangeUser(-100, 4096);    

    TH1F* dwc2UHist = new TH1F("dwc2UHist", "dwc2UHist", 1000, 0, 1000);
    TH1F* dwc2UPedHist = new TH1F("dwc2UPedHist", "dwc2UPedHist", 1000, 0, 1000);
    dwc2UHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc2UPedHist->GetYaxis()->SetRangeUser(-100, 4096);    

    TH1F* dwc2DHist = new TH1F("dwc2DHist", "dwc2DHist", 1000, 0, 1000);
    TH1F* dwc2DPedHist = new TH1F("dwc2DPedHist", "dwc2DPedHist", 1000, 0, 1000);
    dwc2DHist->GetYaxis()->SetRangeUser(-100, 4096);
    dwc2DPedHist->GetYaxis()->SetRangeUser(-100, 4096);    

    TH1F* M1T1CHist = new TH1F("M1T1CHist", "M1T1CHist", 1000, 0, 1000);
    TH1F* M1T1CPedHist = new TH1F("M1T1CPedHist", "M1T1CPedHist", 1000, 0, 1000);
    M1T1CHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T1CPedHist->GetYaxis()->SetRangeUser(-100, 4096);
    
    TH1F* M1T2CHist = new TH1F("M1T2CHist", "M1T2CHist", 1000, 0, 1000);
    TH1F* M1T2CPedHist = new TH1F("M1T2CPedHist", "M1T2CPedHist", 1000, 0, 1000);
    M1T2CHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T2CPedHist->GetYaxis()->SetRangeUser(-100, 4096);

    TH1F* M1T3CHist = new TH1F("M1T3CHist", "M1T3CHist", 1000, 0, 1000);
    TH1F* M1T3CPedHist = new TH1F("M1T3CPedHist", "M1T3CPedHist", 1000, 0, 1000);
    M1T3CHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T3CPedHist->GetYaxis()->SetRangeUser(-100, 4096);

    TH1F* M1T4CHist = new TH1F("M1T4CHist", "M1T4CHist", 1000, 0, 1000);
    TH1F* M1T4CPedHist = new TH1F("M1T4CPedHist", "M1T4CPedHist", 1000, 0, 1000);
    M1T4CHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T4CPedHist->GetYaxis()->SetRangeUser(-100, 4096);

    TH1F* M1T1SHist = new TH1F("M1T1SHist", "M1T1SHist", 1000, 0, 1000);
    TH1F* M1T1SPedHist = new TH1F("M1T1SPedHist", "M1T1SPedHist", 1000, 0, 1000);
    M1T1SHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T1SPedHist->GetYaxis()->SetRangeUser(-100, 4096);

    TH1F* M1T2SHist = new TH1F("M1T2SHist", "M1T2SHist", 1000, 0, 1000);
    TH1F* M1T2SPedHist = new TH1F("M1T2SPedHist", "M1T2SPedHist", 1000, 0, 1000);
    M1T2SHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T2SPedHist->GetYaxis()->SetRangeUser(-100, 4096);

    TH1F* M1T3SHist = new TH1F("M1T3SHist", "M1T3SHist", 1000, 0, 1000);
    TH1F* M1T3SPedHist = new TH1F("M1T3SPedHist", "M1T3SPedHist", 1000, 0, 1000);
    M1T3SHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T3SPedHist->GetYaxis()->SetRangeUser(-100, 4096);

    TH1F* M1T4SHist = new TH1F("M1T4SHist", "M1T4SHist", 1000, 0, 1000);
    TH1F* M1T4SPedHist = new TH1F("M1T4SPedHist", "M1T4SPedHist", 1000, 0, 1000);
    M1T4SHist->GetYaxis()->SetRangeUser(-100, 4096);
    M1T4SPedHist->GetYaxis()->SetRangeUser(-100, 4096);

    TChain* evtChain = new TChain("events");

    for (int fn = 0; fn < 50; fn++) {
        std::string fileName = "ntuple_Run_" + std::to_string(runNum) + "_Wave_" + std::to_string(fn) + ".root";
        std::string filePath = "/gatbawi/dream/ntuple/waveform/Run_"  + std::to_string(runNum) + "/" + fileName;
        if ( !access(filePath.c_str(), F_OK) ){
            std::cout << fn << "th ntuple added to TChain : " << filePath << std::endl;
            evtChain->Add(filePath.c_str());
        }
    }
    int totalEntry = evtChain->GetEntries();

    std::cout << "Total entries : " << totalEntry << std::endl;
    if ( (maxEntry > 0) && (maxEntry < totalEntry) ) {
        totalEntry = maxEntry;
        std::cout << "Will process maximum " << std::to_string(totalEntry) << " events" << std::endl;
    }

    TBevt<TBwaveform>* anEvt = new TBevt<TBwaveform>(); 
    evtChain->SetBranchAddress("TBevt", &anEvt);

    // Evt Loop
    for (int iEvt = 0; iEvt < totalEntry; iEvt++) {
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

            psHist->Fill(bin, ( (float)psWaveform[waveformBin] / (float)totalEntry) );
            psPedHist->Fill(bin, (float) ((float)ps_PedCorrectedWave / (float)totalEntry) );
            ps2DHist->Fill(bin, (float)psWaveform[waveformBin]);
            ps2DPedHist->Fill(bin, (float)ps_PedCorrectedWave) ;

            muHist->Fill(bin, ( (float)muWaveform[waveformBin] / (float)totalEntry) );
            muPedHist->Fill(bin, ((float)mu_PedCorrectedWave / (float)totalEntry) );
            mu2DHist->Fill(bin, (float)muWaveform[waveformBin] );
            mu2DPedHist->Fill(bin, (float)mu_PedCorrectedWave );

            tcHist->Fill(bin, ( (float)tcWaveform[waveformBin] / (float)totalEntry) );
            tcPedHist->Fill(bin, ( (float)tc_PedCorrectedWave / (float)totalEntry) );
            tc2DHist->Fill(bin, (float)tcWaveform[waveformBin] );
            tc2DPedHist->Fill(bin, (float)tc_PedCorrectedWave );

            dwc1RHist->Fill(bin, ( (float)dwc1_waveform.at(0)[waveformBin] / (float)totalEntry) );
            dwc1RPedHist->Fill(bin, ( (float)dwc1R_PedCorrectedWave / (float)totalEntry) );

            dwc1LHist->Fill(bin, ( (float)dwc1_waveform.at(1)[waveformBin] / (float)totalEntry) );
            dwc1LPedHist->Fill(bin, ( (float)dwc1L_PedCorrectedWave / (float)totalEntry) );

            dwc1UHist->Fill(bin, ( (float)dwc1_waveform.at(2)[waveformBin] / (float)totalEntry) );
            dwc1UPedHist->Fill(bin, ( (float)dwc1U_PedCorrectedWave / (float)totalEntry) );

            dwc1DHist->Fill(bin, ( (float)dwc1_waveform.at(3)[waveformBin] / (float)totalEntry) );
            dwc1DPedHist->Fill(bin, ( (float)dwc1D_PedCorrectedWave / (float)totalEntry) );

            dwc2RHist->Fill(bin, ( (float)dwc2_waveform.at(0)[waveformBin] / (float)totalEntry) );
            dwc2RPedHist->Fill(bin, ( (float)dwc2R_PedCorrectedWave / (float)totalEntry) );

            dwc2LHist->Fill(bin, ( (float)dwc2_waveform.at(1)[waveformBin] / (float)totalEntry) );
            dwc2LPedHist->Fill(bin, ( (float)dwc2L_PedCorrectedWave / (float)totalEntry) );

            dwc2UHist->Fill(bin, ( (float)dwc2_waveform.at(2)[waveformBin] / (float)totalEntry) );
            dwc2UPedHist->Fill(bin, ( (float)dwc2U_PedCorrectedWave / (float)totalEntry) );

            dwc2DHist->Fill(bin, ( (float)dwc2_waveform.at(3)[waveformBin] / (float)totalEntry) );
            dwc2DPedHist->Fill(bin, ( (float)dwc2D_PedCorrectedWave / (float)totalEntry) );
        }

        if ( (iEvt % 1000) == 0 ) printProgress(iEvt + 1, totalEntry);
    }
    std::string outFile = "/u/user/swkim/data_certificate/dual-readout_TB/analysis/AuxWaveform/AuxWaveform_Run_" + std::to_string(runNum) + ".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");
    outputRoot->cd();

    psHist->Write();
    psPedHist->Write();
    ps2DHist->Write();
    ps2DPedHist->Write();

    muHist->Write();
    muPedHist->Write();
    mu2DHist->Write();
    mu2DPedHist->Write();

    tcHist->Write();
    tcPedHist->Write();
    tc2DHist->Write();
    tc2DPedHist->Write();

    dwc1RHist->Write();
    dwc1RPedHist->Write();
    dwc1LHist->Write();
    dwc1LPedHist->Write();
    dwc1UHist->Write();
    dwc1UPedHist->Write();
    dwc1DHist->Write();
    dwc1DPedHist->Write();

    dwc2RHist->Write();
    dwc2RPedHist->Write();
    dwc2LHist->Write();
    dwc2LPedHist->Write();
    dwc2UHist->Write();
    dwc2UPedHist->Write();
    dwc2DHist->Write();
    dwc2DPedHist->Write();

    outputRoot->Close();
}
