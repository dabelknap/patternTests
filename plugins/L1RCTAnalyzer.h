// -*- C++ -*-
//
// Package:    L1RCTAnalyzer
// Class:      L1RCTAnalyzer
//
/**\class L1RCTAnalyzer L1RCTAnalyzer.cc src/L1RCTAnalyzer/src/L1RCTAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  pts/47
//         Created:  Thu Jul 13 21:38:08 CEST 2006
// $Id: L1RCTAnalyzer.h,v 1.1 2009/02/12 13:24:44 grogg Exp $
//
//


// system include files
#include <memory>
#include <iostream>
#include <fstream>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
//#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

#include "TH1F.h"
#include "TH2F.h"

//
// class declaration
//

class L1RCTAnalyzer : public edm::EDAnalyzer {
public:
  explicit L1RCTAnalyzer(const edm::ParameterSet&);
  ~L1RCTAnalyzer();
  
  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
private:
  // ----------member data ---------------------------
  std::string m_HepMCProduct;
  bool showEmCands;
  bool showRegionSums;
  bool limitTo64;  
  std::string testName;
  edm::InputTag ecalDigisLabel;
  edm::InputTag hcalDigisLabel;
  edm::InputTag rctDigisLabel;
  std::ofstream ofs;
  std::string fileName;
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
