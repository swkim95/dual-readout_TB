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
    TFile* dwcFile = TFile::Open(("/u/user/swkim/data_certificate/dual-readout_TB/dwc/dwc_Run_" + std::to_string(runNum) + ".root").c_str());
    if (dwcFile->IsOpen()) {
        std::cout << "Opened DWC info : u/user/swkim/data_certificate/dual-readout_TB/dwc/dwc_Run_" + std::to_string(runNum) + ".root" << std::endl;
    }
    else {
        std::cout << "[ERROR] Failed to open DWC info : u/user/swkim/data_certificate/dual-readout_TB/dwc/dwc_Run_" + std::to_string(runNum) + ".root" << std::endl;
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

    // Get pedestals
    float psPed = utility.retrievePed(pscid);
    float muPed = utility.retrievePed(mucid);

    // Preparing histograms
    TH1F* psIntHist = new TH1F("psIntADC", "psIntADC;IntADC;evt", 1000, -5000., 200000.);
    TH1F* psIntHist_PID = new TH1F("psIntADC_PID", "psIntADC_PID;IntADC;evt", 1000, -5000., 200000.);

    TH1F* muIntHist = new TH1F("muIntADC", "muIntADC;IntADC;evt", 1000, -10000., 50000.);
    TH1F* muIntHist_PID = new TH1F("muIntADC_PID", "muIntADC_PID;IntADC;evt", 1000, -10000., 50000.);

    TH2D* dwc1_correctedPos = new TH2D("dwc1_correctedPos", "dwc1_correctedPos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* dwc2_correctedPos = new TH2D("dwc2_correctedPos", "dwc2_correctedPos;mm;mm;events", 480, -120., 120., 480, -120., 120.);

    // Load data using TChain
    TChain* evtChain = new TChain("events");
    for (int fn = 0; fn < 50; fn++) {
        std::string fileName = "ntuple_Run_" + std::to_string(runNum) + "_Wave_" + std::to_string(fn) + ".root";
        std::string filePath = "/gatbawi/dream/ntuple/waveform/Run_"  + std::to_string(runNum) + "/" + fileName;
        if ( !access(filePath.c_str(), F_OK) ){
            std::cout << fn << " Ntuple file added to TChain : " << filePath << std::endl;
            evtChain->Add(filePath.c_str());
        }
    }
    TBevt<TBwaveform>* anEvt = new TBevt<TBwaveform>(); 
    evtChain->SetBranchAddress("TBevt", &anEvt);

    // Check total # of events
    int totalEntry = evtChain->GetEntries();
    std::cout << "Total entries : " << totalEntry << std::endl;
    if ( (maxEntry > 0) && (maxEntry < totalEntry) ) {
        totalEntry = maxEntry;
        std::cout << "Will process maximum " << std::to_string(totalEntry) << " events" << std::endl;
    }

    // Get DWC offset
    TH2D* dwc1_pos   = (TH2D*) dwcFile->Get("dwc1_pos");
    TH2D* dwc2_pos   = (TH2D*) dwcFile->Get("dwc2_pos");
    std::vector<float> dwc1_offset = getDWCoffset(dwc1_pos);
    std::vector<float> dwc2_offset = getDWCoffset(dwc2_pos);

    // Evt Loop
    for (int iEvt = 0; iEvt < totalEntry; iEvt++) {
        printProgress(iEvt + 1, totalEntry);

        evtChain->GetEntry(iEvt);
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
        dwc1_correctedPos->Fill(dwc1_correctedPosition.at(0), dwc1_correctedPosition.at(1));
        dwc2_correctedPos->Fill(dwc2_correctedPosition.at(0), dwc2_correctedPosition.at(1));

        // PS Int. ADC
        float psIntADC = 0.f;
        for (int bin = 240; bin < 520; bin++) {
            int waveformBin = bin + 1;
            psIntADC += psPed - psWaveform[waveformBin];
        }
        // MC Int. ADC
        float muIntADC = 0.f;
        for (int bin = 820; bin < 1000; bin++) {
            int waveformBin = bin + 1;
            muIntADC += muPed - muWaveform[waveformBin];
        }
        // PS, MC Int. ADC plot before PID
        psIntHist->Fill(psIntADC);
        muIntHist->Fill(muIntADC);
    
        // For DWC PID
        if (! dwcCorrelationPID(dwc1_correctedPosition, dwc2_correctedPosition, 2.f) ) continue;

        // PS, MC Int. ADC plot after PID
        psIntHist_PID->Fill(psIntADC);
        muIntHist_PID->Fill(muIntADC);
    }

    std::string outFile = "/u/user/swkim/data_certificate/dual-readout_TB/analysis/auxPID/auxPID_Run_" + std::to_string(runNum) + ".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");
    outputRoot->cd();

    // Fit functions
    TF1* psFitFunc = new TF1("psFitFunc", "gaus", 5000, 21000);
    // TF1* muFitFunc = new TF1("muFitFunc", "gaus", 5000, 21000);

    psIntHist->Write();
    psIntHist_PID->Fit("psFitFunc", "R");
    psIntHist_PID->Write();
    psFitFunc->Write();

    muIntHist->Write();
    // muIntHist_PID->Fit("muFitFunc", "R");
    muIntHist_PID->Write();
    // muFitFunc->Write();

    dwc1_pos->Write();
    dwc2_pos->Write();
    dwc1_correctedPos->Write();
    dwc2_correctedPos->Write();
    
    outputRoot->Close();
}
