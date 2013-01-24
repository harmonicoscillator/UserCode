#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <TMath.h>
#include <TH1D.h>
#include <TLorentzVector.h>
#include <iostream>

#include "../HiForest_V2_latest/hiForest.h"
#include "alexUtils.h"


void photonMass()
{
  TString data = "/mnt/hadoop/cms/store/user/luck/pA2013_forests/PA2013_HiForest_Express_r210614_autoforest_v1.root";

  HiForest *forest = new HiForest(data.Data());

  TH1D *hMass = new TH1D("hMass",";Invariant Mass (GeV)",100,0,200);

  int nentries = forest->GetEntries();
  for (Long64_t jentry = 0 ; jentry < nentries; jentry++)
  {
    if (jentry % 1000 == 0)
      std::cout << jentry << " / " << nentries << std::endl;

    forest->GetEntry(jentry);
    for (int j = 0; j < forest->photon.nPhotons; j++)
    {
      for(int i = j+1; i < forest->photon.nPhotons; i++)
      {
	TLorentzVector v1, v2, vSum;
	v1.SetPtEtaPhiE( forest->photon.pt[i],
			 forest->photon.eta[i],
			 forest->photon.phi[i],
			 forest->photon.energy[i]);
	v2.SetPtEtaPhiE( forest->photon.pt[j],
			 forest->photon.eta[j],
			 forest->photon.phi[j],
			 forest->photon.energy[j]);
	vSum = v1+v2;
	hMass->Fill( vSum.M() );
      }
    }
  }

  hMass->Draw("E");
}
