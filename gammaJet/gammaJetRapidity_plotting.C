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


const Double_t hfBins[] = {0, 20, 30, 1000}; //last entry is upper bound on last bin
const Int_t nhfBins = 3;

//int returnHFBin(double hf);

void gammaJetRapidity_plotting()
{
  TH1::SetDefaultSumw2();

  TFile *inFile = TFile::Open("gammaJets_inclusive_dphi7pi8_data_v2.root");
  // gPt:gEta:gPhi:jPt:jEta:jPhi:HF:HFeta4:avgEta:dPhi:cc4:cr4:ct4PtCut20:hadronicOverEm:sigmaIetaIeta:run:r9
  TNtuple *inTuple = (TNtuple*)inFile->Get("gammaJets");

  //book histos
  TH1D *gammaJetEta[nhfBins];
  TH1D *x_jg[nhfBins];
  TH1D *hfEnergy[nhfBins];
  Int_t numEvts[nhfBins];
  Int_t numEvtsEtaPlus[nhfBins];

  TString labels[] = {"0 < E_{T}^{HF[|#eta|>4]} < 20","20 < E_{T}^{HF[|#eta|>4]} < 30","30 < E_{T}^{HF[|#eta|>4]}"};
  
  TCanvas *c[3];
  c[0] = new TCanvas();
  c[2] = new TCanvas();
  TLegend *leg = new TLegend(0.65,0.65,0.9,0.9);
  leg->SetFillColor(0);
  
  for(int i = 0; i < nhfBins; i++)
  {
    TString name = "gammaJetEta";
    name += i;
    gammaJetEta[i] = new TH1D(name,";Avg #eta_{#gamma jet};Event Fraction",14,-3,3);

    TString name2 = "hfEnergy";
    name2 += i;
    hfEnergy[i] = new TH1D(name2,"",1000,0,1000);

    TString name3 = "x_jg";
    name3 += i;
    x_jg[i] = new TH1D(name3,";p_{T}^{jet}/p_{T}^{#gamma};Event Fraction",20,0,1.5);

    TString cut;
    cut += "(run < 211257) && ";
    cut += "(cc4 + cr4 < 1.0)";
    cut += "&& ((abs(gEta) > 1.479 && sigmaIetaIeta < 0.035)) ";// || (abs(gEta) > 1.479 && sigmaIetaIeta < 0.035)) ";
    cut += "&& (jPt > 30) && (gPt > 50) && (HFeta4 > ";
    cut += hfBins[i];
    cut += " ) && ( HFeta4 < ";
    cut += hfBins[i+1];
    cut += " )";
    numEvts[i] = inTuple->Project(name,"avgEta",cut);
    numEvtsEtaPlus[i] = inTuple->GetEntries(cut+"&& ( avgEta > 0 )");
    inTuple->Project(name2,"HFeta4",cut);
    inTuple->Project(name3,"jPt/gPt",cut);

    gammaJetEta[i]->Scale(1./numEvts[i]);
    gammaJetEta[i]->SetMarkerColor(i+1);
    gammaJetEta[i]->SetLineColor(i+1);

    x_jg[i]->Scale(1./numEvts[i]);
    x_jg[i]->SetMarkerColor(i+1);
    x_jg[i]->SetLineColor(i+1);

    
    leg->AddEntry(gammaJetEta[i], labels[i], "lp");
    
    if(i==0)
    {
      TLatex *lnorm = new TLatex(0.2,0.85, "Monte Carlo");
      lnorm->SetNDC(1);
      lnorm->SetTextSize(0.05);
     
      gammaJetEta[i]->GetXaxis()->CenterTitle();
      gammaJetEta[i]->GetYaxis()->CenterTitle();
      gammaJetEta[i]->SetMaximum(gammaJetEta[i]->GetMaximum()*1.7);
      x_jg[i]->GetXaxis()->CenterTitle();
      x_jg[i]->GetYaxis()->CenterTitle();
      x_jg[i]->SetMaximum(x_jg[i]->GetMaximum()*1.7);

      c[0]->cd();
      gammaJetEta[i]->Draw("");
      lnorm->Draw("same");
      c[2]->cd();
      x_jg[i]->Draw("");
      //lnorm->Draw("same");
    }
    else
    {
      c[0]->cd();
      gammaJetEta[i]->Draw("same");
      c[2]->cd();
      x_jg[i]->Draw("same");
    }

    c[0]->cd();
    gammaJetEta[i]->Draw("same Lhist");
    leg->Draw();
    c[2]->cd();
    x_jg[i]->Draw("same Lhist");
    leg->Draw();
  }

  //c[0]->SaveAs("avgeta_gammajet_sumiso1_numevts_v2.gif");

  c[1] = new TCanvas();
  //";E_{T}^{HF[|#eta|>4]};<#eta_{#gamma jet}>");
  Double_t x[nhfBins];// = {10, 25, 40};
  Double_t y[nhfBins];
  Double_t ye[nhfBins];
  for(int i = 0; i < nhfBins; i++)
  {
    x[i] = hfEnergy[i]->GetMean();
    y[i] = gammaJetEta[i]->GetMean();
    ye[i]= gammaJetEta[i]->GetMeanError();
  }
  TGraphErrors *means = new TGraphErrors(nhfBins,x,y,0,ye);
  means->GetXaxis()->SetTitle("E_{T}^{HF[|#eta|>4]}");
  means->GetXaxis()->CenterTitle();
  means->GetYaxis()->SetTitle("<#eta_{#gamma jet}>");
  means->GetYaxis()->CenterTitle();
  means->SetMinimum(-0.7);
  means->SetMaximum(0);
  means->Draw("AP");
}


