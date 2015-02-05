#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using std::ostream;
using std::cout;
using std::cerr;
using std::endl;

#include <iomanip>

#include "CTP7Tests/CreateRCTPatternsMC/plugins/L1RCTSaveInputV2.h" 
#include "L1Trigger/RegionalCaloTrigger/interface/L1RCTProducer.h"
#include "CondFormats/L1TObjects/interface/L1RCTChannelMask.h"
#include "CondFormats/DataRecord/interface/L1RCTChannelMaskRcd.h"

L1RCTSaveInputV2::L1RCTSaveInputV2(const edm::ParameterSet& conf) :
  fileName(conf.getUntrackedParameter<std::string>("rctTestInputFile")),
  rctLookupTables(new L1RCTLookupTables),
  rct(new L1RCT(rctLookupTables)),
  useEcal(conf.getParameter<bool>("useEcal")),
  useHcal(conf.getParameter<bool>("useHcal")),
  ecalDigisLabel(conf.getParameter<edm::InputTag>("ecalDigisLabel")),
  hcalDigisLabel(conf.getParameter<edm::InputTag>("hcalDigisLabel")),
  useDebugTpgScales(conf.getParameter<bool>("useDebugTpgScales"))
{
  ofs.open(fileName.c_str(), std::ios::app);
  if(!ofs)
    {
      std::cerr << "Could not create " << fileName << std::endl;
      exit(1);
    }
}

L1RCTSaveInputV2::~L1RCTSaveInputV2()
{
  if(rct != 0) delete rct;
  if(rctLookupTables != 0) delete rctLookupTables;
}

void L1RCTSaveInputV2::updateConfiguration(const edm::EventSetup& eventSetup)
{
 // Refresh configuration information every event
 // Hopefully, this does not take too much time
 // There should be a call back function in future to
 // handle changes in configuration
 // parameters to configure RCT (thresholds, etc)
 edm::ESHandle<L1RCTParameters> rctParameters;
 eventSetup.get<L1RCTParametersRcd>().get("", rctParameters);
 const L1RCTParameters* r = rctParameters.product();

 //SCALES

 // energy scale to convert eGamma output
 edm::ESHandle<L1CaloEtScale> emScale;
 eventSetup.get<L1EmEtScaleRcd>().get("", emScale);
 const L1CaloEtScale* s = emScale.product();

 // get energy scale to convert input from ECAL
 edm::ESHandle<L1CaloEcalScale> ecalScale;
 eventSetup.get<L1CaloEcalScaleRcd>().get("", ecalScale);
 const L1CaloEcalScale* e = ecalScale.product();

 // get energy scale to convert input from HCAL
 edm::ESHandle<L1CaloHcalScale> hcalScale;
 eventSetup.get<L1CaloHcalScaleRcd>().get("", hcalScale);
 const L1CaloHcalScale* h = hcalScale.product();

 // set scales
 rctLookupTables->setEcalScale(e);
 rctLookupTables->setHcalScale(h);

 rctLookupTables->setRCTParameters(r);
 rctLookupTables->setL1CaloEtScale(s);
}

void L1RCTSaveInputV2::updateFedVector(const edm::EventSetup& eventSetup, bool getFromOmds, int runNumber) // eventSetup apparently doesn't include run number: http://cmslxr.fnal.gov/lxr/source/FWCore/Framework/interface/EventSetup.h
{
 // list of RCT channels to mask
 edm::ESHandle<L1RCTChannelMask> channelMask;
 eventSetup.get<L1RCTChannelMaskRcd>().get(channelMask);
 const L1RCTChannelMask* cEs = channelMask.product();


 // list of Noisy RCT channels to mask
 edm::ESHandle<L1RCTNoisyChannelMask> hotChannelMask;
 eventSetup.get<L1RCTNoisyChannelMaskRcd>().get(hotChannelMask);
 const L1RCTNoisyChannelMask* cEsNoise = hotChannelMask.product();
 rctLookupTables->setNoisyChannelMask(cEsNoise);



 //Update the channel mask according to the FED VECTOR
 L1RCTChannelMask* fedUpdatedMask = new L1RCTChannelMask();
 // copy a constant object
 for (int i = 0; i < 18; i++)
 {
  for (int j = 0; j < 2; j++)
  {
   for (int k = 0; k < 28; k++)
   {
    fedUpdatedMask->ecalMask[i][j][k] = cEs->ecalMask[i][j][k];
    fedUpdatedMask->hcalMask[i][j][k] = cEs->hcalMask[i][j][k] ;
   }
   for (int k = 0; k < 4; k++)
   {
    fedUpdatedMask->hfMask[i][j][k] = cEs->hfMask[i][j][k];
   }
  }
 }

 rctLookupTables->setChannelMask(fedUpdatedMask);

}

void
L1RCTSaveInputV2::analyze(const edm::Event& event,
                        const edm::EventSetup& eventSetup)
{
 using namespace edm;

 updateConfiguration(eventSetup);
 int runNumber = event.id().run();
 updateFedVector(eventSetup,false,runNumber); // RUNINFO ONLY at beginning of run

 edm::Handle<EcalTrigPrimDigiCollection> ecal;
 edm::Handle<HcalTrigPrimDigiCollection> hcal;
 event.getByLabel(ecalDigisLabel, ecal);
 event.getByLabel(hcalDigisLabel, hcal);
 EcalTrigPrimDigiCollection ecalColl;
 HcalTrigPrimDigiCollection hcalColl;
 if (ecal.isValid()) { ecalColl = *ecal; }
 if (hcal.isValid()) { hcalColl = *hcal; }


 rct->digiInput(ecalColl, hcalColl);
 static int nEvents = 0;
 if(nEvents == 0)
 {
  ofs
   << "Crate = 0-17" << std::endl
   << "Card = 0-7 within the crate" << std::endl
   << "Tower = 0-31 covers 4 x 8 covered by the card" << std::endl
   << "EMAddr(0:8) = EMFGBit(0:0)+CompressedEMET(1:8)" << std::endl
   << "HDAddr(0:8) = HDFGBit(0:0)+CompressedHDET(1:8) - note: HDFGBit(0:0) is not part of the hardware LUT address" << std::endl
   << "LutOut(0:17)= LinearEMET(0:6)+HoEFGVetoBit(7:7)+LinearJetET(8:16)+ActivityBit(17:17)" << std::endl
   << "Event" << "\t"
   << "Crate" << "\t"
   << "Card" << "\t"
   << "Tower" << "\t"
   << "EMAddr" << "\t"
   << "HDAddr" << "\t"
   << "LUTOut"
   << std::endl;
 }
 if(nEvents < 64)
 {
  for(unsigned short iCrate = 0; iCrate < 18; iCrate++)
  {
   for(unsigned short iCard = 0; iCard < 7; iCard++)
   {
    // tower numbered from 0-31
    for(unsigned short iTower = 0; iTower < 32; iTower++)
    {
     unsigned short ecal = rct->ecalCompressedET(iCrate, iCard, iTower);
     unsigned short hcal = rct->hcalCompressedET(iCrate, iCard, iTower);
     unsigned short fgbit = rct->ecalFineGrainBit(iCrate, iCard, iTower);
     unsigned short mubit = rct->hcalFineGrainBit(iCrate, iCard, iTower);
     unsigned long lutOutput = rctLookupTables->lookup(ecal, hcal, fgbit, iCrate, iCard, iTower);
     ofs
      << std::hex 
      << nEvents << "\t"
      << iCrate << "\t"
      << iCard << "\t"
      << iTower << "\t"
      << ecal * 2 + fgbit << "\t"
      << hcal * 2 + mubit << "\t"
      << lutOutput
      << std::dec
      << std::endl;

//      if(ecal!=0 || hcal!=0) 
//      if(iTower<4 || iTower > 28) 

/*      cout
      << std::hex
      << nEvents << "\t"
      << iCrate << "\t"
      << iCard << "\t"
      << iTower << "\t"
      << ecal * 2 + fgbit << "\t"
      << hcal * 2 + mubit << "\t"
      << lutOutput
      << std::dec
      << std::endl;
*/
    }
   }
  }
      for (int i = 0; i < 18; i++) //Crate
        {
          for (int j = 0; j < 8; j++) //HF "tower"
            {
              unsigned short hf = rct->hfCompressedET(i,j);
              unsigned short hfFG = rct->hfFineGrainBit(i,j);
              unsigned long lutOutput = rctLookupTables->lookup(hf,i,999,j);
              ofs
                << std::hex
                << nEvents << "\t"
                << i << "\t"
                << "999" << "\t"
                << j << "\t"
                << "0" << "\t"
                << hf*2+hfFG<< "\t"
                << lutOutput // << "Wha happen'd"
                << std::dec
                << std::endl;

/*              cout
                << std::hex
                << nEvents << "\t"
                << i << "\t"
                << "999" << "\t"
                << j << "\t"
                << "0" << "\t"
                << hf*2+hfFG<< "\t"
                << lutOutput // << "Wha happen'd"
                << std::dec
                << std::endl;
*/

            }
        }
 }
 nEvents++;
}
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1RCTSaveInputV2);

