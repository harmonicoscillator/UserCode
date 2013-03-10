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

#include "../HiForest_V3/hiForest.h"

void make_comp_plots_photontree(const bool save=false,
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
    dataForest = new HiForest(dataName, "dataForest", cPP, false);
  }

  HiForest *mcForest;
  if (doMC){
    mcForest = new HiForest(mcName, "mcForest", cPPb, true);
  }

  const Int_t numPlots = 13;
  TH1D *h[numPlots][3];
  TString name;
  TString add;
  TString cut;
  TString Yname = "counts per event";

  for(int i = 0; i<3; i++)
  {
    int marker;
    int linecolor;
    HiForest *forest;
    char ii;
    TString selection;
    Int_t totalEvents;
    if(i == 0)
    {
      if(!doPilot) continue;
      forest = pilotForest;
      //const char *selectionc = "(1==1)";
      const char *selectionc = "(skim.pHBHENoiseFilter && skim.phfPosFilter1 && skim.phfNegFilter1 && skim.phltPixelClusterShapeFilter && skim.pprimaryvertexFilter)";
      selection = selectionc;
      marker = 24;
      linecolor = (int)kBlack;
      ii='0';
      totalEvents = forest->tree->GetEntries(selectionc);
    } else if (i==1) {
      if(!doData) continue;
      //tree = dataPhotonTree;
      forest = dataForest;
      //const char *selectionc = "(hltTree.HLT_PAZeroBiasPixel_SingleTrack_v1 && skim.pHBHENoiseFilter && skim.phfPosFilter1 && skim.phfNegFilter1 && skim.phltPixelClusterShapeFilter && skim.pprimaryvertexFilter)";
      const char *selectionc = "(1==1)";
      selection = selectionc;
      totalEvents = forest->tree->GetEntries(selectionc);
      marker = 20;
      linecolor = (int)kBlack;
      ii='1';
    } else {
      if(!doMC) continue;
      //tree = mcChain;
      forest = mcForest;
      const char *selectionc = "(skim.phfPosFilter1 && skim.phfNegFilter1 && skim.phltPixelClusterShapeFilter && skim.pprimaryvertexFilter)";
//"(1==1)";
      selection = selectionc;
      totalEvents = forest->tree->GetEntries(selectionc);
      marker = 25;
      linecolor = (int)kRed;
      ii='2';
    }

    name = "pt";
    h[0][i] = new TH1D(name+ii,"",
    		       60, 0, 130);
    h[0][i]->SetMarkerStyle(marker);
    h[0][i]->SetLineColor(linecolor);
    h[0][i]->SetXTitle("p_{T} (GeV)");
    h[0][i]->SetYTitle(Yname);
    forest->tree->Project(name+ii,name,selection);
    //totalEvents = h[0][i]->GetEntries();
    h[0][i]->Scale(1./totalEvents);
		 
    name = "eta";
    h[1][i] = new TH1D(name+ii,"",
    		       40,-5,5);
    h[1][i]->SetMarkerStyle(marker);
    h[1][i]->SetLineColor(linecolor);
    h[1][i]->SetXTitle("#eta");
    h[1][i]->SetYTitle(Yname);
    forest->tree->Project(name+ii,name,selection);
    h[1][i]->Scale(1./totalEvents);
    
    name = "phi";
    h[2][i] = new TH1D(name+ii, "",
    		      36,-TMath::Pi(),TMath::Pi() );
    h[2][i]->SetMarkerStyle(marker);
    h[2][i]->SetLineColor(linecolor);
    h[2][i]->SetXTitle("#phi");
    h[2][i]->SetYTitle(Yname);
    forest->tree->Project(name+ii,name,selection);
    h[2][i]->Scale(1./totalEvents);
    //h[2][i]->SetAxisRange(0, 0.05, "Y");
    h[2][i]->SetMinimum(0);

    name = "hadronicOverEm";
    h[3][i] = new TH1D(name+ii, "",
		      35,0,0.6 );
    h[3][i]->SetMarkerStyle(marker);
    h[3][i]->SetLineColor(linecolor);
    h[3][i]->SetXTitle(name);
    h[3][i]->SetYTitle(Yname);
    forest->tree->Project(name+ii,name,selection);
    h[3][i]->Scale(1./totalEvents);

    name = "cc4";
    h[4][i] = new TH1D(name+ii, "",
    		      50,-20,30 );
    h[4][i]->SetMarkerStyle(marker);
    h[4][i]->SetLineColor(linecolor);
    h[4][i]->SetXTitle("Ecal Iso (cc4) (GeV)");
    h[4][i]->SetYTitle(Yname);
    forest->tree->Project(name+ii,name,selection);
    h[4][i]->Scale(1./totalEvents);

    name = "cr4";
    h[5][i] = new TH1D(name+ii, "",
    		      50,-20,30 );
    h[5][i]->SetMarkerStyle(marker);
    h[5][i]->SetLineColor(linecolor);
    h[5][i]->SetXTitle("Hcal Iso (cr4) (GeV)");
    h[5][i]->SetYTitle(Yname);
    forest->tree->Project(name+ii,name,selection);
    h[5][i]->Scale(1./totalEvents);

    name = "ct4PtCut20";
    h[6][i] = new TH1D(name+ii, "",
    		      50,-20,30 );
    h[6][i]->SetMarkerStyle(marker);
    h[6][i]->SetLineColor(linecolor);
    h[6][i]->SetXTitle("Track Iso (ct4PtCut20) (GeV)");
    h[6][i]->SetYTitle(Yname);
    forest->tree->Project(name+ii,name,selection);
    h[6][i]->Scale(1./totalEvents);

    name = "ecalRecHitSumEtConeDR04";
    add = "Bar";
    cut = "&& (abs(eta) < 1.479)";
    h[7][i] = new TH1D(name+add+ii, "",
    		      50,-2,15 );
    h[7][i]->SetMarkerStyle(marker);
    h[7][i]->SetLineColor(linecolor);
    h[7][i]->SetXTitle("Ecal Iso Barrel (pp style) (GeV)");
    h[7][i]->SetYTitle(Yname);
    forest->tree->Project(name+add+ii,name,selection+cut);
    h[7][i]->Scale(1./totalEvents);

    name = "ecalRecHitSumEtConeDR04";
    add = "End";
    cut = "&& (abs(eta) > 1.479)";
    h[8][i] = new TH1D(name+add+ii, "",
    		      50,-2,15 );
    h[8][i]->SetMarkerStyle(marker);
    h[8][i]->SetLineColor(linecolor);
    h[8][i]->SetXTitle("Ecal Iso Endcap (pp style) (GeV)");
    h[8][i]->SetYTitle(Yname);
    forest->tree->Project(name+add+ii,name,selection+cut);
    h[8][i]->Scale(1./totalEvents);

    name = "hcalTowerSumEtConeDR04";
    add = "Bar";
    cut = "&& (abs(eta) < 1.479)";
    h[9][i] = new TH1D(name+add+ii, "",
    		      50,-2,15 );
    h[9][i]->SetMarkerStyle(marker);
    h[9][i]->SetLineColor(linecolor);
    h[9][i]->SetXTitle("Hcal Iso Barrel (pp style) (GeV)");
    h[9][i]->SetYTitle(Yname);
    forest->tree->Project(name+add+ii,name,selection+cut);
    h[9][i]->Scale(1./totalEvents);

    name = "hcalTowerSumEtConeDR04";
    add = "End";
    cut = "&& (abs(eta) > 1.479)";
    h[10][i] = new TH1D(name+add+ii, "",
    		      50,-2,15 );
    h[10][i]->SetMarkerStyle(marker);
    h[10][i]->SetLineColor(linecolor);
    h[10][i]->SetXTitle("Hcal Iso Endcap (pp style) (GeV)");
    h[10][i]->SetYTitle(Yname);
    forest->tree->Project(name+add+ii,name,selection+cut);
    h[10][i]->Scale(1./totalEvents);

    name = "trkSumPtHollowConeDR04";
    add = "Bar";
    cut = "&& (abs(eta) < 1.479)";
    h[11][i] = new TH1D(name+add+ii, "",
    		      50,-2,15 );
    h[11][i]->SetMarkerStyle(marker);
    h[11][i]->SetLineColor(linecolor);
    h[11][i]->SetXTitle("Track Iso Barrel (pp style) (GeV)");
    h[11][i]->SetYTitle(Yname);
    forest->tree->Project(name+add+ii,name,selection+cut);
    h[11][i]->Scale(1./totalEvents);

    name = "trkSumPtHollowConeDR04";
    add = "End";
    cut = "&& (abs(eta) > 1.479)";
    h[12][i] = new TH1D(name+add+ii, "",
    		      50,-2,15 );
    h[12][i]->SetMarkerStyle(marker);
    h[12][i]->SetLineColor(linecolor);
    h[12][i]->SetXTitle("Track Iso Endcap (pp style) (GeV)");
    h[12][i]->SetYTitle(Yname);
    forest->tree->Project(name+add+ii,name,selection+cut);
    h[12][i]->Scale(1./totalEvents);
  }

  TString dataLabel = "data";
  TString pilotLabel = "pilot";
  TString MCLabel = "HIJING";
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
    if(i == 0 || i == 3)
      c[i]->SetLogy();

    if(save)
    {
      TString savename = "plot_photons_";
      savename += i;
      saveCanvas(c[i],savename);
    }    
  }
  // c[0] = new TCanvas("ratio","",600,600);
  // c[0]->cd();
  // h[3][1]->Divide(h[3][0]);
  // h[3][1]->SetYTitle("(211390)/(211256)");
  // h[3][1]->DrawClone("E");
}
