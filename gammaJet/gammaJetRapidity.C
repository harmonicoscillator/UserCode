#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TString.h"
#include "stdio.h"
#include "../HiForest_V3/hiForest.h"


void gammaJetRapidity(bool doMC = false)
{ 
  TString files[2];

  files[0] = "/mnt/hadoop/cms/store/user/luck/pA2013_forests/PA2013_HiForest_PromptReco_HLT_Photon40.root";
  files[1] = "/mnt/hadoop/cms/store/user/luck/pA2013_MC/HiForest2_QCDPhoton30_5020GeV_100k.root";

  bool montecarlo[2] = {false, true};

  TString name;
  name = "gammaJets_inclusive_dphi7pi8_data_v2.root";
  if(doMC)
    name = "gammaJets_inclusive_dphi7pi8_MC_v2.root";

  TFile *outfile = new TFile(name,"RECREATE");

  TString varList;
  varList = "gPt:gEta:gPhi:jPt:jEta:jPhi:HF:HFeta4:avgEta:dPhi:cc4:cr4:ct4PtCut20:hadronicOverEm:sigmaIetaIeta:run:r9";
  if(doMC)
    varList += ":genMomId:genCalIsoDR04:genTrkIsoDR04";
  
  TNtuple *outTuple = new TNtuple("gammaJets","gammaJets",varList);
  
  int ii = 0;
  if(doMC)
    ii = 1;  
  
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

    //event selection
    if( !((montecarlo[ii] || c->skim.pHBHENoiseFilter) && c->skim.phfPosFilter1 && c->skim.phfNegFilter1 && c->skim.phltPixelClusterShapeFilter && c->skim.pprimaryvertexFilter) )
      continue;

    if(c->photon.nPhotons == 0)
      continue;

    //loop over photons in the event
    Float_t leadingPt = 0;
    Int_t leadingIndex = -1;
    for(int i = 0; i<c->photon.nPhotons; i++)
    {
      if(c->photon.pt[i] > leadingPt)
      {
	leadingPt = c->photon.pt[i];
	leadingIndex = i;
      }
    }
      
    if(leadingIndex == -1) 
      continue;

    //loop over 'away' jets
    int jet2index = -1;
    double jet2pt = 0;
    for(int i = 0; i<c->akPu3PF.nref; i++)
    {
      if( TMath::Abs(c->akPu3PF.jteta[i]) > 3.0)
	continue;
	
      Double_t dphi = TMath::ACos(TMath::Cos(c->photon.phi[leadingIndex] - c->akPu3PF.jtphi[i]));
      if( dphi < 7.*TMath::Pi()/8. )
	continue;
	
      if(c->akPu3PF.jtpt[i] > jet2pt)
      {
	jet2pt = c->akPu3PF.jtpt[i];
	jet2index = i;
      }
    }

    if(jet2index == -1)
      continue;

    Double_t dphi = TMath::ACos(TMath::Cos(c->photon.phi[leadingIndex] - c->akPu3PF.jtphi[jet2index]));

    // gPt:gEta:gPhi:jPt:jEta:jPhi:HF:HFeta4:avgEta:dPhi:cc4:cr4:ct4PtCut20:hadronicOverEm:sigmaIetaIeta:run
    Float_t gPt = c->photon.pt[leadingIndex];
    Float_t gEta = c->photon.eta[leadingIndex];
    Float_t gPhi = c->photon.phi[leadingIndex];
    Float_t jPt = c->akPu3PF.jtpt[jet2index];
    Float_t jEta = c->akPu3PF.jteta[jet2index];
    Float_t jPhi = c->akPu3PF.jtphi[jet2index];
    Float_t HF = c->evt.hiHF;
    Float_t HFeta4 = c->evt.hiHFplusEta4 + c->evt.hiHFminusEta4;
    Float_t avgEta = (c->photon.eta[leadingIndex] + c->akPu3PF.jteta[jet2index])/2.0;
    Float_t dPhi = dphi;
    Float_t cc4 = c->photon.cc4[leadingIndex];
    Float_t cr4 = c->photon.cr4[leadingIndex];
    Float_t ct4PtCut20 = c->photon.ct4PtCut20[leadingIndex];
    Float_t hadronicOverEm = c->photon.hadronicOverEm[leadingIndex];
    Float_t sigmaIetaIeta = c->photon.sigmaIetaIeta[leadingIndex];
    Float_t run = c->evt.run;
    Float_t r9 = c->photon.r9[leadingIndex];

    if(!doMC)
    {
      Float_t x[] = {gPt,gEta,gPhi,jPt,jEta,jPhi,HF,HFeta4,avgEta,dPhi,cc4,cr4,ct4PtCut20,hadronicOverEm,sigmaIetaIeta,run,r9};
      outTuple->Fill(x);
    }
    else
    {
      Float_t genMomId = c->photon.genMomId[leadingIndex];
      Float_t genCalIsoDR04 = c->photon.genCalIsoDR04[leadingIndex];
      Float_t genTrkIsoDR04 = c->photon.genTrkIsoDR04[leadingIndex];

      Float_t x[] = {gPt,gEta,gPhi,jPt,jEta,jPhi,HF,HFeta4,avgEta,dPhi,cc4,cr4,ct4PtCut20,hadronicOverEm,sigmaIetaIeta,run,r9,genMomId,genCalIsoDR04,genTrkIsoDR04};
      outTuple->Fill(x);
    }

  }

  outfile->cd();

  outTuple->Write();
  outfile->Close();
}


