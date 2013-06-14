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
#include "TLorentzVector.h"
#include "../HiForest_V3/hiForest.h"


void gammaJet_diphotonMass(bool doMC = false)
{
  TH1::SetDefaultSumw2();
  
  TString files[2];
  files[0] = "/mnt/hadoop/cms/store/user/luck/pA2013_forests/PA2013_HiForest_PromptReco_HLT_Photon40.root";
  files[1] = "/mnt/hadoop/cms/store/user/luck/pA2013_MC/HiForest2_QCDPhoton30_5020GeV_100k.root";

  TH1D *invariantMass = new TH1D("invariantMass",";M_{diphoton}",100,50,150);
  
  int ii = 0;
  if(doMC)
    ii = 1;  
  
  HiForest *c = new HiForest(files[ii], "Forest", cPPb, doMC);
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
    if( !((doMC || c->skim.pHBHENoiseFilter) && c->skim.phfPosFilter1 && c->skim.phfNegFilter1 && c->skim.phltPixelClusterShapeFilter && c->skim.pprimaryvertexFilter) )
      continue;

    if(c->photon.nPhotons == 0)
      continue;

    for(int i = 0; i < c->photon.nPhotons-1; i++) {
      Double_t sumIso_i = c->photon.cc4[i] + c->photon.cr4[i] + c->photon.ct4PtCut20[i];
      if(sumIso_i > 1.0) continue;
      for(int j = i+1; j < c->photon.nPhotons; j++) {
	Double_t sumIso_j = c->photon.cc4[j] + c->photon.cr4[j] + c->photon.ct4PtCut20[j];
	if(sumIso_j > 1.0) continue;


	TLorentzVector v1, v2, vSum;
	v1.SetPtEtaPhiE( c->photon.pt[i],
			 c->photon.eta[i],
			 c->photon.phi[i],
			 c->photon.energy[i]);
	v2.SetPtEtaPhiE( c->photon.pt[j],
			 c->photon.eta[j],
			 c->photon.phi[j],
			 c->photon.energy[j]);
	vSum = v1+v2;
	invariantMass->Fill( vSum.M() );

      }
    }
  }

  invariantMass->Draw("E");
}


