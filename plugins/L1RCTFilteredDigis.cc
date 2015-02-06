// -*- C++ -*-
//
// Package:    TestProducer/L1RCTFilteredDigis
// Class:      L1RCTFilteredDigis
// 
/**\class L1RCTFilteredDigis L1RCTFilteredDigis.cc TestProducer/L1RCTFilteredDigis/plugins/L1RCTFilteredDigis.cc

   Description: [one line class summary]

   Implementation:
   [Notes on implementation]
*/
//
// Original Author:  Sridhara Dasu
//         Created:  Sat, 04 Oct 2014 05:07:24 GMT
//
//


// system include files

#include <memory>
#include <iostream>
#include <string>

using namespace std;

// Framework stuff

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// RCT data formats

#include "DataFormats/L1CaloTrigger/interface/L1CaloEmCand.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"

// Scan in file

#include <fstream>

//
// class declaration
//

class L1RCTFilteredDigis : public edm::EDProducer {
public:
  explicit L1RCTFilteredDigis(const edm::ParameterSet&);
  ~L1RCTFilteredDigis();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() override;
  virtual void produce(edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;      
  virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
  virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

  // ----------member data ---------------------------

  edm::InputTag rctDigisLabel;
  std::vector<int> crateNumber;
  std::vector<int> cardNumber;
  bool includeHF;

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
L1RCTFilteredDigis::L1RCTFilteredDigis(const edm::ParameterSet& iConfig):
  rctDigisLabel(iConfig.getParameter<edm::InputTag>("rctDigisLabel")),
  crateNumber(iConfig.getParameter<std::vector<int> >("crateNumber")),
  cardNumber(iConfig.getParameter<std::vector<int> >("cardNumber")),
  includeHF(iConfig.getParameter<bool>("includeHF"))
{
  //register your products
  produces<L1CaloEmCollection>();
  produces<L1CaloRegionCollection>();

}


L1RCTFilteredDigis::~L1RCTFilteredDigis()
{
}



//
// member functions
//

// ------------ method called to produce the data  ------------
void
L1RCTFilteredDigis::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // Take six ints at a time from even and odd fibers, assumed to be neighboring
  // channels to make rctInfo buffer, and from that make rctEMCands and rctRegions

  std::auto_ptr<L1CaloEmCollection> newRctEMCands(new L1CaloEmCollection);
  std::auto_ptr<L1CaloRegionCollection> newRctRegions(new L1CaloRegionCollection);

  edm::Handle<L1CaloEmCollection> rctEmCands;
  edm::Handle<L1CaloRegionCollection> rctRegions;

  L1CaloEmCollection::const_iterator em;
  L1CaloRegionCollection::const_iterator rgn;

  iEvent.getByLabel(rctDigisLabel,rctEmCands);
  iEvent.getByLabel(rctDigisLabel,rctRegions);


  for (size_t cand = 0; cand < rctEmCands->size(); ++cand) {
//  for(em=rctEmCands->begin(); em!=rctEmCands->end(); em++){
    L1CaloEmCand em=(*rctEmCands)[cand]; 
    unsigned short card = (unsigned short) em.rctCard();
    //unsigned short rank = (unsigned short) em.rank();
    unsigned short crate = (unsigned short) em.rctCrate();

    bool selectCrate=false, selectCard=false;
    if (crateNumber.size()==1 && crateNumber[0]==-1) selectCrate=true;
    else { for(unsigned int i=0; i<crateNumber.size(); i++){
              if(crate==crateNumber[i]) selectCrate=true;
            }
    }
    if (cardNumber.size()==1 && cardNumber[0]==-1) selectCard=true;
    else { for(unsigned int i=0; i<cardNumber.size(); i++){
              if(card==cardNumber[i]) selectCard=true;
            }
    }
    if (!selectCrate || !selectCard) continue;
  
    newRctEMCands->push_back(em);
  }

  for (size_t cand = 0; cand < rctRegions->size(); ++cand) {
//  for(rgn=rctRegions->begin(); rgn!=rctRegions->end(); rgn++){
    L1CaloRegion rgn=(*rctRegions)[cand];
    //unsigned short rank = (unsigned short) rgn.et();
    unsigned short card = (unsigned short) rgn.rctCard();
    unsigned short crate = (unsigned short)rgn.rctCrate();

    if(!includeHF && card==999) continue;  //check this in MC

    bool selectCrate=false, selectCard=false;
    if (crateNumber.size()==1 && crateNumber[0]==-1) selectCrate=true;
    else { for(unsigned int i=0; i<crateNumber.size(); i++){
              if(crate==crateNumber[i]) selectCrate=true;
            }
    }
    if (cardNumber.size()==1 && cardNumber[0]==-1) selectCard=true;
    else { for(unsigned int i=0; i<cardNumber.size(); i++){
              if(card==cardNumber[i]) selectCard=true;
            }
    }
    if (!selectCrate || !selectCard) continue;

    newRctRegions->push_back(rgn);
  }

  iEvent.put(newRctEMCands);
  iEvent.put(newRctRegions);

}

// ------------ method called once each job just before starting event loop  ------------
void 
L1RCTFilteredDigis::beginJob()
{
  cout << "L1RCTFilteredDigis::beginJob()" << endl;
}

// ------------ method called once each job just after ending the event loop  ------------
void 
L1RCTFilteredDigis::endJob() {
  cout << "L1RCTFilteredDigis::endJob()" << endl;
}

// ------------ method called when starting to processes a run  ------------

void
L1RCTFilteredDigis::beginRun(edm::Run const&, edm::EventSetup const&)
{
  cout << "L1RCTFilteredDigis::beginRun()" << endl;
}

 
// ------------ method called when ending the processing of a run  ------------

void
L1RCTFilteredDigis::endRun(edm::Run const&, edm::EventSetup const&)
{
  cout << "L1RCTFilteredDigis::endRun()" << endl;
}

 
// ------------ method called when starting to processes a luminosity block  ------------

void
L1RCTFilteredDigis::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
  cout << "L1RCTFilteredDigis::beginLuminosityBlock()" << endl;
}

 
// ------------ method called when ending the processing of a luminosity block  ------------

void
L1RCTFilteredDigis::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
  cout << "L1RCTFilteredDigis::endLuminosityBlock()" << endl;
}
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
L1RCTFilteredDigis::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setComment("New collection of digis, filtered");
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1RCTFilteredDigis);
