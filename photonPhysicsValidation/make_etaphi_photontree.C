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
#include "alexUtils.h"

#include "../HiForest_V2_latest/hiForest.h"

void make_etaphi_photontree(const bool save=false,
			    const bool doPilot=true,
			    const bool doMC = false,
			    const bool doData = false,
			    const char *pilotName = "",
			    const char *dataName = "",
			    const char *mcName = "")
{
  TH1::SetDefaultSumw2();

  HiForest *pilotForest;
  if(doPilot){
    pilotForest = new HiForest(pilotName, "pilotForest", cPPb, false);
  }

  HiForest *dataForest;
  if(doData){
    dataForest = new HiForest(dataName, "dataForest", cPPb, false);
  }

  HiForest *mcForest;
  if (doMC){
    mcForest = new HiForest(mcName, "mcForest", cPPb, true);
  }

  TString name;

  TH2D *etaphi[3];
  TH2D *swisscross[3];
  TCanvas *c[3];
  TCanvas *c2[3];
  
  for(int i = 0; i<3; i++)
  {
    HiForest *forest;
    char ii;
    TString selection;
    if(i == 0)
    {
      if(!doPilot) continue;
      forest = pilotForest;
      ii='0';
      selection = "(1==1)";
    } else if (i==1) {
      if(!doData) continue;
      forest = dataForest;
      ii='1';
      selection = "(hltTree.HLT_PAZeroBiasPixel_SingleTrack_v1 && skim.pHBHENoiseFilter && skim.phfPosFilter1 && skim.phfNegFilter1 && skim.phltPixelClusterShapeFilter && skim.pprimaryvertexFilter)";
    } else {
      if(!doMC) continue;
      forest = mcForest;
      ii='2';
      selection = "(1==1)";
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
    forest->tree->Project(name+ii,"phi:eta",selection);
    etaphi[i]->Draw("colz");
    if(save)
    {
      TString savename = "plot_photon_etaphi_";
      savename += i;
      saveCanvas(c[i],savename);
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
    forest->tree->Project(name+ii,"swissCrx:seedTime",selection);
    swisscross[i]->Draw("colz");

    if(save)
    {
      TString savename = "plot_photon_swisscross_";
      savename += i;
      saveCanvas(c2[i],savename);
    }
    

  }
}
