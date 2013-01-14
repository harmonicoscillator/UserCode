// -*- C++ -*-
//
// Package:    FlowCalculator
// Class:      FlowCalculator
// 
/**\class FlowCalculator FlowCalculator.cc GlauberProducer/FlowCalculator/src/FlowCalculator.cc

   Description: [one line class summary]

   Implementation:
   [Notes on implementation]
*/
//
// Original Author:  Richard Alexander Barbieri
//         Created:  Fri Jan 11 14:36:19 EST 2013
// $Id: FlowCalculator.cc,v 1.1 2013/01/14 20:08:21 richard Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//real things
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/HiGenData/interface/GenHIEvent.h"

#include "HepMC/HeavyIon.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "TMath.h"

//
// class declaration
//

class FlowCalculator : public edm::EDProducer {
public:
  explicit FlowCalculator(const edm::ParameterSet&);
  ~FlowCalculator();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
      
  virtual void beginRun(edm::Run&, edm::EventSetup const&);
  virtual void endRun(edm::Run&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

  // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
FlowCalculator::FlowCalculator(const edm::ParameterSet& iConfig)
{
  //register your products
  produces<edm::GenHIEvent>();
  
  //now do what ever other initialization is needed
  
}


FlowCalculator::~FlowCalculator()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
FlowCalculator::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
/* This is an event example
//Read 'ExampleData' from the Event
Handle<ExampleData> pIn;
iEvent.getByLabel("example",pIn);

//Use the ExampleData to create an ExampleData2 which 
// is put into the Event
std::auto_ptr<ExampleData2> pOut(new ExampleData2(*pIn));
iEvent.put(pOut);
*/

/* this is an EventSetup example
//Read SetupData from the SetupRecord in the EventSetup
ESHandle<SetupData> pSetup;
iSetup.get<SetupRecord>().get(pSetup);
*/

  Handle<reco::GenParticleCollection> pNucleons;
  iEvent.getByLabel("GlauberProducer",pNucleons);

  //std::cout << pNucleons->at(0).vx() << std::endl;

  double b = -1;
  int npart = -1;
  int ncoll = 0;
  int nhard = 0;
  Float_t phi = 0;
  Float_t ecc = -1;
 
  int nCharged = 0;
  int nChargedMR = 0;
  int nChargedPtCut = 0; // NchargedPtCut bym
  int nChargedPtCutMR = 0; // NchargedPtCutMR bym
 
  double meanPt = 0;
  double meanPtMR = 0;
  double EtMR = 0; // Normalized of total energy bym

  // Int_t fNA = 208;
  // Int_t fNB = 208;
  // Double_t bgen = b;


  // calc results for the given event
  Double_t fNpart=0;
  Double_t fNcoll=0;
  Double_t fMeanX2=0;
  Double_t fMeanY2=0;
  Double_t fMeanXY=0;
  Double_t fMeanXParts=0;
  Double_t fMeanYParts=0;
  Double_t fMeanXSystem=0;
  Double_t fMeanYSystem=0;
  // Double_t fMeanX_A=0;
  // Double_t fMeanY_A=0;
  // Double_t fMeanX_B=0;
  // Double_t fMeanY_B=0;

  Double_t     fSx2=0;            //Variance of x of wounded nucleons
  Double_t     fSy2=0;            //Variance of y of wounded nucleons
  Double_t     fSxy=0;            //Covariance of x and y of wounded nucleons

  Double_t     fEcc2PART=0;       //v1.4 participant eccentricity from Eqn(4) from PRC81, 054905 (2010)
  Double_t     fPsi2PART=0;       //v1.4 minor eccentricity axis Eqn(5) from PRC81, 054905 (2010)
  Double_t     fEcc3PART=0;       //v1.4 participant triangularity from Eqn(7) from PRC81, 054905 (2010)
  Double_t     fPsi3PART=0;       //v1.4 minor triangularity axis Eqn(8) from PRC81, 054905 (2010)

  Double_t     fEcc1PART=0;       //v1.5 
  Double_t     fPsi1PART=0;       //v1.5 
  Double_t     fEcc4PART=0;       //v1.5 
  Double_t     fPsi4PART=0;       //v1.5 
  Double_t     fEcc5PART=0;       //v1.5 
  Double_t     fPsi5PART=0;       //v1.5 
  Double_t     fEcc6PART=0;       //v1.5 
  Double_t     fPsi6PART=0;       //v1.5 

  Int_t  totNucleons = pNucleons->size();
  for (Int_t i = 0; i < totNucleons; i++) {
    reco::GenParticle nucleon = pNucleons->at(i);
    Double_t xA = nucleon.vx();
    Double_t yA = nucleon.vy();
    fMeanXSystem  += xA;
    fMeanYSystem  += yA;
    // fMeanX_A  += xA;
    // fMeanY_A  += yA;

    if(nucleon.status() != 0) {
      fNpart++;
      fMeanXParts  += xA;
      fMeanYParts  += yA;
      fMeanX2 += xA * xA;
      fMeanY2 += yA * yA;
      fMeanXY += xA * yA;
      fNcoll += nucleon.status();
    }
  }

  fNcoll /= 2; //This is double counted since both nuclei are lumped together


  if (fNpart>0) {
    fMeanXParts /= fNpart;
    fMeanYParts /= fNpart;
    fMeanX2 /= fNpart;
    fMeanY2 /= fNpart;
    fMeanXY /= fNpart;
  } else {
    fMeanXParts = 0;
    fMeanYParts = 0;
    fMeanX2 = 0;
    fMeanY2 = 0;
    fMeanXY = 0;
  }
   
// //  if(fAN+fBN>0) {
     // fMeanXSystem /= (208 + 208);
     // fMeanYSystem /= (208 + 208);
//   // } else {
//   //   fMeanXSystem = 0;
//   //   fMeanYSystem = 0;
//   // }
//   // if(fAN>0) {
//     fMeanX_A /= fAN;
//     fMeanY_A /= fAN;
//   // } else {
//   //   fMeanX_A = 0;
//   //   fMeanY_A = 0;
//   // }

//   // if(fBN>0) {
//     fMeanX_B /= fBN;
//     fMeanY_B /= fBN;
//   // } else {
//   //   fMeanX_B = 0;
//   //   fMeanY_B = 0;
//   // }

  fSx2 = fMeanX2-(fMeanXParts*fMeanXParts);
  fSy2 = fMeanY2-(fMeanYParts*fMeanYParts);
  fSxy = fMeanXY-fMeanXParts*fMeanYParts;

  // fB_MC = bgen;

  //----------------------------v1.4------------------------------
  //v1.4 Put here calculations for triangularity


  if (fNpart>0) {

    Double_t rsquared=0;
    Double_t phipart=0;
    Double_t avgrsqcos1phi=0;
    Double_t avgrsqsin1phi=0;
    Double_t avgrsqcos2phi=0;
    Double_t avgrsqsin2phi=0;
    Double_t avgrsqcos3phi=0;
    Double_t avgrsqsin3phi=0;
    Double_t avgrsqcos4phi=0;
    Double_t avgrsqsin4phi=0;
    Double_t avgrsqcos5phi=0;
    Double_t avgrsqsin5phi=0;
    Double_t avgrsqcos6phi=0;
    Double_t avgrsqsin6phi=0;
    Double_t avgrsquared=0;
     
    for (Int_t i = 0; i < totNucleons; i++) {
      reco::GenParticle nucleon = pNucleons->at(i);
      if(nucleon.status() != 0) {
	Double_t xAshift = nucleon.vx()-fMeanXParts;
	Double_t yAshift = nucleon.vy()-fMeanYParts;
	rsquared = xAshift*xAshift + yAshift*yAshift;
	phipart = TMath::ATan2(yAshift,xAshift);
	avgrsqcos1phi += rsquared*TMath::Cos(phipart);
	avgrsqsin1phi += rsquared*TMath::Sin(phipart);
	avgrsqcos2phi += rsquared*TMath::Cos(2*phipart);
	avgrsqsin2phi += rsquared*TMath::Sin(2*phipart);
	avgrsqcos3phi += rsquared*TMath::Cos(3*phipart);
	avgrsqsin3phi += rsquared*TMath::Sin(3*phipart);
	avgrsqcos4phi += rsquared*TMath::Cos(4*phipart);
	avgrsqsin4phi += rsquared*TMath::Sin(4*phipart);
	avgrsqcos5phi += rsquared*TMath::Cos(5*phipart);
	avgrsqsin5phi += rsquared*TMath::Sin(5*phipart);
	avgrsqcos6phi += rsquared*TMath::Cos(6*phipart);
	avgrsqsin6phi += rsquared*TMath::Sin(6*phipart);
	avgrsquared += rsquared;
      }
    }

    avgrsqcos1phi /= fNpart;
    avgrsqsin1phi /= fNpart;
    avgrsqcos2phi /= fNpart;
    avgrsqsin2phi /= fNpart;
    avgrsqcos3phi /= fNpart;
    avgrsqsin3phi /= fNpart;
    avgrsqcos4phi /= fNpart;
    avgrsqsin4phi /= fNpart;
    avgrsqcos5phi /= fNpart;
    avgrsqsin5phi /= fNpart;
    avgrsqcos6phi /= fNpart;
    avgrsqsin6phi /= fNpart;
    avgrsquared /= fNpart;

    fEcc1PART=TMath::Sqrt(avgrsqcos1phi*avgrsqcos1phi+avgrsqsin1phi*avgrsqsin1phi)/avgrsquared;
    fPsi1PART=(TMath::ATan2(avgrsqsin1phi,avgrsqcos1phi)+TMath::Pi())/1.;
    fEcc2PART=TMath::Sqrt(avgrsqcos2phi*avgrsqcos2phi+avgrsqsin2phi*avgrsqsin2phi)/avgrsquared;
    fPsi2PART=(TMath::ATan2(avgrsqsin2phi,avgrsqcos2phi)+TMath::Pi())/2.;
    fEcc3PART=TMath::Sqrt(avgrsqcos3phi*avgrsqcos3phi+avgrsqsin3phi*avgrsqsin3phi)/avgrsquared;
    fPsi3PART=(TMath::ATan2(avgrsqsin3phi,avgrsqcos3phi)+TMath::Pi())/3.;
    fEcc4PART=TMath::Sqrt(avgrsqcos4phi*avgrsqcos4phi+avgrsqsin4phi*avgrsqsin4phi)/avgrsquared;
    fPsi4PART=(TMath::ATan2(avgrsqsin4phi,avgrsqcos4phi)+TMath::Pi())/4.;
    fEcc5PART=TMath::Sqrt(avgrsqcos5phi*avgrsqcos5phi+avgrsqsin5phi*avgrsqsin5phi)/avgrsquared;
    fPsi5PART=(TMath::ATan2(avgrsqsin5phi,avgrsqcos5phi)+TMath::Pi())/5.;
    fEcc6PART=TMath::Sqrt(avgrsqcos6phi*avgrsqcos6phi+avgrsqsin6phi*avgrsqsin6phi)/avgrsquared;
    fPsi6PART=(TMath::ATan2(avgrsqsin6phi,avgrsqcos6phi)+TMath::Pi())/6.;

  } else {

    fEcc1PART=-1000;
    fPsi1PART=-1000;
    fEcc2PART=-1000;
    fPsi2PART=-1000;
    fEcc3PART=-1000;
    fPsi3PART=-1000;
    fEcc4PART=-1000;
    fPsi4PART=-1000;
    fEcc5PART=-1000;
    fPsi5PART=-1000;
    fEcc6PART=-1000;
    fPsi6PART=-1000;

  }

  
  //EccRP:EccPART:SPART:SPART4:S12RP_Npart
  Float_t v[34]; 
  v[17] = (fSy2-fSx2)/(fSy2+fSx2);  //v1.2  Eqn A3 of PRC77, 014906 (2008)
  v[18] = TMath::Sqrt((fSy2-fSx2)*(fSy2-fSx2)+4.*fSxy*fSxy)/(fSy2+fSx2);  //v1.2  Eqn A6 of PRC77, 014906 (2008)
  v[19] = fSx2*fSy2-fSxy; //v1.3  Eqn A8 of PRC77, 014906 (2008)
  if(v[19]<0) {
    v[19]=0.0;
  }
  else {
    v[19] = TMath::Pi()*TMath::Sqrt(v[19]); //v1.3  Eqn A8 of PRC77, 014906 (2008)
  }
  v[20] = 4.0*v[19]; //v1.3  Four times Eqn A8 of PRC77, 014906 (2008)
  v[21] = 4.0*TMath::Pi()*TMath::Sqrt(fMeanX2)*TMath::Sqrt(fMeanY2); //v1.3  Eur.Phys.J.C66:173,2010 but with participants and relative to original Reaction Plane


  npart = fNpart;
  ncoll = fNcoll;
  phi = fPsi2PART;
  ecc = fEcc2PART;
  
  //push
  std::auto_ptr<edm::GenHIEvent> pGenHI(new edm::GenHIEvent(b,
							    npart,
							    ncoll,
							    nhard,
							    phi, 
							    ecc,
							    nCharged,
							    nChargedMR,
							    meanPt,
							    meanPtMR,   
							    EtMR, 
							    nChargedPtCut,
							    nChargedPtCutMR
					  ));
 
  iEvent.put(pGenHI);

 
}

// ------------ method called once each job just before starting event loop  ------------
void 
FlowCalculator::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
FlowCalculator::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
FlowCalculator::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
FlowCalculator::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
FlowCalculator::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
FlowCalculator::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
FlowCalculator::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(FlowCalculator);
