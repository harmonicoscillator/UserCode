// gammaJet_PurityCalculation
// Author: Alex Barbieri
// Cobbled together from code written by Yongsun Kim
// Calculates and plots the photon purity using a sideband estimation of the background
// and a MC calculation of the signal. This info is used to do a template fit in the
// sigmaIetaIeta variable.

#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TH1D.h"
#include "TCut.h"
#include "TProfile.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TString.h"
#include "TLatex.h"
#include "stdio.h"
#include <iostream>
#include "commonUtility.h"

using namespace std;
//last forward run is 211256

TString data_file = "gammaJets_inclusive_dphi7pi8_data_v2.root";
TString mc_file = "gammaJets_inclusive_dphi7pi8_MC_v2.root";

class fitResult {
public:
  double nSig;
  double nSigErr;
  double purity010;
  double chisq;
};

class histFunction2
{ 
public:
  histFunction2(TH1D *h, TH1D *h2);
  ~histFunction2(){ 
    delete histBck;
    delete histSig;
  };
  
  //      int GetBinNumber(double c);
  Double_t evaluate(Double_t *x, Double_t *par);
  TH1D *histSig;
  TH1D *histBck;
  Double_t lowEdge;
  Double_t highEdge;
  Double_t nbin;
};

histFunction2::histFunction2(TH1D *h,TH1D *h2)
{
  histSig=(TH1D*)h->Clone();
  histBck=(TH1D*)h2->Clone();
   
  nbin=h->GetNbinsX();
  lowEdge=h->GetBinLowEdge(1);
  highEdge=h->GetBinLowEdge(nbin+1);
   
  histSig->SetName("hSig");
  histSig->Scale(1./histSig->Integral(1,histSig->GetNbinsX()+1));
  histBck->SetName("hBck");
  histBck->Scale(1./histBck->Integral(1,histBck->GetNbinsX()+1));
}

void mcStyle(TH1* h=0) {
  h->SetLineColor(kPink);
  h->SetFillColor(kOrange+7);
  h->SetFillStyle(3004);
}

void sbStyle(TH1* h=0) {
  h->SetLineColor(kGreen+4);
  h->SetFillColor(kGreen+1);
  h->SetFillStyle(3001);
}

Double_t histFunction2::evaluate(Double_t *x, Double_t *par) {
     
  Double_t xx = x[0];
  int binNum=histSig->FindBin(xx);  //
   
  return par[0]*(histSig->GetBinContent(binNum)*par[1]+histBck->GetBinContent(binNum)*(1-par[1]));
}

const Double_t hfBins[] = {0, 20, 30, 1000}; //last entry is upper bound on last bin
const Int_t nhfBins = 3;

fitResult getPurity(TCut evtSeltCut, TCut sbEvtCut, TString canvasName = "results.gif", int minpt = 0, int maxpt = 80000000);

fitResult doFit(TH1D* hSig=0, TH1D* hBkg=0, TH1D* hData1=0, float varLow=0.001, float varHigh=0.028, bool drawLeg=true);

//int returnHFBin(double hf);

void gammaJet_PurityCalculation()
{
  TH1::SetDefaultSumw2();

 
  ///////// Photon cut //////////////////////////////////////////////////////////////////////////////
  
  TCut ptPhoCut  = "(gPt > 50)";
  TCut etaCut = "(abs(gEta) < 1.479)";
  //TCut runCut = "(run < 211257)";
  TCut caloIso = "(cc4+cr4+ct4PtCut20<1) && hadronicOverEm<0.1";
  TCut sbIso   = "(cc4+cr4+ct4PtCut20>10) && (cc4+cr4+ct4PtCut20<20) && hadronicOverEm<0.1";
  TCut basicPhoCut = etaCut && ptPhoCut && caloIso;
  TCut sbPhoCut    = etaCut && ptPhoCut && sbIso   ;
  TCut evtSeltCut = basicPhoCut;
  TCut sbSeltCut  = sbPhoCut;

  //TCut phoCandCut   = "sigmaIetaIeta<0.010";
  //TCut phoDecayCut  = "(sigmaIetaIeta>0.011) && (sigmaIetaIeta<0.017)";

  TString canvasName = "purityResults_empty.gif";
  
  // get purity with the current jet cut ! 
  float purity(0);
  
  fitResult fitr = getPurity(evtSeltCut, sbSeltCut, canvasName);
  purity = fitr.purity010;
}

fitResult getPurity(TCut evtSeltCut, TCut sbEvtCut, TString canvasName, int minpt, int maxpt) {
  double purity(0);
  
  TFile *dataFile = TFile::Open(data_file);
  TNtuple *dataTuple = (TNtuple*)dataFile->Get("gammaJets");

  TFile *mcFile = TFile::Open(mc_file);
  TNtuple *mcTuple = (TNtuple*)mcFile->Get("gammaJets");

  TH1D* hCand = new TH1D("cand","",35,0,0.035);
  TH1D* hBkg = (TH1D*)hCand->Clone("bkg");  TH1D* hSig = (TH1D*)hCand->Clone("sig");

  dataTuple->Project(   hCand->GetName(), "sigmaIetaIeta", evtSeltCut && "(run < 211257)", "");
  dataTuple->Project(   hBkg->GetName(), "sigmaIetaIeta", sbEvtCut && "(run < 211257)" , "");
  mcTuple->Project( hSig->GetName(), "sigmaIetaIeta", evtSeltCut && "genCalIsoDR04<5 && abs(genMomId)<=22", "");

  TCanvas* cPurity = new TCanvas("cpurity","",500,500);
  fitResult fitr = doFit ( hSig, hBkg, hCand, 0.005, 0.035);
  //if(minpt !=0 && maxpt != 0)
  //  drawText( Form("%d < photon E_{T} < %d", minpt,maxpt), 0.568,0.8);
  //drawText(Form("Purity : %.2f", (float)fitr.purity010), 0.5680963,0.529118);
  drawText("Barrel",0.5680963,0.529118);
  //cPurity->SaveAs(canvasName);

  delete dataTuple;
  delete mcTuple;
  delete hSig;
  delete hBkg;
  delete hCand;
  
  return fitr;
  
  
}


fitResult doFit(TH1D* hSig, TH1D* hBkg, TH1D* hData1, float varLow, float varHigh, bool drawLeg) {
   
  TH1D* hDatatmp = (TH1D*)hData1->Clone(Form("%s_datatmp",hData1->GetName()));
  int nBins = hDatatmp->GetNbinsX();
  histFunction2 *myFits = new histFunction2(hSig,hBkg);
  TF1 *f = new TF1("f",myFits,&histFunction2::evaluate,varLow,varHigh,2);
  f->SetParameters( hDatatmp->Integral(1,nBins+1), 0.3);
  f->SetParLimits(1,0,1);
  hDatatmp->Fit("f","LL M O Q","",varLow,varHigh);
  hDatatmp->Fit("f","LL M O Q","",varLow,varHigh);

  fitResult res;
  res.nSig =0;
  double nev = f->GetParameter(0);
  double ratio = f->GetParameter(1);
  double ratioErr = f->GetParError(1);
  res.nSig    = nev * ratio;
  res.nSigErr = nev * ratioErr;
  res.chisq = (double)f->GetChisquare()/ f->GetNDF();
   
  TH1F *hSigPdf = (TH1F*)hSig->Clone(Form("%s_tmp",hSig->GetName()));
  hSigPdf->Scale(res.nSig/hSigPdf->Integral(1,nBins+1));

  TH1F *hBckPdf = (TH1F*)hBkg->Clone(Form("%s_tmp",hBkg->GetName()));
  hBckPdf->Scale((nev-res.nSig)/hBckPdf->Integral(1,nBins+1));

  double ss1 = hSigPdf->Integral(1, hSigPdf->FindBin(0.00999),"width");
  double bb1 = hBckPdf->Integral(1, hBckPdf->FindBin(0.00999),"width");
  //   cout <<"  hte bin = " <<hSigPdf->FindBin(0.00999) << endl;
  res.purity010 = ss1/(ss1+bb1);
  cout << "purity = " << res.purity010 << endl;
  hSigPdf->Add(hBckPdf);
  handsomeTH1(hSigPdf);
  mcStyle(hSigPdf);
  sbStyle(hBckPdf);
  cleverRange(hSigPdf,1.5);
  hSigPdf->SetNdivisions(510);

  hSigPdf->SetYTitle("Entries");
  hSigPdf->SetXTitle("#sigma_{#eta #eta}");
  hSigPdf->DrawCopy("hist");
  hBckPdf->DrawCopy("same hist");
  hData1->DrawCopy("same");
  TH1D* temphSigPdf = (TH1D*)hSigPdf->Clone("temp1");
  TH1D* temphBckPdf = (TH1D*)hBckPdf->Clone("temp2");
  if(drawLeg){
    TLegend *t3=new TLegend(0.5402006,0.5963235,0.9186019,0.7853466,NULL,"brNDC");
    //ridiculous
    //t3->AddEntry(hData1,"Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV","pl");
    //t3->AddEntry(hData1,"pp  #sqrt{s}_{_{NN}}=2.76 TeV","pl");
    t3->AddEntry(hData1,"pPb  #sqrt{s}_{_{NN}}=5.02 TeV","pl");
    t3->AddEntry(temphSigPdf,"Signal","lf");
    t3->AddEntry(temphBckPdf,"Background","lf");
    t3->SetFillColor(0);
    t3->SetBorderSize(0);
    t3->SetFillStyle(0);
    t3->SetTextFont(63);
    t3->SetTextSize(15);
    t3->Draw();
    //drawCMS2011(0.53,0.9,150,16);
  }
   

  //   delete hSigPdf;
  //   delete hBckPdf;
   
//   TFile* hout = new TFile("histo.root","recreate");
//   hSigPdf->Write();
//   hBckPdf->Write();
//   hData1->Write();
//   hout->Close();

   
  return res;

}

