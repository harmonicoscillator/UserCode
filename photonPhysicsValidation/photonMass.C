#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <TMath.h>
#include <TH1D.h>
#include <TLorentzVector.h>
#include <iostream>

//#include "../HiForest_V2_latest/hiForest.h"
//#include "alexUtils.h"


void photonMass()
{
  //TString data = "/mnt/hadoop/cms/store/user/luck/pA2013_forests/PA2013_HiForest_Express_r210614_autoforest_v1.root";

  TString data = "root://eoscms//eos/cms/store/group/phys_heavyions/velicanu/forest/PA2013_HiForest_PromptRecofirstPR_forestv51.root";

  //HiForest *forest = new HiForest(data.Data());

  TFile *infile = TFile::Open(data);

  TTree *photon = (TTree*)infile->Get("multiPhotonAnalyzer/photon");
  TTree *hltTree = (TTree*)infile->Get("hltanalysis/HltTree");

  Int_t nPhotons;
  Float_t pt[126];
  Float_t eta[126];
  Float_t phi[126];
  Float_t energy[126];
  Float_t hadronicOverEm[126];
  Float_t sigmaIetaIeta[126];
  Float_t scEta[126];

  Int_t HLT_PAPhoton15_Photon10_NoCaloIdVL_v1;

  photon->SetBranchAddress("nPhotons", &nPhotons);
  photon->SetBranchAddress("pt", pt);
  photon->SetBranchAddress("eta", eta);
  photon->SetBranchAddress("phi", phi);
  photon->SetBranchAddress("energy", energy);
  photon->SetBranchAddress("hadronicOverEm",hadronicOverEm);
  photon->SetBranchAddress("sigmaIetaIeta",sigmaIetaIeta);
  photon->SetBranchAddress("scEta",scEta);

  hltTree->SetBranchAddress("HLT_PAPhoton15_Photon10_NoCaloIdVL_v1",&HLT_PAPhoton15_Photon10_NoCaloIdVL_v1);

  TH1D *hMass = new TH1D("hMass",";Invariant Mass (GeV)",40,0,200);

  int nentries = photon->GetEntries();
  for (Long64_t jentry = 0 ; jentry < nentries; jentry++)
  {
    if (jentry % 1000 == 0)
      std::cout << jentry << " / " << nentries << std::endl;

    photon->GetEntry(jentry);
    hltTree->GetEntry(jentry);

    if(!HLT_PAPhoton15_Photon10_NoCaloIdVL_v1)
      continue;
    
    for (int j = 0; j < nPhotons; j++)
    {
      if(hadronicOverEm[j] > 0.05)
	continue;
      if((scEta[j] < 1.479) && (sigmaIetaIeta[j] > 0.011) )
	continue;
      if((scEta[j] > 1.479) && (sigmaIetaIeta[j] > 0.035) )
	continue;
      
      for(int i = j+1; i < nPhotons; i++)
      {
	if(hadronicOverEm[i] > 0.05)
	  continue;
	if((scEta[i] < 1.479) && (sigmaIetaIeta[i] > 0.011) )
	continue;
	if((scEta[i] > 1.479) && (sigmaIetaIeta[i] > 0.035) )
	continue;

	if( (pt[i] < 20) && (pt[j] < 20) )
	  continue;

	
	
	TLorentzVector v1, v2, vSum;
	v1.SetPtEtaPhiE( pt[i],
			 eta[i],
			 phi[i],
			 energy[i]);
	v2.SetPtEtaPhiE( pt[j],
			 eta[j],
			 phi[j],
			 energy[j]);
	vSum = v1+v2;
	hMass->Fill( vSum.M() );
      }
    }
  }

  hMass->Draw("E");
}
