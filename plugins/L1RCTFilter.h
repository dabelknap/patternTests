#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include <vector>
#include "FWCore/Framework/interface/MakerMacros.h"

  class L1RCTFilter : public edm::EDFilter
  {
  public:
    explicit L1RCTFilter(edm::ParameterSet const&);
    virtual ~L1RCTFilter();
 
    virtual bool filter(edm::Event& e, edm::EventSetup const& c);
    void endJob();
    bool checkEdges(double phiMin, double phiMax, double etaMin, double etaMax, const CaloTowerCollection* calotowers);

  private:
    int particleID;
    //int numberOfCrates;
    double phiMinMin;
    double phiMaxMax;
    //double phiMin2;
    //double phiMax2;
    //double etaMin;
    //double etaMax;
    std::vector<int> crateNumber;
    double pTMin;
    bool edgesZeroed;
  };
