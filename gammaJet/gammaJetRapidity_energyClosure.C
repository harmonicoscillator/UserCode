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

#include "commonUtility.h"


void gammaJetRapidity_energyClosure()
{
  TH1::SetDefaultSumw2();
  
  TString file;

  file = "/mnt/hadoop/cms/store/user/luck/pA2013_MC/HiForest2_QCDPhoton30_5020GeV_100k.root";

  HiForest *c = new HiForest(file, "Forest", cPPb, true);
  c->InitTree();

  TProfile *mcClosure = new TProfile("mcClosure",";p_{T}^{gen};<p_{T}^{reco}/p_{T}^{gen}>",40,0,300);

  Int_t notMatched = 0;

  //loop over events in each file
  int nentries = c->GetEntries();
  for(int jentry = 0; jentry<nentries; jentry++)
  {
    if (jentry% 1000 == 0)  {
      printf("%d / %d\n",jentry,nentries);
    }
    
    c->GetEntry(jentry);
    
    //event selection (mc only)
    if( !( c->skim.phfPosFilter1 && c->skim.phfNegFilter1 && c->skim.phltPixelClusterShapeFilter && c->skim.pprimaryvertexFilter) )
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

    if(c->photon.isGenMatched[leadingIndex])
    {
      Double_t genPt = c->photon.genMatchedPt[leadingIndex];
      Double_t ratio = leadingPt/genPt;

      mcClosure->Fill(genPt,ratio);

    }
    else
    {
      notMatched++;
    }
  }

  mcClosure->Draw();
  mcClosure->GetYaxis()->SetRangeUser(0.,2.);
  TLine *line = new TLine(0, 1, 300, 1);
  line->Draw();
  drawText("Leading Photons",0.75,0.75);

  printf("Number of not matched leading photons: %d\n",notMatched);
}


