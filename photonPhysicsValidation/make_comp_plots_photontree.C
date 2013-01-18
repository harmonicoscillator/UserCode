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

void make_comp_plots_photontree(const bool save=false,
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

  // pilotPhotonTree->Draw("pt","","E");
  // dataPhotonTree->Draw("pt","","E same");

  const Int_t numPlots = 13;
  TH1D *h[numPlots][3];
  //TString name[numPlots];
  TString name;

  for(int i = 0; i<3; i++)
  {
    int marker;
    TTree *tree;
    char ii;
    TString Yname = "counts per event";
    if(i == 0)
    {
      if(!doPilot) continue;
      tree = pilotPhotonTree;
      marker = 24;
      ii='0';
    } else if (i==1) {
      if(!doData) continue;
      tree = dataPhotonTree;
      marker = 20;
      ii='1';
    } else {
      if(!doMC) continue;
      tree = mcChain;
      marker = 25;
      ii='2';
    }

    name = "pt";
    h[0][i] = new TH1D(name+ii,"",
		       130, 0, 130);
    h[0][i]->SetMarkerStyle(marker);
    h[0][i]->SetXTitle("p_{T} (GeV)");
    h[0][i]->SetYTitle(Yname);
    tree->Project(name+ii,name);
    h[0][i]->Scale(1./h[0][i]->GetEntries());
		 
    name = "eta";
    h[1][i] = new TH1D(name+ii,"",
		       40,-5,5);
    h[1][i]->SetMarkerStyle(marker);
    h[1][i]->SetXTitle("#eta");
    h[1][i]->SetYTitle(Yname);
    tree->Project(name+ii,name);
    h[1][i]->Scale(1./h[1][i]->GetEntries());
    
    name = "phi";
    h[2][i] = new TH1D(name+ii, "",
		      36,-TMath::Pi(),TMath::Pi() );
    h[2][i]->SetMarkerStyle(marker);
    h[2][i]->SetXTitle("#phi");
    h[2][i]->SetYTitle(Yname);
    tree->Project(name+ii,name);
    h[2][i]->Scale(1./h[2][i]->GetEntries());
    h[2][i]->SetAxisRange(0, 0.05, "Y");

    name = "hadronicOverEm";
    h[3][i] = new TH1D(name+ii, "",
		      70,0,0.6 );
    h[3][i]->SetMarkerStyle(marker);
    h[3][i]->SetXTitle(name);
    h[3][i]->SetYTitle(Yname);
    tree->Project(name+ii,name);
    h[3][i]->Scale(1./h[3][i]->GetEntries());

    name = "cc4";
    h[4][i] = new TH1D(name+ii, "",
		      200,-20,30 );
    h[4][i]->SetMarkerStyle(marker);
    h[4][i]->SetXTitle("Ecal Iso (cc4) (GeV)");
    h[4][i]->SetYTitle(Yname);
    tree->Project(name+ii,name);
    h[4][i]->Scale(1./h[4][i]->GetEntries());

    name = "cr4";
    h[5][i] = new TH1D(name+ii, "",
		      100,-20,30 );
    h[5][i]->SetMarkerStyle(marker);
    h[5][i]->SetXTitle("Hcal Iso (cr4) (GeV)");
    h[5][i]->SetYTitle(Yname);
    tree->Project(name+ii,name);
    h[5][i]->Scale(1./h[5][i]->GetEntries());

    name = "ct4PtCut20";
    h[6][i] = new TH1D(name+ii, "",
		      100,-20,30 );
    h[6][i]->SetMarkerStyle(marker);
    h[6][i]->SetXTitle("Track Iso (ct4PtCut20) (GeV)");
    h[6][i]->SetYTitle(Yname);
    tree->Project(name+ii,name);
    h[6][i]->Scale(1./h[6][i]->GetEntries());

    name = "ecalRecHitSumEtConeDR04";
    TString add = "Bar";
    TString cut = "(abs(eta) < 1.479)";
    h[7][i] = new TH1D(name+add+ii, "",
		      100,-2,15 );
    h[7][i]->SetMarkerStyle(marker);
    h[7][i]->SetXTitle("Ecal Iso Barrel (pp style) (GeV)");
    h[7][i]->SetYTitle(Yname);
    tree->Project(name+add+ii,name,cut);
    h[7][i]->Scale(1./h[7][i]->GetEntries());

    add = "End";
    cut = "(abs(eta) > 1.479)";
    h[8][i] = new TH1D(name+add+ii, "",
		      100,-2,15 );
    h[8][i]->SetMarkerStyle(marker);
    h[8][i]->SetXTitle("Ecal Iso Endcap (pp style) (GeV)");
    h[8][i]->SetYTitle(Yname);
    tree->Project(name+add+ii,name,cut);
    h[8][i]->Scale(1./h[8][i]->GetEntries());

    name = "hcalTowerSumEtConeDR04";
    add = "Bar";
    cut = "(abs(eta) < 1.479)";
    h[9][i] = new TH1D(name+add+ii, "",
		      100,-2,15 );
    h[9][i]->SetMarkerStyle(marker);
    h[9][i]->SetXTitle("Hcal Iso Barrel (pp style) (GeV)");
    h[9][i]->SetYTitle(Yname);
    tree->Project(name+add+ii,name,cut);
    h[9][i]->Scale(1./h[9][i]->GetEntries());

    add = "End";
    cut = "(abs(eta) > 1.479)";
    h[10][i] = new TH1D(name+add+ii, "",
		      100,-2,15 );
    h[10][i]->SetMarkerStyle(marker);
    h[10][i]->SetXTitle("Hcal Iso Endcap (pp style) (GeV)");
    h[10][i]->SetYTitle(Yname);
    tree->Project(name+add+ii,name,cut);
    h[10][i]->Scale(1./h[10][i]->GetEntries());

    name = "trkSumPtHollowConeDR04";
    add = "Bar";
    cut = "(abs(eta) < 1.479)";
    h[11][i] = new TH1D(name+add+ii, "",
		      100,-2,15 );
    h[11][i]->SetMarkerStyle(marker);
    h[11][i]->SetXTitle("Track Iso Barrel (pp style) (GeV)");
    h[11][i]->SetYTitle(Yname);
    tree->Project(name+add+ii,name,cut);
    h[11][i]->Scale(1./h[11][i]->GetEntries());

    add = "End";
    cut = "(abs(eta) > 1.479)";
    h[12][i] = new TH1D(name+add+ii, "",
		      100,-2,15 );
    h[12][i]->SetMarkerStyle(marker);
    h[12][i]->SetXTitle("Track Iso Endcap (pp style) (GeV)");
    h[12][i]->SetYTitle(Yname);
    tree->Project(name+add+ii,name,cut);
    h[12][i]->Scale(1./h[12][i]->GetEntries());

    // name = "swissCrx";
    // h[13][i] = new TH1D(name+ii, "",
    // 		       100,-1,1.1 );
    // h[13][i]->SetMarkerStyle(marker);
    // h[13][i]->SetXTitle("Swiss Crx Var.");
    // h[13][i]->SetYTitle(Yname);
    // tree->Project(name+ii,name);
    // h[13][i]->Scale(1./h[13][i]->GetEntries());

    // name = "seedTime";
    // h[14][i] = new TH1D(name+ii, "",
    // 		       100,-10, 10 );
    // h[14][i]->SetMarkerStyle(marker);
    // h[14][i]->SetXTitle("Reco. Time of Seed (ns)");
    // h[14][i]->SetYTitle(Yname);
    // tree->Project(name+ii,name);
    // h[14][i]->Scale(1./h[14][i]->GetEntries());

    // name = "sigmaIetaIeta";
    // TString add = "Bar";
    // TString cut = "(abs(eta) < 1.479)";
    // h[15][i] = new TH1D(name+add+ii, "",
    // 		      70,0,0.025 );
    // h[15][i]->SetMarkerStyle(marker);
    // h[15][i]->SetXTitle("#sigma_{#eta#eta} Barrel");
    // h[15][i]->SetYTitle(Yname);
    // tree->Project(name+add+ii,name,cut);
    // h[15][i]->Scale(1./h[15][i]->GetEntries());
    

    // name = "sigmaIetaIeta";
    // add = "End";
    // cut = "(abs(eta) > 1.479)";
    // h[16][i] = new TH1D(name+add+ii, "",
    // 		      70,0,0.1 );
    // h[16][i]->SetMarkerStyle(marker);
    // h[16][i]->SetXTitle("#sigma_{#eta#eta} Endcap");
    // h[16][i]->SetYTitle(Yname);
    // tree->Project(name+add+ii,name,cut);
    // h[16][i]->Scale(1./h[16][i]->GetEntries());


  }

  TString dataLabel = "data";
  TString pilotLabel = "pilot";
  TString MCLabel = "epos MC";
  TLegend *leg;
  TCanvas *c[numPlots];

  for(int i = 0; i < numPlots; i++)
  {
    TString canvName = "cphoton";
    canvName += i;
    c[i] = new TCanvas(canvName,"",500,500);
    c[i]->cd();
    leg = new TLegend(0.8,0.6,0.9,0.7);
    leg->SetFillColor(0);
    if(doPilot){
      leg->AddEntry(h[i][0],pilotLabel,"p");
      h[i][0]->DrawClone("E");
    }
    if(doData){
      leg->AddEntry(h[i][1],dataLabel,"p");
      h[i][1]->DrawClone("E same");
    }
    if(doMC){
      leg->AddEntry(h[i][2],MCLabel,"p");
      h[i][2]->DrawClone("E same");
    }
    leg->DrawClone();
    if(i == 0 || i == 3)
      c[i]->SetLogy();

    if(save)
    {
      TString savename = "plot_photons_";
      savename += i;
      savename += ".eps";
      c[i]->SaveAs(savename);
    }
  }
}
