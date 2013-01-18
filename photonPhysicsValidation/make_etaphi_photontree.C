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

void make_etaphi_photontree(const bool save=false,
			    const bool doPilot=true,
			    const bool doMC = false,
			    const bool doData = false,
			    const char *dataName = "")
{
  TH1::SetDefaultSumw2();

  // const bool doPilot = true;
  // const bool doMC = false;
  // const bool doData = false;

  // const bool save = false;

  TChain *mcChain = new TChain("multiPhotonAnalyzer","pilotChain");
  if (doMC){
    const TString mcName = "/mnt/hadoop/cms/store/user/dgulhan/pPb/Pythia_5_3_3_v0/HiForest2_v01/merged_3/*.root/multiPhotonAnalyzer/photon";
    mcChain->Add(mcName);
  }

  TFile *pilotData;
  TTree *pilotPhotonTree;
  if(doPilot){    
    pilotData = new TFile("/mnt/hadoop/cms/store/user/luck/pA2013_pilot/forest200kHz.root");
    //TFile *pilotData = new TFile("/mnt/hadoop/cms/store/user/luck/pA2013_pilot/pPb_hiForest2_11_1_zso.root");  
    pilotPhotonTree = (TTree*)pilotData->Get("multiPhotonAnalyzer/photon");
  }

  if(dataName == "") dataName = "/mnt/hadoop/cms/store/user/luck/pA2013_pilot/pPb_hiForest2_10_1_ylL.root";

  TFile *data;
  TTree *dataPhotonTree;
  if(doData){
    data = new TFile(dataName);
    dataPhotonTree = (TTree*)data->Get("multiPhotonAnalyzer/photon");
  }

  TString name;

  TH2D *etaphi[3];
  TH2D *swisscross[3];
  TCanvas *c[3];
  TCanvas *c2[3];
  
  for(int i = 0; i<3; i++)
  {
    TTree *tree;
    char ii;
    if(i == 0)
    {
      if(!doPilot) continue;
      tree = pilotPhotonTree;
      ii='0';
    } else if (i==1) {
      if(!doData) continue;
      tree = dataPhotonTree;
      ii='1';
    } else {
      if(!doMC) continue;
      tree = mcChain;
      ii='2';
    }

    TString canvName = "cetaphi";
    canvName += i;
    c[i] = new TCanvas(canvName,"",500,500);
    c[i]->cd();
    name = "etaphi";
    etaphi[i] = new TH2D(name+ii,"",
			 40,-5,5,
			 36, -TMath::Pi(), TMath::Pi() );    
    etaphi[i]->SetXTitle("#eta");
    etaphi[i]->SetYTitle("#phi");
    tree->Project(name+ii,"phi:eta");
    etaphi[i]->Draw("colz");
    if(save)
    {
      TString savename = "plot_photon_etaphi_";
      savename += i;
      savename += ".eps";
      c[i]->SaveAs(savename);
    }

    canvName+=2;
    c2[i] = new TCanvas(canvName,"",500,500);
    c2[i]->cd();
    name = "swissCrx";
    swisscross[i] = new TH2D(name+ii, "",
			     100, -10, 10,
			     100, -1, 1.1 );
    swisscross[i]->SetXTitle("Reco. Time of Seed (ns)");
    swisscross[i]->SetYTitle("Swiss Crx Var.");
    tree->Project(name+ii,"swissCrx:seedTime");
    swisscross[i]->Draw("colz");

    if(save)
    {
      TString savename = "plot_photon_swisscross_";
      savename += i;
      savename += ".eps";
      c2[i]->SaveAs(savename);
    }
    

  }
}
