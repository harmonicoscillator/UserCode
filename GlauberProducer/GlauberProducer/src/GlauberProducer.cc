// -*- C++ -*-
//
// Package:    GlauberProducer
// Class:      GlauberProducer
// 
/**\class GlauberProducer GlauberProducer.cc GlauberProducer/GlauberProducer/src/GlauberProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Richard Alexander Barbieri
//         Created:  Wed Jan  2 13:31:36 EST 2013
// $Id$
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

// actual things
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/HiGenData/interface/GenHIEvent.h"

#include "HepMC/HeavyIon.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "../runglauber_v1.5.cc"

//
// class declaration
//

class GlauberProducer : public edm::EDProducer {
   public:
      explicit GlauberProducer(const edm::ParameterSet&);
      ~GlauberProducer();

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
GlauberProducer::GlauberProducer(const edm::ParameterSet& iConfig)
{
  //register your products
  produces<edm::GenHIEvent>();
  //now do what ever other initialization is needed
}


GlauberProducer::~GlauberProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
GlauberProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   //book
   //if(!(pdt.isValid())) iSetup.getData(pdt);
 
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
   //double TotEnergy = 0; // Total energy bym

   //run Glauber
   TString Pb = "Pb";
   TGlauberMC *mcg = new TGlauberMC(Pb, Pb, 42); //last argument is Xsection?
   mcg->SetMinDistance(0);
   mcg->Run(1);

   b = mcg->GetB();
   npart = mcg->GetNpart();
   ncoll = mcg->GetNcoll();

   TNtuple *glauberResults = mcg->GetNtuple();
   glauberResults->SetBranchAddress("Psi2PART", &phi);
   glauberResults->SetBranchAddress("Ecc2PART", &ecc);
   glauberResults->GetEntry(0);
   

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
   delete mcg;
  
}

// ------------ method called once each job just before starting event loop  ------------
void 
GlauberProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GlauberProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
GlauberProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
GlauberProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
GlauberProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
GlauberProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GlauberProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GlauberProducer);


