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

void make_sigmaetaeta(const bool save=false,
			    const bool doPilot=true,
			    const bool doMC = false,
			    const bool doData = false,
			    const char *dataName = "")
{
  TH1::SetDefaultSumw2();

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

  const Int_t numPlots = 10;
  TH1D *h[numPlots][3];
  //TString name[numPlots];
  TString name;

  for(int i = 0; i<3; i++)
  {
    HiForest *forest;
    char ii;
    TString Yname = "counts per event";
    TString selection;
    Int_t totalEvents;
    if(i == 0)
    {
      if(!doPilot) continue;
      forest = pilotForest;
      ii='0';
      selection = "(1==1)";
      const char *selectionc = "(1==1)";
      totalEvents = forest->tree->GetEntries(selectionc);
    } else if (i==1) {
      if(!doData) continue;
      forest = dataForest;
      ii='1';
      selection = "(hltTree.HLT_PAZeroBiasPixel_SingleTrack_v1 && skim.pHBHENoiseFilter && skim.phfPosFilter1 && skim.phfNegFilter1 && skim.phltPixelClusterShapeFilter && skim.pprimaryvertexFilter)";
      const char *selectionc = "(hltTree.HLT_PAZeroBiasPixel_SingleTrack_v1 && skim.pHBHENoiseFilter && skim.phfPosFilter1 && skim.phfNegFilter1 && skim.phltPixelClusterShapeFilter && skim.pprimaryvertexFilter)";
      totalEvents = forest->tree->GetEntries(selectionc);
    } else {
      if(!doMC) continue;
      forest = mcForest;
      ii='2';
      selection = "(1==1)";
      const char *selectionc = "(1==1)";
      totalEvents = forest->tree->GetEntries(selectionc);
    }

    name = "sigmaIetaIeta";
    TString add = "Bar";
    TString cut = "&&(abs(eta) < 1.479)";
    h[0][i] = new TH1D(name+add+ii, "",
		      70,0,0.03 );
    h[0][i]->SetMarkerColor(kBlack);
    h[0][i]->SetXTitle("#sigma_{#eta#eta} Barrel");
    h[0][i]->SetYTitle(Yname);
    forest->tree->Project(name+add+ii,name,selection+cut);
    totalEvents = h[0][i]->GetEntries();
    h[0][i]->Scale(1./totalEvents);

    name = "sigmaIetaIeta";
    add = "End";
    cut = "&&(abs(eta) > 1.479)";
    h[1][i] = new TH1D(name+add+ii, "",
		      70,0,0.1 );
    h[1][i]->SetMarkerColor(kBlack);
    h[1][i]->SetXTitle("#sigma_{#eta#eta} Endcap");
    h[1][i]->SetYTitle(Yname);
    forest->tree->Project(name+add+ii,name,selection+cut);
    h[1][i]->Scale(1./totalEvents);

    name = "sigmaIetaIeta";
    add = "Barlp";
    cut = "&&(abs(eta) < 1.479)&&(pt<15)";
    h[2][i] = new TH1D(name+add+ii, "",
		      70,0,0.1 );
    h[2][i]->SetMarkerColor(kRed);
    h[2][i]->SetXTitle("p_{T}<15GeV");
    h[2][i]->SetYTitle(Yname);
    forest->tree->Project(name+add+ii,name,selection+cut);
    h[2][i]->Scale(1./totalEvents);

    name = "sigmaIetaIeta";
    add = "Endlp";
    cut = "&&(abs(eta) > 1.479)&&(pt<15)";
    h[3][i] = new TH1D(name+add+ii, "",
		      70,0,0.1 );
    h[3][i]->SetMarkerColor(kRed);
    h[3][i]->SetXTitle("p_{T}<15GeV");
    h[3][i]->SetYTitle(Yname);
    forest->tree->Project(name+add+ii,name,selection+cut);
    h[3][i]->Scale(1./totalEvents);

    name = "sigmaIetaIeta";
    add = "Barup";
    cut = "&&(abs(eta) < 1.479)&&(pt>15)";
    h[4][i] = new TH1D(name+add+ii, "",
		      70,0,0.1 );
    h[4][i]->SetMarkerColor(kBlue);
    h[4][i]->SetXTitle("p_{T}>15GeV");
    h[4][i]->SetYTitle(Yname);
    forest->tree->Project(name+add+ii,name,selection+cut);
    h[4][i]->Scale(1./totalEvents);

    name = "sigmaIetaIeta";
    add = "Endup";
    cut = "&&(abs(eta) > 1.479)&&(pt>15)";
    h[5][i] = new TH1D(name+add+ii, "",
		      70,0,0.1 );
    h[5][i]->SetMarkerColor(kBlue);
    h[5][i]->SetXTitle("p_{T}>15GeV");
    h[5][i]->SetYTitle(Yname);
    forest->tree->Project(name+add+ii,name,selection+cut);
    h[5][i]->Scale(1./totalEvents);

    TString isoCut = "&& (hadronicOverEm < 0.05) && (cr4+ct4PtCut20 < 4.5)";
    
    name = "sigmaIetaIeta";
    add = "Barlpiso";
    cut = "&&(abs(eta) < 1.479)&&(pt<15)";
    h[6][i] = new TH1D(name+add+ii, "",
		      70,0,0.1 );
    h[6][i]->SetMarkerColor(kCyan);
    h[6][i]->SetXTitle("p_{T}<15GeV isolated");
    h[6][i]->SetYTitle(Yname);
    forest->tree->Project(name+add+ii,name,selection+cut+isoCut);
    h[6][i]->Scale(1./totalEvents);

    name = "sigmaIetaIeta";
    add = "Endlpiso";
    cut = "&&(abs(eta) > 1.479)&&(pt<15)";
    h[7][i] = new TH1D(name+add+ii, "",
		      70,0,0.1 );
    h[7][i]->SetMarkerColor(kCyan);
    h[7][i]->SetXTitle("p_{T}<15GeV isolated");
    h[7][i]->SetYTitle(Yname);
    forest->tree->Project(name+add+ii,name,selection+cut+isoCut);
    h[7][i]->Scale(1./totalEvents);

    name = "sigmaIetaIeta";
    add = "Barupiso";
    cut = "&&(abs(eta) < 1.479)&&(pt>15)";
    h[8][i] = new TH1D(name+add+ii, "",
		      70,0,0.1 );
    h[8][i]->SetMarkerColor(kMagenta);
    h[8][i]->SetXTitle("p_{T}>15GeV isolated");
    h[8][i]->SetYTitle(Yname);
    forest->tree->Project(name+add+ii,name,selection+cut+isoCut);
    h[8][i]->Scale(1./totalEvents);

    name = "sigmaIetaIeta";
    add = "Endupiso";
    cut = "&&(abs(eta) > 1.479)&&(pt>15)";
    h[9][i] = new TH1D(name+add+ii, "",
		      70,0,0.1 );
    h[9][i]->SetMarkerColor(kMagenta);
    h[9][i]->SetXTitle("p_{T}>15GeV isolated");
    h[9][i]->SetYTitle(Yname);
    forest->tree->Project(name+add+ii,name,selection+cut+isoCut);
    h[9][i]->Scale(1./totalEvents);

  }

  TString dataLabel = "data";
  TString pilotLabel = "pilot";
  TString MCLabel = "epos MC";
  TLegend *leg;
  TCanvas *cEnd[3];
  TCanvas *cBar[3];

  for(int i = 0; i < 3; i++)
  {
    if(!doPilot && i==0){
      continue;
    }
    if(!doData && i==1){
      continue;
    }
    if(!doMC && i==2){
      continue;
    }
    
    TString canvName = "cBar";
    canvName += i;
    cBar[i] = new TCanvas(canvName,"",500,500);
    cBar[i]->cd();
    leg = new TLegend(0.7,0.6,0.9,0.9);
    leg->SetFillColor(0);

    h[0][i]->SetAxisRange(0, 0.1, "Y");
    h[0][i]->Draw("E");
    h[2][i]->Draw("E same");
    h[4][i]->Draw("E same");
    h[6][i]->Draw("E same");
    h[8][i]->Draw("E same");

    leg->AddEntry(h[0][i],h[0][i]->GetXaxis()->GetTitle(),"p");
    leg->AddEntry(h[2][i],h[2][i]->GetXaxis()->GetTitle(),"p");
    leg->AddEntry(h[4][i],h[4][i]->GetXaxis()->GetTitle(),"p");
    leg->AddEntry(h[6][i],h[6][i]->GetXaxis()->GetTitle(),"p");
    leg->AddEntry(h[8][i],h[8][i]->GetXaxis()->GetTitle(),"p");
    
    leg->DrawClone();
    if(save)
    {
      TString savename = "plot_sigmaetaeta_Bar";
      savename += i;
      saveCanvas(cBar[i],savename);
    }

    canvName = "cEnd";
    canvName += i;
    cEnd[i] = new TCanvas(canvName,"",500,500);
    cEnd[i]->cd();
    leg = new TLegend(0.7,0.6,0.9,0.9);
    leg->SetFillColor(0);

    h[1][i]->SetAxisRange(0, 0.1, "Y");
    h[1][i]->Draw("E");
    h[3][i]->Draw("E same");
    h[5][i]->Draw("E same");
    h[7][i]->Draw("E same");
    h[9][i]->Draw("E same");

    leg->AddEntry(h[1][i],h[1][i]->GetXaxis()->GetTitle(),"p");
    leg->AddEntry(h[3][i],h[3][i]->GetXaxis()->GetTitle(),"p");
    leg->AddEntry(h[5][i],h[5][i]->GetXaxis()->GetTitle(),"p");
    leg->AddEntry(h[7][i],h[7][i]->GetXaxis()->GetTitle(),"p");
    leg->AddEntry(h[9][i],h[9][i]->GetXaxis()->GetTitle(),"p");
    
    leg->DrawClone();
    if(save)
    {
      TString savename = "plot_sigmaetaeta_End";
      savename += i;
      saveCanvas(cEnd[i],savename);
    }

  }
}
