#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TString.h"
#include "TLatex.h"
#include "stdio.h"
#include "../HiForest_V3/hiForest.h"

//last forward run is 211256


//const Double_t hfBins[] = {0, 20, 30, 1000}; //last entry is upper bound on last bin
//const Int_t nhfBins = 3;

//int returnHFBin(double hf);

void gammaJetRapidity_mcComp()
{
  TH1::SetDefaultSumw2();

  TNtuple *inTuple[2];
  TFile *inFileData = TFile::Open("gammaJets_inclusive_dphi7pi8_data_v2.root");
  inTuple[0] = (TNtuple*)inFileData->Get("gammaJets");
  TFile *inFileMC = TFile::Open("gammaJets_inclusive_dphi7pi8_MC_v2.root");
  inTuple[1] = (TNtuple*)inFileMC->Get("gammaJets");

  // gPt:gEta:gPhi:jPt:jEta:jPhi:HF:HFeta4:avgEta:dPhi:cc4:cr4:ct4PtCut20:hadronicOverEm:sigmaIetaIeta:run:r9
  const Int_t nVars = 16;

  //book histos
  TH1D *hist[nVars][2];
  TCanvas *c[nVars];

  const TString name[nVars] = {"gPt","gEta","gPhi","jPt","jEta","jPhi","HF","HFeta4","avgEta","dPhi","cc4","cr4","ct4PtCut20","hadronicOverEm","sigmaIetaIeta","r9"};

  //const TString analysisCut = "(cc4 + cr4 < 1.0) && ((abs(gEta) < 1.44 && sigmaIetaIeta < 0.01) || (abs(gEta) > 1.44 && sigmaIetaIeta < 0.035)) && (jPt > 30) && (gPt > 50)";

  //barrel
  const TString analysisCut = "(cc4 + cr4 < 1.0) && (jPt > 30) && (gPt > 50) && (gEta > 1.479) && (sigmaIetaIeta < 0.035)";

  //endcap
  //const TString analysisCut = "(cc4 + cr4 < 1.0) && (jPt > 30) && (gPt > 50) && (abs(gEta) > 1.44) && (sigmaIetaIeta < 0.035)";

  const Int_t nBins = 50;

  const Double_t minVal[nVars] = {0, -3.5, -TMath::Pi(), 0, -3.5, -TMath::Pi(), 0, 0, -3.5, 7.*TMath::Pi()/8., -10, -10, -10, 0, 0, 0};

  const Double_t maxVal[nVars] = {150, 3.5, TMath::Pi(), 200, 3.5, TMath::Pi(), 100, 40, 3.5, TMath::Pi(), 20, 20, 20, 0.5, 0.06, 1.5};			       
  
  for(int i = 0; i < nVars; i++)
  {
    hist[i][0] = new TH1D(name[i],name[i]+";"+name[i],nBins, minVal[i], maxVal[i]);
    hist[i][1] = (TH1D*)hist[i][0]->Clone(name[i]+"mc");

    inTuple[0]->Project(name[i],name[i],analysisCut+"&&(run<211257)");
    inTuple[1]->Project(name[i]+"mc",name[i],analysisCut);
    
    //inTuple[0]->Project(name[i],name[i],"(run<211257)");
    //inTuple[1]->Project(name[i]+"mc",name[i]);

    hist[i][0]->Scale(1./hist[i][0]->GetEntries());
    hist[i][1]->Scale(1./hist[i][1]->GetEntries());

    c[i] = new TCanvas();
    
    hist[i][1]->SetLineColor(kRed);
    hist[i][0]->SetMinimum(0);
    hist[i][0]->SetMaximum(hist[i][0]->GetMaximum()*2.0);

    hist[i][0]->Draw("p");
    hist[i][1]->Draw("same hist");

    TLatex *lnorm = new TLatex(0.25,0.85, "Barrel");
    lnorm->SetNDC(1);
    lnorm->SetTextSize(0.05);
    lnorm->Draw("same");
    

    TLegend *leg = new TLegend(0.7,0.7,0.9,0.9);
    leg->SetFillColor(0);
    leg->AddEntry(hist[i][0],"Data","p");
    leg->AddEntry(hist[i][1],"MC","l");
    leg->Draw();
    
    TString savename = "MCcomp_barrel_";
    savename += i;
    savename += ".gif";
    //c[i]->SaveAs(savename);

  }

}


