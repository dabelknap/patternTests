// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
//#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"

#include "CTP7Tests/CreateRCTPatternsMC/plugins/L1RCTAnalyzer.h"

using std::string;
using std::cout;
using std::endl;

//
// constructors and destructor
//
L1RCTAnalyzer::L1RCTAnalyzer(const edm::ParameterSet& iConfig) :
 showEmCands(iConfig.getUntrackedParameter<bool>("showEmCands",true)),
 showRegionSums(iConfig.getUntrackedParameter<bool>("showRegionSums",true)),
 limitTo64(iConfig.getUntrackedParameter<bool>("limitTo64",false)),
 testName(iConfig.getUntrackedParameter<std::string>("testName","none")),
 ecalDigisLabel(iConfig.getParameter<edm::InputTag>("ecalDigisLabel")),
 hcalDigisLabel(iConfig.getParameter<edm::InputTag>("hcalDigisLabel")),
 rctDigisLabel(iConfig.getParameter<edm::InputTag>("rctDigisLabel")),
 crateNumber(iConfig.getParameter<std::vector<int> >("crateNumber")),
 cardNumber(iConfig.getParameter<std::vector<int> >("cardNumber")),
 includeHF(iConfig.getParameter<bool>("includeHF"))
{  
 //cout << "testAnalyzer" << endl;
 fileName = testName+".txt";
 if(testName=="ttbar")
  ofs.open(fileName.c_str(), std::ios::app); //for ttbar running
 else  
  ofs.open(fileName.c_str());
 if(!ofs)
 {
  std::cerr << "Could not create " << fileName << std::endl;
  exit(1);
 }
}


L1RCTAnalyzer::~L1RCTAnalyzer()
{

 // do anything here that needs to be done at destruction time
 // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
 void
L1RCTAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 //cout << "testAnalyzer analyze" << endl;
 using namespace edm;

 // as in L1GctAnalyzer.cc
 Handle<L1CaloEmCollection> rctEmCands;
 Handle<L1CaloRegionCollection> rctRegions;
 Handle<EcalTrigPrimDigiCollection> ecalColl;
 Handle<HcalTrigPrimDigiCollection> hcalColl;

 L1CaloEmCollection::const_iterator em;
 L1CaloRegionCollection::const_iterator rgn;
 EcalTrigPrimDigiCollection::const_iterator ecal;
 HcalTrigPrimDigiCollection::const_iterator hcal;

 iEvent.getByLabel(rctDigisLabel, rctEmCands);
 iEvent.getByLabel(rctDigisLabel, rctRegions);
 iEvent.getByLabel(ecalDigisLabel, ecalColl);
 iEvent.getByLabel(hcalDigisLabel, hcalColl);

 static int numEvents = 0;
 if(limitTo64==true&&numEvents<63||limitTo64==false)
 {   
  if(testName=="ttbar")
   ofs << endl << "Event " << numEvents+1 << endl;
  else
  {         ofs << endl << "Event " << numEvents << endl;
   //cout << "Events" << endl; 
  }      numEvents++;
  if(showEmCands)
  {
   ofs << endl << "L1 RCT EmCand objects" << endl;
  }
  for (em=rctEmCands->begin(); em!=rctEmCands->end(); em++){
   if ((*em).bx() == 0)
   {
    {
     if ((*em).rank() > 0 || (5==5))
     {
      ofs <<endl;
      unsigned short rgnPhi = 999;
      unsigned short rgn = (unsigned short) (*em).rctRegion();
      unsigned short card = (unsigned short) (*em).rctCard();
      unsigned short crate = (unsigned short) (*em).rctCrate();

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


      if (card == 6)
      {
       rgnPhi = rgn;
      }
      else if (card < 6)
      {
       rgnPhi = (card % 2);
      }
      else 
      {
       ofs << "rgnPhi not assigned (still " << rgnPhi << ") -- Weird card number! " << card ;
      }
      unsigned short phi_bin = ((crate % 9) * 2) + rgnPhi;
      short eta_bin = (card/2) * 2 + 1;
      if (card < 6)
      {
       eta_bin = eta_bin + rgn;
      }
      if (crate < 9)
      {
       eta_bin = -eta_bin;
      }
      if(selectCrate&&selectCard) ofs <<  "rank: " <<(*em).rank() <<  "  eta_bin: " << eta_bin << "  phi_bin: " << phi_bin << ".  crate: " << crate << "  card: " << card << "  region: " << rgn << ".  isolated: " << (*em).isolated();
      else ofs <<"rank: " <<"0"<<"  eta_bin: " << eta_bin << "  phi_bin: " << phi_bin << ".  crate: " << crate << "  card: " << card << "  region: " << rgn << ".  isolated: " << (*em).isolated();


     }
    }
   }
  }
  if(showEmCands)
  {
   ofs << endl;
  }

  bool header = false;
  if(showRegionSums)
  {
   //cout <<"show reg sum cands" << endl; 
   ofs << "Regions" << endl;
  }
  for (rgn=rctRegions->begin(); rgn!=rctRegions->end(); rgn++)
  {
   if ((*rgn).bx() == 0)
   {
    if(showRegionSums)
    {	  
     if(header==false) {

      ofs << "Et\to/f\tf/g\ttau\tmip\tqt\tRCTcrt\tRCTcrd\tRCTrgn\tRCTeta\tRCTphi\tGCTeta\tGCTphi\t" << endl;
      header = true;
     }
     //cout << /* "(Analyzer)\n" << */ (*rgn) << endl;
    }
   }	     
   if(showRegionSums)
   {

    short crate=(*rgn).rctCrate();
    short card=(*rgn).rctCard();

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


    if((*rgn).rctCard()<7){

     if(selectCrate&&selectCard)
      ofs << (*rgn).et() << "\t" << (*rgn).overFlow() << "\t" 
       << (*rgn).fineGrain() <<  "\t" << (*rgn).tauVeto() << "\t" 
       << (*rgn).mip()
       <<  "\t" << (*rgn).quiet()  << "\t" << (*rgn).rctCrate() 
       << "\t" << (*rgn).rctCard() << "\t" 
       << (*rgn).rctRegionIndex() << "\t" 
       << (*rgn).rctEta() << "\t" << (*rgn).rctPhi() << "\t" 
       << (*rgn).gctEta() << "\t" << (*rgn).gctPhi() << endl;
     else      ofs << "0" << "\t" << "0" << "\t"<< "0" << "\t" << "0" << "\t" << "0" << "\t"<<"1"
      << "\t" << (*rgn).rctCrate()
       << "\t" << (*rgn).rctCard() << "\t"
       << (*rgn).rctRegionIndex() << "\t"
       << (*rgn).rctEta() << "\t" << (*rgn).rctPhi() << "\t"
       << (*rgn).gctEta() << "\t" << (*rgn).gctPhi() << endl;


    }
   }
  }


  ofs << "HF" << endl;
  header = false;

  for (rgn=rctRegions->begin(); rgn!=rctRegions->end(); rgn++)
  {
   if(header==false&&showRegionSums) {

    ofs << "Et\to/f\tf/g\ttau\tmip\tqt\tRCTcrt\tRCTcrd\tRCTrgn\tRCTeta\tRCTphi\tGCTeta\tGCTphi\t" << endl;
    header = true;
   }	


   if(showRegionSums)
   {

    short crate=(*rgn).rctCrate();

    bool selectCrate=false;
    if (crateNumber.size()==1 && crateNumber[0]==-1) selectCrate=true;
    else { for(unsigned int i=0; i<crateNumber.size(); i++){
     if(crate==crateNumber[i]) selectCrate=true;
    }
    }

    if(includeHF&&selectCrate){
     if((*rgn).rctCard()>7){
      ofs << (*rgn).et() << "\t" << (*rgn).overFlow() << "\t" 
       << (*rgn).fineGrain() <<  "\t" << (*rgn).tauVeto() << "\t" 
       << (*rgn).mip()
       <<  "\t" << (*rgn).quiet()  << "\t" << (*rgn).rctCrate() 
       << "\t" << (*rgn).rctCard() << "\t" 
       << (*rgn).rctRegionIndex() << "\t" 
       << (*rgn).rctEta() << "\t" << (*rgn).rctPhi() << "\t" 
       << (*rgn).gctEta() << "\t" << (*rgn).gctPhi() << endl;
     }}
    else{
     ofs <<"0"<<"\t"<<"0"<<"\t"<<"0"<<"\t"<<"0"<<"\t"<<"0"<<"\t"<<"1"<<"\t"
      << (*rgn).rctCrate()      << "\t" << (*rgn).rctCard() << "\t"
      << (*rgn).rctRegionIndex() << "\t"
      << (*rgn).rctEta() << "\t" << (*rgn).rctPhi() << "\t"
      << (*rgn).gctEta() << "\t" << (*rgn).gctPhi() << endl;
    }
   }
  }

  if(showRegionSums)
  {
   ofs << endl;
  }
 }
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1RCTAnalyzer);

