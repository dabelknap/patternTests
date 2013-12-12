// -*- C++ -*-
//
// Package:    L1RCTAnalyzeFilter
// Class:      L1RCTAnalyzeFilter
// 
/**\class L1RCTAnalyzeFilter L1RCTAnalyzeFilter.cc L1Trigger/L1RCTAnalyzeFilter/src/L1RCTAnalyzeFilter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Kira Suzanne Grogg
//         Created:  Wed Apr 16 13:51:56 CEST 2008
// $Id: L1RCTAnalyzeFilter.cc,v 1.1 2009/02/12 13:24:43 grogg Exp $
//
//


// system include files
#include <memory>

#include "L1Trigger/RegionalCaloTrigger/interface/L1RCTAnalyzeFilter.h"
#include "FWCore/Framework/interface/OutputModule.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
L1RCTAnalyzeFilter::L1RCTAnalyzeFilter(const edm::ParameterSet& iConfig)

{
  fOutputFileName = iConfig.getUntrackedParameter<string>("HistOutFile"); 
}


L1RCTAnalyzeFilter::~L1RCTAnalyzeFilter()
{
   
}

// ------------ method called once each job just before starting event loop  ------------
void 
L1RCTAnalyzeFilter::beginJob(const edm::EventSetup&)
{
  hOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" );

  //ntupleL1EMObj = new TNtuple("L1EmObj","data","et:eta:phi");
  //ntupleL1EMObjNiso = new TNtuple("L1EmObjNiso","data","pt:eta:phi");
  //ntupleL1JetObj = new TNtuple("L1JetObj","data","et:eta:phi");
  //ntupleL1MissEt = new TNtuple("L1MissEt","data","etMiss:etTotal:etHad");
  ntupleEcalDigis = new TNtuple("EcalDigis","data","nEvents:et:eta:phi");
  ntupleHcalDigis = new TNtuple("HcalDigis","data","nEvents:et:eta:phi");
  ntupleRgnCands= new TNtuple("RgnCands","data","nEvents:et:card:crate");
  ntupleEmCands= new TNtuple("EmCands","data","nEvents:et:card:crate");
  cout << "beginJob" << endl;

  nEvents = 0;
  
}

// ------------ method called to for each event  ------------
void
L1RCTAnalyzeFilter::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

    Handle<EcalTrigPrimDigiCollection> ecalD;
    Handle<HcalTrigPrimDigiCollection> hcalD;
    iEvent.getByLabel("ecalTriggerPrimitiveDigis",ecalD);
    iEvent.getByLabel("hcalTriggerPrimitiveDigis",hcalD);
    
    for(EcalTrigPrimDigiCollection::const_iterator itr = ecalD->begin(); itr!=ecalD->end();++itr)
      {
	if(itr->compressedEt()>0)
	  ntupleEcalDigis->Fill(nEvents,itr->compressedEt(),itr->id().ieta(),itr->id().iphi());
      }
    for(HcalTrigPrimDigiCollection::const_iterator itr = hcalD->begin(); itr!=hcalD->end();++itr)
      {
	if(itr->SOI_compressedEt()>0)
	  ntupleHcalDigis->Fill(nEvents,itr->SOI_compressedEt(),itr->id().ieta(),itr->id().iphi());
      }
    
  Handle<L1CaloEmCollection> rctEmCands;
  Handle<L1CaloRegionCollection> rctRegions;
  
  L1CaloEmCollection::const_iterator em;
  L1CaloRegionCollection::const_iterator rgn;
  
  iEvent.getByType(rctEmCands);
  iEvent.getByType(rctRegions);
  int count = 0;
  for(em=rctEmCands->begin(); em!=rctEmCands->end(); em++){
    unsigned short card = (unsigned short) em->rctCard();
    unsigned short rank = (unsigned short) em->rank();
    unsigned short crate = (unsigned short) em->rctCrate();
    if(rank>0) {
      ntupleEmCands->Fill(nEvents,rank,card,crate);
      count++;
    }
  }
  for(rgn=rctRegions->begin(); rgn!=rctRegions->end(); rgn++){
    unsigned short rank = (unsigned short) (*rgn).et();
    unsigned short card = (unsigned short) (*rgn).rctCard();
    unsigned short crate = (unsigned short) (*rgn).rctCrate();
    if(card < 8 && rank>0)
      ntupleRgnCands->Fill(nEvents,rank,card,crate);
  }
  
}

// ------------ method called once each job just after ending the event loop  ------------
void 
L1RCTAnalyzeFilter::endJob() 
{
  hOutputFile->Write();
  hOutputFile->Close();
  cout << "endjob " << endl;
  return;
}

//define this as a plug-in
//DEFINE_FWK_MODULE(L1RCTAnalyzeFilter);
