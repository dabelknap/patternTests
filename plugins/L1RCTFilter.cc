/*************************************************************************
 *
 *   Program:  L1RCTFilter.cc
 *   Author:   K. Grogg, M. Weinberg
 *   Date:     1 May, 2007
 *   Revision: 25 July, 2007
 *   Purpose:  Filter out desired events when running NCrateTest.
 *             Selected as true only those events that have a significant 
 *             amount of energy within a specified group of crates. 
 *             Takes as parameters particle id (usu. and electron, id = 11),
 *             the minimum and maximum phi values for the group of crates,
 *             the minimum pt of the particle, wether the edges of the 
 *             crates should be zeroed (usu. true, unless all 18 crates)
 *             and the set of crate numbers to be used.
 *
 *************************************************************************/

#include "CTP7Tests/CreateRCTPatternsMC/plugins/L1RCTFilter.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/OutputModule.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"

#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include <string>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <vector>

using namespace std;
using namespace edm;
using namespace reco;

using std::cerr;
using std::cout;
using std::endl;

L1RCTFilter::L1RCTFilter(const edm::ParameterSet& ps) :
  particleID(ps.getUntrackedParameter<int>("particleIDValue",11)),
  phiMinMin(ps.getUntrackedParameter<double>("phiMinValue",0)),
  phiMaxMax(ps.getUntrackedParameter<double>("phiMaxValue",6.28318531)),
  pTMin(ps.getUntrackedParameter<double>("pTMinValue",0)),
  edgesZeroed(ps.getUntrackedParameter<bool>("edgesZeroed",true))
  {
 vector<int> defpid1;
   defpid1.push_back(0);
  crateNumber=ps.getUntrackedParameter< vector<int> >("crateNumber",defpid1);
    cout << "filter!" << endl;
  }
     
L1RCTFilter::~L1RCTFilter()
  {
  }
/************************************************************************
 *
 * Main class. Loops over all generated particles in the event. 
 * If particle is the specified type and has more than the min pt,
 * then loops over the selected crates.  If particle is in one of
 * the selected crates, then event is set to good. 
 * Finally, check for edge energy, if none, then event if still good.
 *
 ************************************************************************/ 
bool L1RCTFilter::filter(edm::Event& iEvent, edm::EventSetup const&)
{
  std::cout << "filtering..." << "particles " << particleID << std::endl;
  edm::Handle< vector<reco::GenParticle> >genParticlesHandle;
  iEvent.getByLabel("genParticles", genParticlesHandle);
 
 
//  Handle<CaloTowerCollection> calotowers;
//  iEvent.getByType(calotowers);
  
  bool goodEvent = false;
  bool noEdges = true;
  
  double phiMin = 0;
  double phiMax = 3.1416;
  double phiMin2 = 0;
  double phiMax2 = 3.1416;
  double etaMin = 0;
  double etaMax = 0;
  double etaMinMin = 5;
  double etaMaxMax = -5;
  
	  //loop over all crates to select the phi boundaries
	  for(unsigned int i = 0; i < crateNumber.size(); i++)
	    {
	      switch(crateNumber[i])
		{
		case 0: phiMin = 0.8727 ; phiMax = 1.5708; 
		  break;
		case 1: phiMin = 0.1745 ; phiMax = 0.8727;
		  break;
		case 2: phiMin = -.5236 ; phiMax = 0.1745;
		  break;
		case 3: phiMin = -1.2217; phiMax = -.5236;
		  break;
		case 4: phiMin = -1.9199; phiMax = -1.2217; 
		  break;
		case 5: phiMin = -2.6180 ; phiMax = -1.9199 ; 
		  break;
		case 6: phiMin = -3.1415; phiMax = -2.618; phiMin2 = 2.9671; phiMax2 = 3.1415;
		  break;
		case 7: phiMin = 2.2689; phiMax = 2.9671;
		  break;
		case 8: phiMin = 1.5708; phiMax = 2.2689;
		  break;
		case 9: phiMin = 0.8727 ; phiMax = 1.5708; 
		  break;
		case 10: phiMin = 0.1745; phiMax = 0.8727; 
		  break;
		case 11: phiMin = -.5236; phiMax = 0.1745;
		  break;
		case 12: phiMin = -1.2217; phiMax = -.5236; 
		  break;
		case 13: phiMin = -1.9199; phiMax = -1.2217; 
		  break;
		case 14: phiMin = -2.6180; phiMax = -1.9199; 
		  break;
		case 15: phiMin = -3.1415; phiMax = -2.618; phiMin2 = 2.9671; phiMax2 = 3.1415;
		  break;
		case 16: phiMin = 2.2689 ; phiMax = 2.9671;   
		  break;
		case 17: phiMin = 1.5708; phiMax = 2.2689; 
		  break;
		}
	      
	      //set min/max eta
	      if(crateNumber[i] <9) {
		etaMin = -5; etaMax = 0; 
	      }
	      else {
		etaMin = 0; etaMax = 5;
	      }
	      
	      if(etaMin < etaMinMin) etaMinMin = etaMin;
	      if(etaMax > etaMaxMax) etaMaxMax = etaMax;
       
            } 


  loop over all particles in event
    for(size_t i = 0; i < genParticlesHandle->size(); ++ i )
        {
              const reco::GenParticle& p = (*genParticlesHandle)[i];
  
                    //if particle matches chosen particle ID 
                          //(or any particle if particleID is set to 999
                                if((p.pdgId()== particleID || p.pdgId() ==( -1 * particleID) || particleID== 999) && p.pt() >= pTMin)
                                        {
  

	      //check if particle is within crate
	      if(crateNumber[i] != 6 && crateNumber[i] !=15) {
		if((p.phi() >= phiMin && p.phi() <= phiMax) && p.eta() >= etaMin && p.eta() <= etaMax) {goodEvent = true;}
	      } 
	      // for crates 6 & 15, on -180 to 180 phi border
	      else {
		if((p.phi() >= phiMin && p.phi() <= phiMax)||(p.phi() >= phiMin2 && p.phi() <= phiMax2) && p.eta() >= 0 && p.eta() <= 5) {goodEvent = true;}
	      }
	    }
	}
    }
  //if energy has been found in a selected crate, check the edges
  if(goodEvent)
    noEdges = true; //checkEdges(phiMinMin,phiMaxMax,etaMinMin,etaMaxMax,calotowers.product());
  
  //if energy is in selected crates and passes edge test, then return true
  if(goodEvent && noEdges) {
    goodEvent = true;
    cout << "good!" << endl;
  }
  else goodEvent = false;
  
  return goodEvent;  
}

void L1RCTFilter::endJob()
{
}

/***************************************************************************
 *
 * If there is energy in a selected crate, checks for energy on the 
 * edges of the group of crates. If there is energy on the edge
 * of the group of selected crate and energy on the edge of the next crate 
 * (adjacent to the selected group, but not part of it) in the same eta 
 * region, then the hardware won't respond properly -- doesn't know there
 * are other crates -- so don't allow this event. 
 * (Won't catch all problems -- if there is corner energy it can affect the
 * iso/non-iso status of a candidate.) 
 *
 ***************************************************************************/
bool L1RCTFilter::checkEdges(double phiMin, double phiMax, double etaMin, double etaMax,const CaloTowerCollection* calotowers)
{
  bool ret = true; 
  vector<double> etaList1(20);
  vector<double> etaList2(20);
  etaList1[0]=99; etaList2[0] = 99;
  
  if( edgesZeroed == true)
    {
      //first find the eta values where there is energy on the edge of 
      //the group of selected crates
      int i = 0; int j = 0;
      for(CaloTowerCollection::const_iterator cal = calotowers->begin(); cal != calotowers->end(); ++cal)
	{
	  double CTphi = cal->phi();
	  double CTeta = cal->eta();
	  double emEt = cal->emEt();
	  
	  if(CTphi >=(phiMin - 0.0875) && CTphi <= phiMin)
	    {
	      if(emEt > 0.0) {
		etaList1[i]=CTeta; 
		//cout << "upper edge, eta: " << CTeta << endl; 
		i++;}	      
	    }
	  if(CTphi <= (phiMax + 0.0875) && CTphi >= phiMax)
	    {
	      if(emEt > 0.0) {
		etaList2[j]=CTeta; 
		//cout << "lower edge, eta: " << CTeta << endl; 
		j++;}
	    }
	}
      
      //use the arrays of eta values found above to see if adjacent
      //towers in non-selected crates have energy
      //if they do,return false, and event won't be counted
      for(CaloTowerCollection::const_iterator cal = calotowers->begin(); cal != calotowers->end(); ++cal)
	{
	  double CTphi = cal->phi();
	  double CTeta = cal->eta();
	  double emEt = cal->emEt();
	  double hadEt = cal->hadEt();
	  if(CTphi <=(phiMin + 0.0875) && CTphi >= phiMin && (emEt > 0.0 || hadEt > 0.0))
	    {
	      for(unsigned int k=0; k<etaList1.size();k++) 
	     	{
	     	  if(etaList1[k] == CTeta) 
		    {
		      ret = false;
		    }
		}
	    }
	  
	  if(CTphi >= (phiMax - 0.0875) && CTphi <= phiMax && (emEt > 0.0 || hadEt > 0.0))
	    {
	      for(unsigned int k=0; k<etaList2.size();k++) 
	      	{
		  if(etaList2[k]==CTeta) 
		    {
		      ret = false; 
		    }
		}
	    }
	}
    }
  return ret;
}

DEFINE_FWK_MODULE(L1RCTFilter);
