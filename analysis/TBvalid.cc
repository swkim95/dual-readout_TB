#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <filesystem>

#include "TBevt.h"
#include "TBmid.h"
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

namespace fs = std::filesystem;

TBmidbase readMetadata(FILE* fp) {
    char data[64];
    int data_length;
    int run_number;
    int tcb_trig_type;
    int tcb_trig_number;
    long long tcb_trig_time;
    int mid;
    int local_trig_number;
    int local_trigger_pattern;
    long long local_trig_time;
    long long fine_time;
    long long coarse_time;
    int itmp;
    long long ltmp;

    // read header
    fread(data, 1, 64, fp);

    // data length
    data_length = data[0] & 0xFF;
    itmp = data[1] & 0xFF;
    itmp = itmp << 8;
    data_length = data_length + itmp;
    itmp = data[2] & 0xFF;
    itmp = itmp << 16;
    data_length = data_length + itmp;
    itmp = data[3] & 0xFF;
    itmp = itmp << 24;
    data_length = data_length + itmp;

    // run number
    run_number = data[4] & 0xFF;
    itmp = data[5] & 0xFF;
    itmp = itmp << 8;
    run_number = run_number + itmp;

    // trigger type
    tcb_trig_type = data[6] & 0xFF;

    // TCB trigger #
    tcb_trig_number = data[7] & 0xFF;
    itmp = data[8] & 0xFF;
    itmp = itmp << 8;
    tcb_trig_number = tcb_trig_number + itmp;
    itmp = data[9] & 0xFF;
    itmp = itmp << 16;
    tcb_trig_number = tcb_trig_number + itmp;
    itmp = data[10] & 0xFF;
    itmp = itmp << 24;
    tcb_trig_number = tcb_trig_number + itmp;

    // TCB trigger time
    fine_time = data[11] & 0xFF;
    fine_time = fine_time * 11;     // actually * (1000 / 90)
    coarse_time = data[12] & 0xFF;
    ltmp = data[13] & 0xFF;
    ltmp = ltmp << 8;
    coarse_time = coarse_time + ltmp;
    ltmp = data[14] & 0xFF;
    ltmp = ltmp << 16;
    coarse_time = coarse_time + ltmp;
    ltmp = data[15] & 0xFF;
    ltmp = ltmp << 24;
    coarse_time = coarse_time + ltmp;
    ltmp = data[16] & 0xFF;
    ltmp = ltmp << 32;
    coarse_time = coarse_time + ltmp;
    ltmp = data[17] & 0xFF;
    ltmp = ltmp << 40;
    coarse_time = coarse_time + ltmp;
    coarse_time = coarse_time * 1000;   // get ns
    tcb_trig_time = fine_time + coarse_time;

    // mid
    mid = data[18] & 0xFF;

    // local trigger #
    local_trig_number = data[19] & 0xFF;
    itmp = data[20] & 0xFF;
    itmp = itmp << 8;
    local_trig_number = local_trig_number + itmp;
    itmp = data[21] & 0xFF;
    itmp = itmp << 16;
    local_trig_number = local_trig_number + itmp;
    itmp = data[22] & 0xFF;
    itmp = itmp << 24;
    local_trig_number = local_trig_number + itmp;

    // local trigger #
    local_trigger_pattern = data[23] & 0xFF;
    itmp = data[24] & 0xFF;
    itmp = itmp << 8;
    local_trigger_pattern = local_trigger_pattern + itmp;
    itmp = data[25] & 0xFF;
    itmp = itmp << 16;
    local_trigger_pattern = local_trigger_pattern + itmp;
    itmp = data[26] & 0xFF;
    itmp = itmp << 24;
    local_trigger_pattern = local_trigger_pattern + itmp;

    // local trigger time
    fine_time = data[27] & 0xFF;
    fine_time = fine_time * 11;     // actually * (1000 / 90)
    coarse_time = data[28] & 0xFF;
    ltmp = data[29] & 0xFF;
    ltmp = ltmp << 8;
    coarse_time = coarse_time + ltmp;
    ltmp = data[30] & 0xFF;
    ltmp = ltmp << 16;
    coarse_time = coarse_time + ltmp;
    ltmp = data[31] & 0xFF;
    ltmp = ltmp << 24;
    coarse_time = coarse_time + ltmp;
    ltmp = data[32] & 0xFF;
    ltmp = ltmp << 32;
    coarse_time = coarse_time + ltmp;
    ltmp = data[33] & 0xFF;
    ltmp = ltmp << 40;
    coarse_time = coarse_time + ltmp;
    coarse_time = coarse_time * 1000;   // get ns
    local_trig_time = fine_time + coarse_time;

    auto amid = TBmidbase(tcb_trig_number,run_number,mid);
    amid.setTCB(tcb_trig_type,tcb_trig_number,tcb_trig_time);
    amid.setLocal(local_trig_number,local_trigger_pattern,local_trig_time);

    return std::move(amid);
}

TBmid<TBfastmode> readFastmode(FILE* fp) {
    const auto base = readMetadata(fp);

    short data[96];
    int energy;
    int timing;

    // read fast data
    fread(data, 2, 96, fp);

    unsigned int channelsize = 32;

    auto amid = TBmid<TBfastmode>(base);
    std::vector<TBfastmode> fastmodes;
    fastmodes.reserve(channelsize);

    for (unsigned int idx = 0; idx < channelsize; idx++) {
        auto afast = TBfastmode();
        afast.setChannel(idx+1); // WARNING channel number 1 - 32

        // fill waveform for channel to plot
        energy = data[idx * 3 + 1] & 0xFFFF;
        energy = energy * 65536;
        energy = energy + (data[idx * 3] & 0xFFFF);

        timing = data[idx * 3 + 2] & 0xFFFF;

        afast.setAdc(energy);
        afast.setTiming(timing);

        fastmodes.emplace_back(afast);
    }

    amid.setChannels(fastmodes);

    return std::move(amid);
}

void printProgress(int current, int total) {
    // print progress
    float progress = (float) current / total;
    int barWidth = 70;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; i++) {
        if ( i < pos ) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "]  " << current << "/" << total << "  " << int(progress * 100.0) << "%\r";
    std::cout.flush();
}

// const int numOfMID = 15;
// const int numOfFiles = 5;

int main(int argc, char** argv) {
    gStyle->SetOptFit(1);

    TString dataBaseDir    = "/Users/swkim/DRC/dual-readout_TB/";
    // TString ntupleBaseDir = "/Users/swkim/DRC/dual-readout_TB/test/";
    TString runNum    = argv[1];
    const int numOfMID = atoi(argv[2]);
    const int numOfFiles = atoi(argv[3]);

    TH1F* h_fastADCFromData_C = new TH1F("h_fastADCFromData_C", "h_fastADCFromData_C", 1024, -1024., 40960.); h_fastADCFromData_C->SetLineWidth(2); h_fastADCFromData_C->SetLineColor(kBlue);
    TH1F* h_fastADCFromData_S = new TH1F("h_fastADCFromData_S", "h_fastADCFromData_S", 1024, -1024., 40960.); h_fastADCFromData_S->SetLineWidth(2); h_fastADCFromData_S->SetLineColor(kRed);

    TH1F* h_fastADCFromNtuple_C = new TH1F("h_fastADCFromNtuple_C", "h_fastADCFromNtuple_C", 1024, -1024., 40960.); h_fastADCFromNtuple_C->SetLineWidth(2); h_fastADCFromNtuple_C->SetLineColor(kBlue);
    TH1F* h_fastADCFromNtuple_S = new TH1F("h_fastADCFromNtuple_S", "h_fastADCFromNtuple_S", 1024, -1024., 40960.); h_fastADCFromNtuple_S->SetLineWidth(2); h_fastADCFromNtuple_S->SetLineColor(kRed);

    TH1F* h_fastADCFromData_Mu = new TH1F("h_fastADCFromData_Mu", "h_fastADCFromData_Mu", 1024, -1024., 40960.); h_fastADCFromData_Mu->SetLineWidth(2); h_fastADCFromData_Mu->SetLineColor(kBlack);
    TH1F* h_fastADCFromData_Ps = new TH1F("h_fastADCFromData_Ps", "h_fastADCFromData_Ps", 1024, -1024., 40960.*4.); h_fastADCFromData_Ps->SetLineWidth(2); h_fastADCFromData_Ps->SetLineColor(kBlack);
    TH1F* h_fastADCFromData_Tc = new TH1F("h_fastADCFromData_Tc", "h_fastADCFromData_Tc", 1024, -1024., 40960.); h_fastADCFromData_Tc->SetLineWidth(2); h_fastADCFromData_Tc->SetLineColor(kBlack);

    TH1F* h_fastADCFromNtuple_Mu = new TH1F("h_fastADCFromNtuple_Mu", "h_fastADCFromNtuple_Mu", 1024, -1024., 40960.); h_fastADCFromNtuple_Mu->SetLineWidth(2); h_fastADCFromNtuple_Mu->SetLineColor(kBlack);
    TH1F* h_fastADCFromNtuple_Ps = new TH1F("h_fastADCFromNtuple_Ps", "h_fastADCFromNtuple_Ps", 1024, -1024., 40960.*4.); h_fastADCFromNtuple_Ps->SetLineWidth(2); h_fastADCFromNtuple_Ps->SetLineColor(kBlack);
    TH1F* h_fastADCFromNtuple_Tc = new TH1F("h_fastADCFromNtuple_Tc", "h_fastADCFromNtuple_Tc", 1024, -1024., 40960.); h_fastADCFromNtuple_Tc->SetLineWidth(2); h_fastADCFromNtuple_Tc->SetLineColor(kBlack);

    TString ntupleName = "/Users/swkim/DRC/dual-readout_TB/test/ntuple_Run_" + runNum + "_Fast_FILE_total.root";
    // TString ntupleName = "/Users/swkim/DRC/dual-readout_TB/test/ntuple_Run_" + runNum + "_Fast_FILE_0.root";
    TFile* ntupleFile = new TFile(ntupleName);
    TTree* ntupleTree = (TTree*)ntupleFile->Get("events");
    int ntupleEntry = ntupleTree->GetEntries();

    TBevt<TBfastmode>* anEvtFromData = new TBevt<TBfastmode>();
    TBevt<TBfastmode>* anEvtFromNtuple = new TBevt<TBfastmode>();
    ntupleTree->SetBranchAddress("TBevt", &anEvtFromNtuple);

    TButility utility = TButility();
    utility.loading("mapping_data_MCPPMT_positiveSignal_v3.csv");
    utility.loadped("ped_400.csv");

    TBcid cidMu = utility.getcid(TBdetector::detid::muon);
    TBcid cidPs = utility.getcid(TBdetector::detid::preshower);
    TBcid cidTc = utility.getcid(TBdetector::detid::tail);

    std::vector<TBcid> cidM2TC;
    cidM2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 1, true));
    cidM2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 2, true));
    cidM2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 3, true));
    cidM2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 4, true));
    cidM2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 6, true));
    cidM2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 7, true));
    cidM2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 8, true));
    cidM2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 9, true));

    std::vector<TBcid> cidM2TS;
    cidM2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 1, false));
    cidM2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 2, false));
    cidM2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 3, false));
    cidM2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 4, false));
    cidM2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 6, false));
    cidM2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 7, false));
    cidM2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 8, false));
    cidM2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 9, false));

    unsigned channelsize = 32;

    std::vector< std::vector<std::string> > dataFileList(numOfFiles, std::vector<std::string>(numOfMID));
    for (int fileNum = 0; fileNum < numOfFiles; fileNum++) {
        for (int MID = 0; MID < numOfMID; MID++) {
            std::string fileName = (std::string)(dataBaseDir + "Run_" + runNum + "/Run_" + runNum + "_Fast/Run_" + runNum + "_Fast_MID_" + (MID+1) + "/Run_" + runNum + "_Fast_MID_" + (MID+1) + "_FILE_" + fileNum +".dat"); 
            // std::cout << "Reading file " << fileName << std::endl;
            dataFileList[fileNum][MID] = fileName;
        }        
    }

    std::vector< std::vector<FILE*> > fastFileList(numOfFiles, std::vector<FILE*>(numOfMID));
    for (int fileNum = 0; fileNum < numOfFiles; fileNum++) {
        for (int MID = 0; MID < numOfMID; MID++) {
            fastFileList[fileNum][MID] = fopen(dataFileList[fileNum][MID].c_str(), "rb");
        }
    }

    std::vector<int> entryPerMID(numOfMID);
    std::vector< std::vector<int> > entryPerFile( numOfFiles, std::vector<int>(numOfMID) );
    for (unsigned fileNum = 0; fileNum < numOfFiles; fileNum++) {
        for (unsigned MID = 0; MID < numOfMID; MID++) {
            FILE* fp = fopen(dataFileList[fileNum][MID].c_str(), "rb");
            fseek(fp, 0L, SEEK_END);
            int file_size = ftell(fp);
            fclose(fp);
            entryPerMID[MID] += (int)(file_size / 256);
            entryPerFile[fileNum][MID] = (int)(file_size / 256);
        }
    }
    int minTotalEventNum = *std::min_element(entryPerMID.begin(), entryPerMID.end());

    if (ntupleEntry < minTotalEventNum) minTotalEventNum = ntupleEntry;
    std::cout << "Total Entry : " << minTotalEventNum << std::endl;

    std::vector<int> currentOpenFileNum(numOfMID);
    std::vector<int> entryCounted(numOfMID);
    // Start event loop
    for (int iEvt = 0; iEvt < minTotalEventNum; iEvt++){
        // get Ntuple data
        ntupleTree->GetEntry(iEvt);

        // if count[MID] exceeds # of entry of corresponding file, move to next file
        for (unsigned MID = 0; MID < numOfMID; MID++) {
            // currently opened file number for each MID
            int fileNum = currentOpenFileNum[MID];
            if ( entryPerFile[fileNum][MID] == entryCounted[MID] ) {
                currentOpenFileNum[MID]++;
                entryCounted[MID] = 0;
            }
        }

        std::vector<TBmid<TBfastmode>> dataMIDs(numOfMID);
        for (unsigned MID = 0; MID < numOfMID; MID++) {
            int fileNum = currentOpenFileNum[MID];
            TBmid<TBfastmode> aMID = readFastmode(fastFileList[fileNum][MID]);
            dataMIDs[MID] = aMID;
        }
        anEvtFromData->setTCB(dataMIDs[0].evt());
        anEvtFromData->set(dataMIDs);

        // how to get M2T4 adc data
        // int fastADCFromData_C = anEvtFromData->data(cidM2TC.at(3)).adc();
        // int fastADCFromData_S = anEvtFromData->data(cidM2TS.at(3)).adc();
        // int fastADCFromNtuple_C = anEvtFromNtuple->data(cidM2TC.at(3)).adc();
        // int fastADCFromNtuple_S = anEvtFromNtuple->data(cidM2TS.at(3)).adc();
        
        // Can also use event data directly from TBmid<TBfastmode> by configuring MID & ch num
        // int fastADCFromData_C = dataMIDs[1].channel(14).adc();
        // int fastADCFromData_S = dataMIDs[1].channel(6).adc();

        // investigating M2T5 data
        h_fastADCFromData_C->Fill(anEvtFromData->data(cidM2TC.at(4)).adc());
        h_fastADCFromData_S->Fill(anEvtFromData->data(cidM2TS.at(4)).adc());
        h_fastADCFromNtuple_C->Fill(anEvtFromNtuple->data(cidM2TC.at(4)).adc());
        h_fastADCFromNtuple_S->Fill(anEvtFromNtuple->data(cidM2TS.at(4)).adc());

        h_fastADCFromData_Mu->Fill(anEvtFromData->data(cidMu).adc());
        h_fastADCFromNtuple_Mu->Fill(anEvtFromNtuple->data(cidMu).adc());

        h_fastADCFromData_Ps->Fill(anEvtFromData->data(cidPs).adc());
        h_fastADCFromNtuple_Ps->Fill(anEvtFromNtuple->data(cidPs).adc());

        h_fastADCFromData_Tc->Fill(anEvtFromData->data(cidTc).adc());
        h_fastADCFromNtuple_Tc->Fill(anEvtFromNtuple->data(cidTc).adc());

        // end of evt loop >> increase # of entry counted per file
        for (unsigned MID = 0; MID < numOfMID; MID++) entryCounted[MID]++;

        // print progress
        printProgress(iEvt, minTotalEventNum);
    }

    // closing files
    ntupleFile->Close();
    for (unsigned fileNum = 0; fileNum < numOfFiles; fileNum++) {
        for (unsigned MID = 0; MID < numOfMID; MID++) {
            fclose(fastFileList[fileNum][MID]);
        }
    }

    TCanvas* c = new TCanvas("","");
    c->cd(); h_fastADCFromData_C->Draw("Hist"); c->SaveAs("./validPlots/Data_C.png");
    c->cd(); h_fastADCFromData_S->Draw("Hist"); c->SaveAs("./validPlots/Data_S.png");
    c->cd(); h_fastADCFromNtuple_C->Draw("Hist"); c->SaveAs("./validPlots/Ntuple_C.png");
    c->cd(); h_fastADCFromNtuple_S->Draw("Hist"); c->SaveAs("./validPlots/Ntuple_S.png");

    c->cd(); h_fastADCFromData_Mu->Draw("Hist"); c->SaveAs("./validPlots/Data_Mu.png");
    c->cd(); h_fastADCFromData_Ps->Draw("Hist"); c->SaveAs("./validPlots/Data_Ps.png");
    c->cd(); h_fastADCFromData_Tc->Draw("Hist"); c->SaveAs("./validPlots/Data_Tc.png");

    c->cd(); h_fastADCFromNtuple_Mu->Draw("Hist"); c->SaveAs("./validPlots/Ntuple_Mu.png");
    c->cd(); h_fastADCFromNtuple_Ps->Draw("Hist"); c->SaveAs("./validPlots/Ntuple_Ps.png");
    c->cd(); h_fastADCFromNtuple_Tc->Draw("Hist"); c->SaveAs("./validPlots/Ntuple_Tc.png");

    TFile* histFile = new TFile("./validPlots/HistContainer.root", "RECREATE");
    histFile->WriteTObject(h_fastADCFromData_C);
    histFile->WriteTObject(h_fastADCFromData_S);
    histFile->WriteTObject(h_fastADCFromNtuple_C);
    histFile->WriteTObject(h_fastADCFromNtuple_S);
    
    histFile->WriteTObject(h_fastADCFromData_Mu);
    histFile->WriteTObject(h_fastADCFromData_Ps);
    histFile->WriteTObject(h_fastADCFromData_Tc);

    histFile->WriteTObject(h_fastADCFromNtuple_Mu);
    histFile->WriteTObject(h_fastADCFromNtuple_Ps);
    histFile->WriteTObject(h_fastADCFromNtuple_Tc);

    histFile->Close();

    return 0;
}
