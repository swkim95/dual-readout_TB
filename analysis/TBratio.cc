#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "TROOT.h"
#include "TStyle.h"
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TH1F.h>
#include <TCanvas.h>

void drawRatio(TH1F* num, TH1F* den, TString title, TString output){
    TH1F* h_fromNtuple = num;
    TH1F* h_fromData = den;
    TH1F* h_copy = (TH1F*)num->Clone("h_copy");

    TCanvas* c1 = new TCanvas("c", "c", 1000, 800);
    c1->cd();

    TPad* p1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);

    p1->SetGridx();
    p1->SetBottomMargin(0);
    p1->Draw();
    p1->cd();

    h_fromData->SetTitle(title);
    h_fromData->SetLineColor(kBlue);
    h_fromData->GetXaxis()->SetLabelSize(0);
    h_fromData->GetYaxis()->SetTitle("Evt");
    h_fromData->GetYaxis()->SetTitleOffset(1.5);
    h_fromData->GetYaxis()->SetTitleSize(0.035);
    h_fromData->GetYaxis()->ChangeLabel(1, -1, -1, -1, -1, -1, " ");
    h_fromData->Draw("Hist");
    p1->Update();
    TPaveStats* stat_Data_C = (TPaveStats*)p1->GetPrimitive("stats");
    stat_Data_C->SetName("Data_C");
    stat_Data_C->SetTextColor(kBlue);
    stat_Data_C->SetY1NDC(0.6); stat_Data_C->SetY2NDC(0.75);
    stat_Data_C->SetX1NDC(0.7); stat_Data_C->SetX2NDC(0.9);

    h_fromNtuple->SetTitle("");
    h_fromNtuple->SetMarkerStyle(21);
    h_fromNtuple->SetMarkerSize(0.6);
    h_fromNtuple->SetMarkerColor(kRed);
    h_fromNtuple->Draw("sames&p");
    p1->Update();
    TPaveStats* stat_Ntuple_C = (TPaveStats*)p1->GetPrimitive("stats");
    stat_Ntuple_C->SetName("Ntuple_C");
    stat_Ntuple_C->SetTextColor(kRed);
    stat_Ntuple_C->SetY1NDC(0.75); stat_Ntuple_C->SetY2NDC(.9);
    stat_Ntuple_C->SetX1NDC(0.7); stat_Ntuple_C->SetX2NDC(0.9);

    c1->cd();
    TPad* p2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    p2->SetGridx();
    p2->SetGridy();
    p2->SetTopMargin(0);
    p2->SetBottomMargin(0);
    p2->SetBorderSize(0);
    p2->SetBorderMode(0);
    p2->Draw();
    p2->cd();

    h_copy->SetMarkerColor(kBlack);
    h_copy->SetMarkerStyle(21);
    h_copy->SetMarkerSize(0.5);
    h_copy->Sumw2();
    h_copy->SetTitle("");
    h_copy->SetLineWidth(0);
    h_copy->Divide(h_fromData);

    h_copy->GetYaxis()->SetNdivisions(505);
    h_copy->GetYaxis()->ChangeLabel(-1, -1, -1, -1, -1, -1, " ");
    h_copy->GetYaxis()->SetTitle("Ntuple/Data");
    h_copy->GetYaxis()->SetTitleSize(0.08);
    h_copy->GetYaxis()->SetTitleOffset(0.5);
    h_copy->GetYaxis()->SetLabelSize(0.085);

    h_copy->GetXaxis()->SetTitle("Int ADC");
    h_copy->GetXaxis()->SetTitleSize(0.09);
    h_copy->GetXaxis()->SetTitleOffset(0.9);
    h_copy->GetXaxis()->SetLabelSize(0.08);

    
    h_copy->SetStats(0);
    h_copy->SetMinimum(0.99);
    h_copy->SetMaximum(1.01);

    h_copy->Draw("p");    

    p2->Update();

    gPad->SetBottomMargin(0.2);

    c1->Draw();
    c1->SaveAs(output);

    delete p1;
    delete p2;
    delete c1;
}

void TBratio(TString runNum) {

    TFile* histFile = new TFile("./validPlots/HistContainer.root");

    TH1F* h_fastADCFromData_C = (TH1F*)histFile->Get("h_fastADCFromData_C");
    TH1F* h_fastADCFromNtuple_C = (TH1F*)histFile->Get("h_fastADCFromNtuple_C");

    TH1F* h_fastADCFromData_S = (TH1F*)histFile->Get("h_fastADCFromData_S");
    TH1F* h_fastADCFromNtuple_S = (TH1F*)histFile->Get("h_fastADCFromNtuple_S");

    TH1F* h_fastADCFromData_Mu = (TH1F*)histFile->Get("h_fastADCFromData_Mu");
    TH1F* h_fastADCFromNtuple_Mu = (TH1F*)histFile->Get("h_fastADCFromNtuple_Mu");

    TH1F* h_fastADCFromData_Ps = (TH1F*)histFile->Get("h_fastADCFromData_Ps");
    TH1F* h_fastADCFromNtuple_Ps = (TH1F*)histFile->Get("h_fastADCFromNtuple_Ps");

    TH1F* h_fastADCFromData_Tc = (TH1F*)histFile->Get("h_fastADCFromData_Tc");
    TH1F* h_fastADCFromNtuple_Tc = (TH1F*)histFile->Get("h_fastADCFromNtuple_Tc");

    drawRatio(h_fastADCFromNtuple_C, h_fastADCFromData_C, "Run " + runNum + " M2T5 C ch. Ratio of data & ntuple", "./ratioPlots/Run_" + runNum + "_M2T5C.png");
    drawRatio(h_fastADCFromNtuple_S, h_fastADCFromData_S, "Run " + runNum + " M2T5 S ch. Ratio of data & ntuple", "./ratioPlots/Run_" + runNum + "_M2T5S.png");
    drawRatio(h_fastADCFromNtuple_Mu, h_fastADCFromData_Mu, "Run " + runNum + " MuonCounter Ratio of data & ntuple", "./ratioPlots/Run_" + runNum + "_MuonCounter.png");
    drawRatio(h_fastADCFromNtuple_Ps, h_fastADCFromData_Ps, "Run " + runNum + " PreShower Ratio of data & ntuple", "./ratioPlots/Run_" + runNum + "_PreShower.png");
    drawRatio(h_fastADCFromNtuple_Tc, h_fastADCFromData_Tc, "Run " + runNum + " TailCatcher Ratio of data & ntuple", "./ratioPlots/Run_" + runNum + "_TailCatcher.png");

    // TCanvas* c1 = new TCanvas("c", "c", 1000, 800);
    // c1->cd();

    // TPad* p1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);

    // p1->SetGridx();
    // p1->SetBottomMargin(0);
    // p1->Draw();
    // p1->cd();

    // h_fastADCFromData_C->SetTitle("M2T5 C ch. Ratio of data & ntuple");
    // h_fastADCFromData_C->GetXaxis()->SetLabelSize(0);
    // h_fastADCFromData_C->GetYaxis()->SetTitle("Evt");
    // h_fastADCFromData_C->GetYaxis()->ChangeLabel(1, -1, -1, -1, -1, -1, " ");
    // h_fastADCFromData_C->Draw("Hist");
    // p1->Update();
    // TPaveStats* stat_Data_C = (TPaveStats*)p1->GetPrimitive("stats");
    // stat_Data_C->SetName("Data_C");
    // stat_Data_C->SetTextColor(kBlue);
    // stat_Data_C->SetY1NDC(0.6); stat_Data_C->SetY2NDC(0.75);
    // stat_Data_C->SetX1NDC(0.7); stat_Data_C->SetX2NDC(0.9);

    // h_fastADCFromNtuple_C->SetTitle("");
    // h_fastADCFromNtuple_C->SetMarkerStyle(21);
    // h_fastADCFromNtuple_C->SetMarkerSize(0.6);
    // h_fastADCFromNtuple_C->SetMarkerColor(kRed);
    // h_fastADCFromNtuple_C->Draw("sames&ep");
    // p1->Update();
    // TPaveStats* stat_Ntuple_C = (TPaveStats*)p1->GetPrimitive("stats");
    // stat_Ntuple_C->SetName("Ntuple_C");
    // stat_Ntuple_C->SetTextColor(kRed);
    // stat_Ntuple_C->SetY1NDC(0.75); stat_Ntuple_C->SetY2NDC(.9);
    // stat_Ntuple_C->SetX1NDC(0.7); stat_Ntuple_C->SetX2NDC(0.9);

    // c1->cd();
    // TPad* p2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    // p2->SetGridx();
    // p2->SetGridy();
    // p2->SetTopMargin(0);
    // p2->SetBottomMargin(0);
    // p2->SetBorderSize(0);
    // p2->SetBorderMode(0);
    // p2->Draw();
    // p2->cd();

    // // TH1F* h_copy = (TH1F*)h_fastADCFromNtuple_C->Clone("h_copy");
    // h_copy->SetMarkerColor(kBlack);
    // h_copy->SetMarkerStyle(21);
    // h_copy->SetMarkerSize(0.5);
    // h_copy->Sumw2();
    // h_copy->SetTitle("");
    // h_copy->SetLineWidth(0);
    // h_copy->Divide(h_fastADCFromData_C);

    // h_copy->GetYaxis()->SetNdivisions(505);
    // h_copy->GetYaxis()->ChangeLabel(-1, -1, -1, -1, -1, -1, " ");
    // h_copy->GetYaxis()->SetTitle("Ntuple/Data");
    // h_copy->GetYaxis()->SetTitleSize(0.08);
    // h_copy->GetYaxis()->SetTitleOffset(0.5);
    // h_copy->GetYaxis()->SetLabelSize(0.08);

    // h_copy->GetXaxis()->SetTitle("Int ADC");
    // h_copy->GetXaxis()->SetTitleSize(0.09);
    // h_copy->GetXaxis()->SetTitleOffset(0.9);
    // h_copy->GetXaxis()->SetLabelSize(0.08);

    
    // h_copy->SetStats(0);
    // h_copy->SetMinimum(0.99);
    // h_copy->SetMaximum(1.01);

    // h_copy->Draw("p");    

    // p2->Update();

    // gPad->SetBottomMargin(0.2);

    // c1->Draw();
    // c1->SaveAs("test.png");
}