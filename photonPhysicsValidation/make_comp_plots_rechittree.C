#include "TChain.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"
#include "TMath.h"

void make_comp_plots_rechittree(const bool save=false,
				const bool doPilot=true,
				const bool doMC = false,
				const bool doData = false,
				const char *dataName = "",
				const TString section = "eb")
{
  TH1::SetDefaultSumw2();

  // const bool doPilot = true;
  // const bool doMC = true;
  // const bool doData = false;

  // const bool save = false;
  
  // const TString section = "eb";

  TString cut = "";//"(e>15)";

  TChain *mcChain = new TChain("rechitanalyzer","mcChain");
  if (doMC){
    const TString mcName = "/mnt/hadoop/cms/store/user/dgulhan/pPb/Pythia_5_3_3_v0/HiForest2_v01/merged_3/*.root/rechitanalyzer/" + section;
    mcChain->Add(mcName);
  }

  TFile *pilotData;
  TTree *pilotPhotonTree;
  if(doPilot){    
    pilotData = new TFile("/mnt/hadoop/cms/store/user/luck/pA2013_pilot/forest200kHz.root");
    //TFile *pilotData = new TFile("/mnt/hadoop/cms/store/user/luck/pA2013_pilot/pPb_hiForest2_11_1_zso.root");  
    pilotPhotonTree = (TTree*)pilotData->Get("rechitanalyzer/" + section);
  }

  if(dataName == "") dataName = "/mnt/hadoop/cms/store/user/luck/pA2013_pilot/pPb_hiForest2_10_1_ylL.root";

  TFile *data;
  TTree *dataPhotonTree;
  if(doData){
    data = new TFile(dataName);
    dataPhotonTree = (TTree*)data->Get("rechitanalyzer/" + section);
  }

  // pilotPhotonTree->Draw("pt","","E");
  // dataPhotonTree->Draw("pt","","E same");

  const Int_t numPlots = 4;
  TH1D *h[numPlots][3];
  TH2D *etaphi[3];
  //TString name[numPlots];
  TString name;
  TCanvas *c2[3];

  for(int i = 0; i<3; i++)
  {
    int marker;
    TTree *tree;
    char ii;
    TString Yname = "counts per event";
    TString label = "";
    int markerColor;
    if(i == 0)
    {
      if(!doPilot) continue;
      tree = pilotPhotonTree;
      marker = 24;
      markerColor = (int)kRed;
      ii='0';
    } else if (i==1) {
      if(!doData) continue;
      tree = dataPhotonTree;
      marker = 20;
      markerColor = (int)kBlack;
      ii='1';
    } else {
      if(!doMC) continue;
      tree = mcChain;
      marker = 25;
      markerColor = (int)kBlue;
      ii='2';
    }

    (section == "eb") ? label="Barrel" : label = "Endcap";

    name = "e";
    h[0][i] = new TH1D(name+ii,"",
		       50,0,100);
    h[0][i]->SetMarkerStyle(marker);
    h[0][i]->SetMarkerColor(markerColor);
    h[0][i]->SetXTitle("e " + label );
    h[0][i]->SetYTitle(Yname);
    tree->Project(name+ii,name,cut);
    h[0][i]->Scale(1./h[0][i]->GetEntries());

    name = "et";
    h[1][i] = new TH1D(name+ii,"",
		       50,0,100);
    h[1][i]->SetMarkerStyle(marker);
    h[1][i]->SetMarkerColor(markerColor);
    h[1][i]->SetXTitle("et (GeV) " + label );
    h[1][i]->SetYTitle(Yname);
    tree->Project(name+ii,name,cut);
    h[1][i]->Scale(1./h[1][i]->GetEntries());

    name = "eta";
    h[2][i] = new TH1D(name+ii,"",
		       50,-5,5);
    h[2][i]->SetMarkerStyle(marker);
    h[2][i]->SetMarkerColor(markerColor);
    h[2][i]->SetXTitle("#eta " + label );
    h[2][i]->SetYTitle(Yname);
    tree->Project(name+ii,name,cut);
    h[2][i]->Scale(1./h[2][i]->GetEntries());

    name = "phi";
    h[3][i] = new TH1D(name+ii,"",
		       18,-TMath::Pi(),TMath::Pi() );
    h[3][i]->SetMarkerStyle(marker);
    h[3][i]->SetMarkerColor(markerColor);
    h[3][i]->SetXTitle("#phi " + label );
    h[3][i]->SetYTitle(Yname);
    tree->Project(name+ii,name,cut);
    h[3][i]->Scale(1./h[3][i]->GetEntries());
    h[3][i]->SetAxisRange(0, 0.1, "Y");

    TString canvName = "c2etaphi"+section;
    canvName += i;
    c2[i] = new TCanvas(canvName,"",500,500);
    c2[i]->cd();
    name = "etaphi";
    etaphi[i] = new TH2D(name+ii,"",
			 40,-5,5,
			 36, -TMath::Pi(), TMath::Pi() );    
    etaphi[i]->SetXTitle("#eta");
    etaphi[i]->SetYTitle("#phi");
    tree->Project(name+ii,"phi:eta",cut);
    etaphi[i]->Draw("colz");
    if(save)
    {
      TString savename = "plot_etaphi_rechit_"+section;
      savename += i;
      savename += ".eps";
      c2[i]->SaveAs(savename);
    }

  }

  TString dataLabel = "data";
  TString pilotLabel = "pilot";
  TString MCLabel = "epos MC";
  TLegend *leg;
  TCanvas *c[numPlots];

  for(int i = 0; i < numPlots; i++)
  {
    TString canvName = "crechit"+section;
    canvName+=i;
    c[i] = new TCanvas(canvName,"",500,500);
    c[i]->cd();
    leg = new TLegend(0.8,0.6,0.9,0.7);
    leg->SetFillColor(0);
    if(doData){
      leg->AddEntry(h[i][1],dataLabel,"p");
      h[i][1]->DrawClone("E");
    }
    if(doPilot){
      leg->AddEntry(h[i][0],pilotLabel,"p");
      h[i][0]->DrawClone("E same");
    }
    if(doMC){
      leg->AddEntry(h[i][2],MCLabel,"p");
      h[i][2]->DrawClone("E same");
    }
    leg->DrawClone();

    if(save)
    {
      TString savename = "plot_rechit_" + section;
      savename += i;
      savename += ".eps";
      c[i]->SaveAs(savename);
    }
  }

  //2D plots
  

  
}
