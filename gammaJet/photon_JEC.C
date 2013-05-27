#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TString.h"
#include "stdio.h"
#include "../HiForest_V3/hiForest.h"


//const Double_t ptbins[] = {20, 30, 50, 70, 100, 150, 300}; //7
//const Int_t nptbins = 7;
const Double_t ptbins[] = {0,1000};
const Int_t nptbins = 2;
int returnPtBin(double pt);

void photon_JEC()
{
  TH1::SetDefaultSumw2();

  Double_t alphabins[] = {0.075, 0.125, 0.175, .25, .35};

  TH1D *RdRmcalpha[nptbins];
  TProfile *Ratio[2][nptbins];

  for(int i = 0; i < nptbins; i++)
  {
    TString name;
    name = "RdRmcalpha_";
    name += ptbins[i];
    RdRmcalpha[i] = new TH1D(name,name+";#alpha;R_{data}/R_{MC}",4,alphabins);
    name = "Rd_";
    name += ptbins[i];
    Ratio[0][i] = new TProfile(name,"",4,alphabins);
    name = "Rmc_";
    name += ptbins[i];
    Ratio[1][i] = new TProfile(name,"",4,alphabins);
  }

  TH1D *alphas = new TH1D("alphas",";#alpha = p_{T}^{Jet3}/p_{T}^{#gamma}",100,0,0.8);
  
  TH1D *hPhotonPt[2];
  hPhotonPt[0] = new TH1D("hPhotonPt_data",";photon p_{T}",200,0,400);
  hPhotonPt[1] = (TH1D*)hPhotonPt[0]->Clone("hPhotonPt_mc");
  TH1D *hPhotonEta[2];
  hPhotonEta[0] = new TH1D("hPhotonEta_data",";photon #eta",26,-3,3);
  hPhotonEta[1] = (TH1D*)hPhotonEta[0]->Clone("hPhotonEta_mc");

  TH1D *hJet2Pt[2];
  hJet2Pt[0] = new TH1D("hJet2Pt_data",";jet 2 p_{T}",200,0,400);
  hJet2Pt[1] = (TH1D*)hJet2Pt[0]->Clone("hJet2Pt_mc");
  TH1D *hJet2Eta[2];
  hJet2Eta[0] = new TH1D("hJet2Eta_data",";jet 2 #eta",26,-3,3);
  hJet2Eta[1] = (TH1D*)hJet2Eta[0]->Clone("hJet2Eta_mc");

  TH1D *hJet3Pt[2];
  hJet3Pt[0] = new TH1D("hJet3Pt_data",";jet 3 p_{T}",200,0,400);
  hJet3Pt[1] = (TH1D*)hJet3Pt[0]->Clone("hJet3Pt_mc");
  TH1D *hJet3Eta[2];
  hJet3Eta[0] = new TH1D("hJet3Eta_data",";jet 3 #eta",26,-3,3);
  hJet3Eta[1] = (TH1D*)hJet3Eta[0]->Clone("hJet3Eta_mc");


  TString files[2];

  files[0] = "/mnt/hadoop/cms/store/user/luck/pA2013_forests/PA2013_HiForest_PromptReco_HLT_Photon40.root";
  files[1] = "/mnt/hadoop/cms/store/user/luck/pA2013_MC/HiForest2_QCDPhoton30_5020GeV_100k.root";

  bool montecarlo[2] = {false, true};

  //loop over files
  //do the smaller MC file first, for some reason I have segfaults when I process the MC second.
  for(int ii = 1; ii > -1; ii--)
  {
    HiForest *c = new HiForest(files[ii], "Forest", cPPb, montecarlo[ii]);
    c->InitTree();

    //loop over events in each file
    int nentries = c->GetEntries();
    for(int jentry = 0; jentry<nentries; jentry++)
    {
      if (jentry% 1000 == 0)  {
	printf("%d / %d\n",jentry,nentries);
      }
    
      c->GetEntry(jentry);

      //collision selection
      if( !((montecarlo[ii] || c->skim.pHBHENoiseFilter) && c->skim.phfPosFilter1 && c->skim.phfNegFilter1 && c->skim.phltPixelClusterShapeFilter && c->skim.pprimaryvertexFilter) )
	continue;

      if(c->photon.nPhotons == 0)
	continue;

      //loop over photons in the event
      Float_t leadingPt = 40; //minPt is 40GeV
      Int_t leadingIndex = -1;
      for(int i = 0; i<c->photon.nPhotons; i++)
      {
	if(c->photon.pt[i] > leadingPt)
	{
	  leadingPt = c->photon.pt[i];
	  leadingIndex = i;
	}
      }
      
      if(leadingIndex == -1) // no photons above minPt
	continue;
      if ( TMath::Abs(c->photon.eta[leadingIndex]) > 1.3 ) // barrel photons only
      	continue;
      if(c->photon.ecalRecHitSumEtConeDR04[leadingIndex] > TMath::Min(3., 0.05*c->photon.energy[leadingIndex]) )
	continue;
      if(c->photon.hcalTowerSumEtConeDR04[leadingIndex] > TMath::Min(2.4, 0.05*c->photon.energy[leadingIndex]) )
	continue;
      //if(c->photon.trkSumPtHollowConeDR04[leadingIndex] > 0.10*leadingPt )
      //continue;
      //if ( (c->photon.sigmaIetaIeta[leadingIndex] < 0.15) || (c->photon.sigmaIetaIeta[leadingIndex] > 0.35) ) 
	  //continue;
      //if ( (c->photon.sigmaIphiIphi[leadingIndex] < 0.15) || (c->photon.sigmaIphiIphi[leadingIndex] > 0.30) ) 
      //continue;
      if ( c->photon.sigmaIetaIeta[leadingIndex] > 0.01 )
	continue;

      //loop over 'away' jets
      int jet2index = -1;
      double jet2pt = 0;
      int jet3index = -1;
      double jet3pt = 0;
      for(int i = 0; i<c->akPu3PF.nref; i++)
      {
	if( TMath::Abs(c->akPu3PF.jteta[i]) > 3.0)
	  continue;
	Double_t dphi = TMath::ACos(TMath::Cos(c->photon.phi[leadingIndex] - c->akPu3PF.jtphi[i]));
	if(dphi < TMath::Pi()/2)
	  continue;
	if(c->akPu3PF.jtpt[i] > jet2pt)
	{
	  jet3pt = jet2pt;
	  jet3index = jet2index;
	  jet2pt = c->akPu3PF.jtpt[i];
	  jet2index = i;
	}
	else if(c->akPu3PF.jtpt[i] > jet3pt)
	{
	  jet3pt = c->akPu3PF.jtpt[i];
	  jet3index = i;
	}
      }

      if(jet3index == -1) //no 3rd jet means alpha = 0, we don't care
	continue;
      if( TMath::Abs(c->akPu3PF.jteta[jet2index]) > 1.3 )
	continue;
      Double_t dphi = TMath::ACos(TMath::Cos(c->photon.phi[leadingIndex] - c->akPu3PF.jtphi[jet2index]));
      if( dphi < 2.7 )
	continue;

      Double_t alpha = jet3pt/leadingPt;
      Double_t ratio = jet2pt/leadingPt;

      int ptbin = returnPtBin(leadingPt);
      if(ptbin != -1)
	Ratio[ii][ptbin]->Fill(alpha,ratio);
      
      alphas->Fill(alpha);
      hPhotonPt[ii]->Fill(leadingPt);
      hPhotonEta[ii]->Fill( c->photon.eta[leadingIndex] );
      hJet2Pt[ii]->Fill(jet2pt);
      hJet2Eta[ii]->Fill( c->akPu3PF.jteta[jet2index] );
      hJet3Pt[ii]->Fill(jet3pt);
      hJet3Eta[ii]->Fill( c->akPu3PF.jteta[jet3index] );
    }
  }

  TCanvas *canvas[nptbins];
  for(int i = 0; i < nptbins; i++)
  {
    TH1D *hRatiod = Ratio[0][i]->ProjectionX();
    TH1D *hRatiomc = Ratio[1][i]->ProjectionX();
    RdRmcalpha[i]->Divide(hRatiod,hRatiomc,1,1);

    canvas[i] = new TCanvas();
    RdRmcalpha[i]->Draw();
  }

  TCanvas *danvas = new TCanvas();
  alphas->Draw();
  danvas->SaveAs("alpha_counts.gif");


  hPhotonPt[0]->Scale( 1./hPhotonPt[0]->GetEntries() );
  hPhotonPt[1]->Scale( 1./hPhotonPt[1]->GetEntries() );
  TCanvas *d1 = new TCanvas();
  hPhotonPt[0]->Draw();
  hPhotonPt[1]->SetMarkerColor(kRed);
  hPhotonPt[1]->Draw("same");
  d1->SetLogy();
  TLegend *leg = new TLegend(0.75,0.75,0.9,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(hPhotonPt[0], "Data","P");
  leg->AddEntry(hPhotonPt[1], "MC", "P");
  leg->DrawClone();  
  d1->SaveAs("photon_pt.gif");

  hPhotonEta[0]->Scale( 1./hPhotonEta[0]->GetEntries() );
  hPhotonEta[1]->Scale( 1./hPhotonEta[1]->GetEntries() );
  TCanvas *d2 = new TCanvas();
  hPhotonEta[0]->Draw();
  hPhotonEta[1]->SetMarkerColor(kRed);
  hPhotonEta[1]->Draw("same");
  leg->DrawClone();
  d2->SaveAs("photon_eta.gif");

  hJet2Pt[0]->Scale( 1./hJet2Pt[0]->GetEntries() );
  hJet2Pt[1]->Scale( 1./hJet2Pt[1]->GetEntries() );
  TCanvas *d3 = new TCanvas();
  hJet2Pt[0]->Draw();
  hJet2Pt[1]->SetMarkerColor(kRed);
  hJet2Pt[1]->Draw("same");
  d3->SetLogy();
  leg->DrawClone();
  d3->SaveAs("jet2_pt.gif");

  hJet2Eta[0]->Scale( 1./hJet2Eta[0]->GetEntries() );
  hJet2Eta[1]->Scale( 1./hJet2Eta[1]->GetEntries() );
  TCanvas *d4 = new TCanvas();
  hJet2Eta[0]->Draw();
  hJet2Eta[1]->SetMarkerColor(kRed);
  hJet2Eta[1]->Draw("same");
  leg->DrawClone();
  d4->SaveAs("jet2_eta.gif");

  hJet3Pt[0]->Scale( 1./hJet3Pt[0]->GetEntries() );
  hJet3Pt[1]->Scale( 1./hJet3Pt[1]->GetEntries() );
  TCanvas *d5 = new TCanvas();
  hJet3Pt[0]->Draw();
  hJet3Pt[1]->SetMarkerColor(kRed);
  hJet3Pt[1]->Draw("same");
  d5->SetLogy();
  leg->DrawClone();
  d5->SaveAs("jet3_pt.gif");

  hJet3Eta[0]->Scale( 1./hJet3Eta[0]->GetEntries() );
  hJet3Eta[1]->Scale( 1./hJet3Eta[1]->GetEntries() );
  TCanvas *d6 = new TCanvas();
  hJet3Eta[0]->Draw();
  hJet3Eta[1]->SetMarkerColor(kRed);
  hJet3Eta[1]->Draw("same");
  leg->DrawClone();
  d6->SaveAs("jet3_eta.gif");
}


int returnPtBin(double pt)
{
  for(int i = 0; i < nptbins; i++)
  {
    if( pt < ptbins[i] )
      return(i-1);
  }
  return -1;
}  
