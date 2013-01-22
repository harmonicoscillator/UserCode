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

void make_comp_plots_rechittree(const bool save=false,
				const bool doPilot=true,
				const bool doMC = false,
				const bool doData = false,
				const char *dataName = "",
				const TString section = "eb")
{
  TH1::SetDefaultSumw2();

  TString cut = "";//"(et>15)";

  HiForest *pilotForest;
  if(doPilot){    
    pilotForest = new HiForest("/mnt/hadoop/cms/store/user/luck/pA2013_pilot/forest200kHz.root",
			       "pilotForest", cPPb, false);
  }

  if(dataName == "") dataName = "/mnt/hadoop/cms/store/user/luck/pA2013_forests/PA2013_HiForest_Express_r210534_stablebeams_72bunch.root";

  HiForest *dataForest;
  if(doData){
    dataForest = new HiForest(dataName, "dataForest", cPPb, false);
  }

  HiForest *mcForest;
  if (doMC){
    mcForest = new HiForest ("/mnt/hadoop/cms/store/user/luck/pA2013_MC/HiForest_pPb_Hijing_NEWFIX_v2.root",
			     "mcForest", cPPb, true);
  }

  const Int_t numPlots = 4;
  TH1D *h[numPlots][3];
  TH2D *etaphi[3];
  //TString name[numPlots];
  TString name;
  TCanvas *c2[3];

  for(int i = 0; i<3; i++)
  {
    int marker;
    HiForest *forest;
    char ii;
    TString Yname = "counts per event";
    TString label = "";
    int markerColor;
    TString selection;
    Int_t totalEvents;
    if(i == 0)
    {
      if(!doPilot) continue;
      forest = pilotForest;
      marker = 24;
      markerColor = (int)kBlack;
      ii='0';
      selection = "(1==1)";
      const char *selectionc = "(1==1)";
      totalEvents = forest->tree->GetEntries(selectionc);
    } else if (i==1) {
      if(!doData) continue;
      forest = dataForest;
      marker = 20;
      markerColor = (int)kBlack;
      ii='1';
      selection = "(hltTree.HLT_PAZeroBiasPixel_SingleTrack_v1 && skim.pHBHENoiseFilter && skim.phfPosFilter1 && skim.phfNegFilter1 && skim.phltPixelClusterShapeFilter && skim.pprimaryvertexFilter)";
      const char *selectionc = "(hltTree.HLT_PAZeroBiasPixel_SingleTrack_v1 && skim.pHBHENoiseFilter && skim.phfPosFilter1 && skim.phfNegFilter1 && skim.phltPixelClusterShapeFilter && skim.pprimaryvertexFilter)";
      totalEvents = forest->tree->GetEntries(selectionc);
    } else {
      if(!doMC) continue;
      forest = mcForest;
      marker = 25;
      markerColor = (int)kRed;
      ii='2';
      selection = "(1==1)";
      const char *selectionc = "(1==1)";
      totalEvents = forest->tree->GetEntries(selectionc);
    }

    (section == "eb") ? label="Barrel" : label = "Endcap";

    name = section+".e";
    h[0][i] = new TH1D(name+ii,"",
		       50,0,100);
    h[0][i]->SetMarkerStyle(marker);
    h[0][i]->SetLineColor(markerColor);
    h[0][i]->SetXTitle("e " + label );
    h[0][i]->SetYTitle(Yname);
    forest->tree->Project(name+ii,name,selection+cut);
    //totalEvents = h[0][i]->GetEntries();
    h[0][i]->Scale(1./totalEvents);

    name = section+".et";
    h[1][i] = new TH1D(name+ii,"",
		       50,0,100);
    h[1][i]->SetMarkerStyle(marker);
    h[1][i]->SetLineColor(markerColor);
    h[1][i]->SetXTitle("et (GeV) " + label );
    h[1][i]->SetYTitle(Yname);
    forest->tree->Project(name+ii,name,selection+cut);
    h[1][i]->Scale(1./totalEvents);

    name = section+".eta";
    h[2][i] = new TH1D(name+ii,"",
		       50,-5,5);
    h[2][i]->SetMarkerStyle(marker);
    h[2][i]->SetLineColor(markerColor);
    h[2][i]->SetXTitle("#eta " + label );
    h[2][i]->SetYTitle(Yname);
    forest->tree->Project(name+ii,name,selection+cut);
    h[2][i]->Scale(1./totalEvents);

    name = section+".phi";
    h[3][i] = new TH1D(name+ii,"",
		       18,-TMath::Pi(),TMath::Pi() );
    h[3][i]->SetMarkerStyle(marker);
    h[3][i]->SetLineColor(markerColor);
    h[3][i]->SetXTitle("#phi " + label );
    h[3][i]->SetYTitle(Yname);
    forest->tree->Project(name+ii,name,selection+cut);
    h[3][i]->Scale(1./totalEvents);
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
    forest->tree->Project(name+ii,section+".phi:"+section+".eta",selection+cut);
    etaphi[i]->Draw("colz");
    if(save)
    {
      TString savename = "plot_etaphi_rechit_"+section;
      savename += i;
      saveCanvas(c2[i],savename);
    }

  }

  TString dataLabel = "data";
  TString pilotLabel = "pilot";
  TString MCLabel = "HIJING";
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
      leg->AddEntry(h[i][2],MCLabel,"l");
      h[i][2]->SetLineColor(kRed);
      h[i][2]->DrawClone("hist same");
    }
    leg->DrawClone();
    if(i == 0 || i == 1)
      c[i]->SetLogy();


    if(save)
    {
      TString savename = "plot_rechit_" + section;
      savename += i;
      saveCanvas(c[i],savename);
    }
  }

  //2D plots
  

  
}
