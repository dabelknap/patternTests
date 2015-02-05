// -*- C++ -*-
//
// Package:    L1RCTSaveInputV2
// Class:      L1RCTSaveInputV2
//
/**\class L1RCTSaveInputV2 L1RCTSaveInputV2.cc src/L1RCTSaveInputV2/src/L1RCTSaveInputV2.cc

 Description: Saves the input event from TPGs for loading 
              simulated events in hardware

 Implementation: Kind of kludgy -- should think of a better way in future

*/
//
// Original Author:  Sridhara Dasu
//         Created:  Tue Jul 31 17:10:13 CEST 2007
//
//

#include <memory>
#include <iostream>
#include <fstream>

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/MakerMacros.h"

class L1RCTLookupTables;
class L1RCT;

class L1RCTSaveInputV2 : public edm::EDAnalyzer {
public:
  explicit L1RCTSaveInputV2(const edm::ParameterSet&);
  ~L1RCTSaveInputV2();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  void updateFedVector(const edm::EventSetup& eventSetup, bool getFromOmds, int runNumber);
  void updateConfiguration(const edm::EventSetup& eventSetup);

private:
  std::string fileName;
  L1RCTLookupTables* rctLookupTables;
  L1RCT* rct;
  bool useEcal;
  bool useHcal;
  edm::InputTag ecalDigisLabel;
  edm::InputTag hcalDigisLabel;
  bool useDebugTpgScales;
  std::ofstream ofs;
};
