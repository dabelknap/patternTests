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

#include "CTP7Tests/CreateRCTPatternsMC/plugins/L1RCTPatternProducer.h" 
#include "L1Trigger/RegionalCaloTrigger/interface/L1RCTProducer.h"
#include "CondFormats/L1TObjects/interface/L1RCTChannelMask.h"
#include "CondFormats/DataRecord/interface/L1RCTChannelMaskRcd.h"

L1RCTPatternProducer::L1RCTPatternProducer(const edm::ParameterSet& conf) :
 fileName(conf.getUntrackedParameter<std::string>("rctTestInputFile")),
 rctLookupTables(new L1RCTLookupTables), 
 fgEcalE(conf.getUntrackedParameter<int>("fgEcalE")),
 testName(conf.getUntrackedParameter<std::string>("testName","none")),
 randomPercent(conf.getUntrackedParameter<int>("randomPercent",5)),
 randomSeed(conf.getUntrackedParameter<int>("randomSeed",12345)),
 regionSums(conf.getUntrackedParameter<bool>("regionSums",true))
{
 produces<EcalTrigPrimDigiCollection>();
 //produces<EcalTrigPrimDigiCollection>("formatTCP");
 produces<HcalTrigPrimDigiCollection>();
 produces<HBHEDigiCollection>();
 produces<HFDigiCollection>();

 fileName = testName+"Input.txt";

 ofs.open(fileName.c_str());//, std::ios::app);
 if(!ofs)
 {
  std::cerr << "Could not create " << fileName << std::endl;
  exit(1);
 }
}

L1RCTPatternProducer::~L1RCTPatternProducer()
{
 //  if(rct != 0) delete rct;
 if(rctLookupTables != 0) delete rctLookupTables;
}

void L1RCTPatternProducer::updateConfiguration(const edm::EventSetup& eventSetup)
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

void L1RCTPatternProducer::updateFedVector(const edm::EventSetup& eventSetup, bool getFromOmds, int runNumber) // eventSetup apparently doesn't include run number: http://cmslxr.fnal.gov/lxr/source/FWCore/Framework/interface/EventSetup.h
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
L1RCTPatternProducer::produce(edm::Event& event,
  const edm::EventSetup& eventSetup)
{
 using namespace edm;

 updateConfiguration(eventSetup);
 int runNumber = event.id().run();
 updateFedVector(eventSetup,false,runNumber); // RUNINFO ONLY at beginning of run


 std::auto_ptr<EcalTrigPrimDigiCollection>
  ecalTPs(new EcalTrigPrimDigiCollection());
 //std::auto_ptr<EcalTrigPrimDigiCollection>
 // tcpEcalTPs(new EcalTrigPrimDigiCollection());
 std::auto_ptr<HcalTrigPrimDigiCollection> 
  hcalTPs(new HcalTrigPrimDigiCollection());
 ecalTPs->reserve(56*72); 
 hcalTPs->reserve(56*72+18*8);  // includes HF

 std::auto_ptr<HBHEDigiCollection> 
  digiHBHE(new HBHEDigiCollection());
 std::auto_ptr<HFDigiCollection> 
  digiHF(new HFDigiCollection());

 const int nEcalSamples = 1;   // we only use 1 sample for each
 const int nHcalSamples = 1;
 const int nHBHESamples = 1;
 const int nHFSamples = 1;



 long randomNum = 0;
 long NineBit = 0;
 long Overflow = 0;
 static int nEvents = 0;
 if(nEvents==0) {
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
  for(unsigned short iCrate = 0; iCrate < 18; iCrate++){
   for(unsigned short iCard = 0; iCard < 7; iCard++){    
    //  unsigned short ecal, fgbit, hcal, mubit;  
    int tt = (nEvents%3)*2;
    //int s = 0;

    fgbit =0;
    hcal = 0;
    mubit =0;
    ecal = 0;
    //int TowersHit = (int) rand()%32+1;
    //int Energy=(int)rand()%(0x80);
    Etot = 0;
    randomNum = rand()%100;
    if(randomNum>70&&randomNum<=90)
     NineBit = 1;
    if(randomNum>90)
     Overflow = 1;
    //int k = 0;

    //set up event 'bunches'
    bool eBunch4[16] = {false};
    bool eBunch8[8] = {false};
    int eNum4 = (nEvents-1)/4;
    int eNum8 = (nEvents-1)/8;
    eBunch4[eNum4]=true;
    eBunch8[eNum8]=true;

    cout<<testName<<endl;

    // tower numbered from 0-31
    for(unsigned short iTower = 0; iTower < 32; iTower++)
    {
     if(testName=="zeros") {
      fgbit = 0;
      hcal  = 0;
      mubit = 0;
      ecal  = 0;
     }
     else if(testName=="count") {
      fgbit = 0;
      hcal  = 0;
      mubit = 0;
      ecal  = 0;

      if(iTower==5 || iTower==21 )
       //ecal = nEvents;
       hcal = nEvents;
     }
     else if(testName=="testCard6"){  //checking card 6 sharing using energies and fg or quiet corner veto for non-iso
      fgbit = 0;  hcal  = 0; mubit = 0; ecal  = 0;

      cout<<"Here I am!! in"<<iTower<<endl;

      int walk6 = (nEvents%4)*4;
      if(iCard==6){

       if((eBunch4[0]&&iTower==walk6) ||
         (eBunch4[1]&&iTower==(31-walk6)))
        ecal = 0x10;
       else if((eBunch4[0]&&iTower==walk6+1) ||
         (eBunch4[1]&&iTower==(30-walk6)))
        ecal = 0xa;
       else if((eBunch4[0]&&iTower==(31-walk6))||
         (eBunch4[1]&&iTower==walk6))
        ecal = 0x03;

       if((eBunch4[2]&&iTower==walk6) ||
         (eBunch4[3]&&iTower==(31-walk6)))
        ecal = 0x10;
       else if((eBunch4[2]&&iTower==walk6+1)||
         (eBunch4[3]&&iTower==(30-walk6)))
        ecal = 0xa;
       else if((eBunch4[2]&&iTower==(31-walk6))||
         (eBunch4[3]&&iTower==walk6))
        ecal = 0x04;

       if((eBunch4[4]&&iTower==walk6) ||
         (eBunch4[5]&&iTower==(31-walk6)))
        ecal = 0x10;
       else if((eBunch4[4]&&iTower==walk6+1) ||
         (eBunch4[5]&&iTower==(30-walk6)))
        ecal = 0xa;
       else if((eBunch4[4]&&iTower==(31-walk6))||
         (eBunch4[5]&&iTower==walk6))
        ecal = 0x08;

       if((eBunch4[6]&&iTower==walk6) ||
         (eBunch4[7]&&iTower==(31-walk6)))
        ecal = 0x10;
       else if((eBunch4[6]&&iTower==walk6+1) ||
         (eBunch4[7]&&iTower==(30-walk6)))
        ecal = 0x04; // HACK 0x4;
       else if((eBunch4[6]&&iTower==(31-walk6))||
         (eBunch4[7]&&iTower==walk6))
        ecal = 0x05;

       if((eBunch4[8]&&iTower==walk6) ||
         (eBunch4[9]&&iTower==(31-walk6)))
        ecal = 0x10;
       else if((eBunch4[8]&&iTower==(31-walk6))||
         (eBunch4[9]&&iTower==walk6))
        ecal = 0x10;

       if((eBunch4[10]&&iTower==walk6) ||
         (eBunch4[11]&&iTower==(31-walk6)))
        ecal = 0x10;
       else if((eBunch4[10]&&iTower==(31-walk6))||
         (eBunch4[11]&&iTower==walk6))
        ecal = 0x20;
      }
      //put walking ones into hcal
      if(regionSums)
       walkHCAL(nEvents, iCard, iTower);
     }
     else { //no test specified
      cout << "NO TEST SPECIFIED!" <<endl;
      fgbit = 0;
      hcal  = 0;
      mubit = 0;
      ecal  = 0;
     }

     // Let's analyze this event

     unsigned eAddr = ecal + fgbit; // ecal * 2 + fgbit; 
     unsigned hAddr = hcal + mubit;// hcal * 2 + mubit; 

     unsigned long lutOutput = rctLookupTables->lookup(ecal, hcal, fgbit, iCrate, iCard, iTower);

     ofs
      << std::hex 
      << nEvents << "\t"
      << iCrate << "\t"
      << iCard << "\t"
      << iTower << "\t"
      << eAddr << "\t"
      << hAddr << "\t"
      << lutOutput 
      << std::dec 
      << std::endl;

     //  Add the item into emulutor
     int iEta = rctLookupTables->rctParameters()->calcIEta(iCrate,iCard,iTower);
     int iPhi = rctLookupTables->rctParameters()->calcIPhi(iCrate,iCard,iTower);
     // transform rct iphi coords into global coords

     if(ecal>0||hcal>0)
     cout<<iEta<<"   "<<iPhi<<"   "<<ecal<<"    "<<hcal<<"    "<<lutOutput<<endl;

     iPhi = ((72 + 18 - iPhi) % 72);
     if (iPhi == 0) {iPhi = 72;}
     int zSide = (iEta/abs(iEta));

     // args to detid are zside, type of tower, absieta, iphi
     // absieta and iphi must be between 1 and 127 inclusive



     //writeEcalFiles(TCC, iPhi, iEta); 


     //EcalTriggerPrimitiveDigi   ecalDigi;

     if(iCard < 4){
      EcalTriggerPrimitiveDigi 
       ecalDigi(EcalTrigTowerDetId(zSide, EcalBarrel, abs(iEta),
          iPhi));
      // 		EcalTriggerPrimitiveDigi 
      // 		  tcpEcalDigi(EcalTrigTowerDetId(zSide, EcalBarrel, abs(iEta),
      // 						 iPhi));

      ecalDigi.setSize(nEcalSamples);

      // last arg is 3-bit trigger tower flag, which we don't use
      // we only use 8-bit encoded et and 1-bit fg
      ecalDigi.setSample(0, EcalTriggerPrimitiveSample(ecal, 
         fgbit, 0));
      // 		tcpEcalDigi.setSample(0, EcalTriggerPrimitiveSample(ecal, 
      // 								    fgbit, 0));
      // 		tcpEcalTPs->push_back(tcpEcalDigi);
      ecalTPs->push_back(ecalDigi);  
     }
     else{
      EcalTriggerPrimitiveDigi 
       ecalDigi(EcalTrigTowerDetId(zSide, EcalEndcap, abs(iEta),
          iPhi));	
      // 		EcalTriggerPrimitiveDigi 
      // 		  tcpEcalDigi(EcalTrigTowerDetId(zSide, EcalBarrel, abs(iEta),
      // 						 iPhi));
      ecalDigi.setSize(nEcalSamples);

      // last arg is 3-bit trigger tower flag, which we don't use
      // we only use 8-bit encoded et and 1-bit fg
      ecalDigi.setSample(0, EcalTriggerPrimitiveSample(ecal, 
         fgbit, 0));
      // 		tcpEcalDigi.setSample(0, EcalTriggerPrimitiveSample(ecal, 
      // 								    fgbit, 0));
      // 		tcpEcalTPs->push_back(tcpEcalDigi);
      ecalTPs->push_back(ecalDigi);
     }


     HcalTriggerPrimitiveDigi
      hcalDigi(HcalTrigTowerDetId(iEta, iPhi));

     hcalDigi.setSize(nHcalSamples);

     // last two arg's are slb and slb channel, which we don't need
     hcalDigi.setSample(0, HcalTriggerPrimitiveSample(hcal,
        mubit,
        0, 0));
     //std::cout << hcalDigi << std::endl;
     hcalTPs->push_back(hcalDigi);

     //Used for making HCAL pattern files (input to HCAL instead of RCT)
     // HB valid DetIds: phi=1-72,eta=1-14,depth=1; phi=1-72,eta=15-16,depth=1-2

     // HE valid DetIds: phi=1-72,eta=16-17,depth=1; phi=1-72,eta=18-20,depth=1-2; 
     //                  phi=1-71(in steps of 2),eta=21-26,depth=1-2; phi=1-71(in steps of 2),eta=27-28,depth=1-3
     //                  phi=1-71(in steps of 2),eta=29,depth=1-2

     int depth=1;
     int adc = hcal;
     int capid = 0; //capacitor id (0-3), possibly anything?
     bool dv=true; bool er=false; //data valid and error
     int fiber = 0; int fiberchan = 0; //maybe need this???
     int aiEta = abs(iEta);

     // 	      if(aiEta<15) depth = 1;
     // 	      if(aiEta==16 || aiEta==15) depth = 1; //or 2
     HBHEDataFrame hbDataFrame(HcalDetId((HcalSubdetector) HcalBarrel, iEta, iPhi, depth));
     hbDataFrame.setSize(nHBHESamples);
     hbDataFrame.setSample(0,HcalQIESample(adc,capid,fiber,fiberchan,dv,er));

     // 	      if(aiEta==16 || aiEta==17) depth = 1;
     // 	      if(aiEta >=18 && aiEta <=26 || aiEta==29) depth = 1; // or 2
     // 	      if(aiEta==27 || aiEta==28) depth = 1; // or 2 or 3

     //MAYBE keep this???
     if(aiEta >= 21) {
      if(iPhi%2==0) {
       iPhi=iPhi-1;
       depth = 2;
      }
     }

     HBHEDataFrame heDataFrame(HcalDetId((HcalSubdetector) HcalEndcap, iEta, iPhi, depth));
     heDataFrame.setSize(nHBHESamples);
     heDataFrame.setSample(0,HcalQIESample(adc,capid,fiber,fiberchan,dv,er));

     if(aiEta<=16)
      digiHBHE->push_back(hbDataFrame);
     if(aiEta>16)
      digiHBHE->push_back(heDataFrame);
    }
   }
  }

  for (int i = 0; i < 18; i++) //Crate
  {	  
   for (int j = 0; j < 8; j++) //HF "tower"
   {	 
    fgbit = 0;
    hcal  = 0;
    mubit = 0;
    ecal  = 0;
    hf=0;
    if(regionSums){
     int tt = (nEvents%3)*2;
     if(testName=="testCrateNumber"&&nEvents>0) {
      hf = ((i<<3)|(j))&0xFF;
     }
     else if(testName=="random"&&nEvents>0) {
      if(rand()%100<randomPercent*2)
       hf = rand()%0xFF;
     }
     else if(testName=="zeros"||testName=="flooding" || testName=="test") 
      hf = 0x00;
     else if(testName=="max")
      hf = 0x00;
     else {
      if(nEvents==0)
       hf=0;
      else if((j<2||j==4||j==5)&&nEvents<32)
       hf = (1<<tt)&0xFF;
      else if((j<2||j==4||j==5)&&nEvents>=32)
       hf = (1<<(tt+1))&0xFF;
      else if((j>5||j==2||j==3)&&nEvents>=32)
       hf = (1<<tt)&0x3F;
      else if((j>5||j==2||j==3)&&nEvents<32)
       hf = (1<<(tt+1))&0xFF;
     }
     if(testName=="flooding12") {
      if(nEvents%12==0) {
       hf = rand()%0xFF;
       fgbit = 1;
      }
      else {
       hf = 0;
       fgbit = 0;
      }
     }
     unsigned long lutOutput = rctLookupTables->lookup(hf,i, 999, j);
     ofs
      << std::hex 
      << nEvents << "\t"
      << i << "\t"
      << "999" << "\t"
      << j << "\t"
      << "0" << "\t"
      << hf << "\t"
      << lutOutput // << "Wha happen'd"
      << std::dec 
      << std::endl;
    }

    // HF ieta: +- 29 through 32.  HF iphi: 1,5,9,13,etc.
    int hfIEta = (j%4)+29;
    if (i < 9)
    {
     hfIEta = hfIEta*(-1);
    }
    //int hfIPhi= i%9*2+j/4;
    //int hfIPhi = (i%9*2 + j/4)*4 + 1;
    int hfIPhi = i%9*8 + (j/4)*4 + 1;
    hfIPhi = (72 + 18 - hfIPhi)%72;

    HcalTriggerPrimitiveDigi
     hfDigi(HcalTrigTowerDetId(hfIEta, hfIPhi));
    hfDigi.setSize(1);
    hfDigi.setSample(0, HcalTriggerPrimitiveSample(hf,fgbit,0,0));
    hcalTPs->push_back(hfDigi);
    int subdet = 4; 
    int ieta = hfIEta; int iphi = hfIPhi; int depth = 1;
    HFDataFrame hfDataFrame(HcalDetId((HcalSubdetector) subdet, ieta, iphi, depth));
    int adc = hf;  
    int capid = 0; //capacitor id (0-3), possibly anything?
    bool dv=true; bool er=false; //data valid and error
    int fiber = 0; int fiberchan = 0; //maybe need this???
    hfDataFrame.setSize(nHFSamples);
    hfDataFrame.setSample(0,HcalQIESample(adc,capid,fiber,fiberchan,dv,er));
    digiHF->push_back(hfDataFrame);


   }
  }

 }
 event.put(ecalTPs);//,"formatTCP");
 //   event.put(tcpEcalTPs);
 event.put(hcalTPs);
 event.put(digiHBHE);
 event.put(digiHF);

 nEvents++;
}

/**********************************************
 *
 * Put known pattern into the first event
 *
 **********************************************/
void L1RCTPatternProducer::firstEvent(unsigned short iCrate,unsigned short iCard,unsigned short iTower) {
 fgbit =0;
 hcal = 0;
 mubit =0;
 ecal = 0;
 if(iTower==11) { //Easy to Find tower (i think)

  //if((iCrate%2)==0) {
  switch(iCard){
   case 0:
    ecal = 0x0d;
    break;
   case 1:
    ecal = 0x1e;
    break;
   case 2:
    ecal = 0x2a;
    break;  
   case 3:
    ecal = 0x3d;
    break;
   default:
    ecal = 0;
    break;
  }
  // } else {
  switch(iCard){
   case 0:
    ecal = 0x2e;
    break;
   case 1:
    ecal = 0x3f;
    break;
   case 2:
    ecal = 0x0b;
    break;
   case 3:
    ecal = 0x1e;
    break;
   default:
    ecal = 0;
    break;
    // }
 }

 }
}
/*****************************************
 *
 * Simple walking ones through hcal
 *
 ****************************************/
void L1RCTPatternProducer::walkHCAL(int nEvents, unsigned short iCard, unsigned short iTower) {
 //No tau bit stuff yet
 //Putting energy in hcal for jets
 //Emulator **and not the hardware** currently set up so that if any tower is saturated (0xff) the entire region is saturated (0x3ff) -- so the following code avoids saturating any towers, but also allows for a saturated region.

 //For HCAL link patterns need towers not next to each other in phi 
 //for endcap where phi divisions are two towers wide
 hcal = 0;
 int regID = 0;
 if(nEvents<32)
  if(iTower>=16) 
   regID = 1;
  else 
   regID = 0;
 else
  if(iTower < 16) 
   regID = 1;
  else 
   regID = 0;

 int s = (nEvents*2+regID)%12;

 if(s<10&&nEvents<64&&(iTower==0||iTower==17)){
  Etot = (1<<s)&0x3ff;
 }
 else if(s==10&&nEvents<60&&(iTower==0||iTower==17)) {
  Etot = 0x3ff;
 }
 else if(s==11||nEvents>=60)
  Etot = 0x0;

 if(s>=8 && (iTower == 11 || iTower == 13 || iTower == 15 || iTower == 27 || iTower == 29 || iTower == 31)){
  if(Etot >=0xff)
   hcal = 0xf0;
  else 
   hcal = Etot;
  Etot = Etot - hcal;
 }
 if((iTower == 3 || (iTower == 19) )&& Etot!=0) {
  if(Etot>=0xff)
   hcal = 0xf0;
  else
   hcal = rand()%(Etot+1);
  Etot=Etot-hcal;   
 }
 if((iTower == 12 || (iTower == 25 && iCard<6)) && Etot!=0) {
  if(Etot>=0xff)
   hcal = 0xf0;
  else
   hcal = Etot;
  Etot=Etot-hcal;
 }
 if(s<8 && (iTower == 17 || iTower == 31) && iCard==6)
  hcal = Etot/2;
 if((iTower == 17 ) && iCard==6){
  if(Etot>=0xff)
   hcal = 0xf0;
  else
   hcal = Etot;
  Etot = Etot-hcal;
 }
}

/****************************************
 *
 * Simple walking ones through hcal
 *
 ****************************************/
void L1RCTPatternProducer::walkZeroHCAL(int nEvents, unsigned short iCard, unsigned short iTower) {
 //No tau bit stuff yet
 //Putting energy in hcal for jets
 //Emulator **and not the hardware** currently set up so that if any tower is saturated (0xff) the entire region is saturated (0x3ff) -- so the following code avoids saturating any towers, but also allows for a saturated region.
 //cout<<"walkZeroHCAL " << nEvents << " card " << iCard << " tower " << iTower << endl;

 //For HCAL link patterns need towers not next to each other in phi 
 //for endcap where phi divisions are two towers wide
 hcal = 0;
 int regID = 0;
 if(nEvents<32) {
  if(iTower>=16) 
   regID = 1;
  else 
   regID = 0;
 }
 else 
  if(iTower < 16) 
   regID = 1;
  else 
   regID = 0;

 int s = (nEvents*2+regID)%12;
 if(s<12&&nEvents<64&&(iTower==0||iTower==17)){
  Etot = ~(1<<s)&0x3ff;
 }
 /*
    else if(s==10&&nEvents<60&&(iTower==0||iTower==16)) {
    Etot = 0x3ff;
    }
    else if(s==11||nEvents>=64)
    Etot = 0x0;
    */
 //if(iCard == 6)
 //cout << "Etot 0 " << Etot << endl;

 if((iTower == 11 || iTower == 13 || iTower == 15 || iTower == 27 || iTower == 29 || iTower == 31 )){
  if(Etot >=0xff)
   hcal = 0xf0;
  else 
   hcal = Etot;
  Etot = Etot - hcal;

 }
 //if(iCard == 6)
 //cout << "Etot 1 " << Etot << endl;
 if((iTower == 3 || (iTower == 19 )) && Etot!=0) {
  if(Etot>=0xff)
   hcal = 0xf0;
  else
   hcal = rand()%(Etot+1);
  Etot=Etot-hcal;   
 }
 //if(iCard == 6)
 //cout << "Etot 2 " << Etot << endl;
 if((iTower == 12 || (iTower == 25 && iCard<6)) && Etot!=0) {
  if(Etot>=0xff)
   hcal = 0xf0;
  else
   hcal = Etot;
  Etot=Etot-hcal;
 }
 //if(iCard == 6)
 //cout << "Etot 3 " << Etot << endl;
 if((iTower == 17) && iCard==6) {
  if(Etot>=0xff)
   hcal = 0xf0;
  else
   hcal = 0xf0;
  Etot=Etot-hcal;
 }
 //if(iCard == 6)
 //cout << "Etot 5 " << Etot << endl;
 //if(iCard == 6)
 //cout<<"walkZeroHCAL6 " << hcal << endl;
}

/***********************************************************
 *
 * Set up on/off to test bit switching on jet sum pins
 *
 ***********************************************************/
void L1RCTPatternProducer::jetSumPins(int nEvents, unsigned short iCard, unsigned short iTower, int num) {
 //No tau bit stuff yet
 //Putting energy in hcal for jets

 //For HCAL link patterns need towers not next to each other in phi 
 //for endcap where phi divisions are two towers wide

 hcal = 0;
 int oneZero = nEvents%2; 
 switch(num){
  case 0:
   if((iTower == 19 || iTower == 28 || iTower == 25) && iCard<6)
    hcal = (0xff)*(1-oneZero);
   if(iTower == 3 || iTower == 12 || iTower == 9)
    hcal = (0xff)*(oneZero);
   if((iTower == 17 || iTower == 31 || iTower == 21) && iCard==6) //16->17
    hcal = (0xff)*(1-oneZero);
   break;
  case 1:
   if((iTower == 19  || iTower == 28 ) && iCard<6)
    hcal = 0xE6>>oneZero;
   if(iTower == 3 || iTower == 12)
    hcal = 0xE6>>oneZero;
   if((iTower == 17 || iTower == 31) && iCard==6)//16->17
    hcal = 0xE6>>oneZero;
   if((iTower == 25 && iCard<6) || iTower == 9 || (iCard==6 && iTower == 21))
    hcal = 0xDE>>oneZero;
   break;
  case 2:
   if((iTower == 19 || iTower == 28 || iTower == 25) && iCard<6)
    hcal = (0xff)*(1-oneZero);
   if(iTower == 3 || iTower == 12 || iTower == 9)
    hcal = (0xff)*(oneZero);
   if((iTower == 17 || iTower == 31 || iTower == 21) && iCard==6)
    hcal = (0xff)*(1-oneZero);
   break;
  case 3:
   if((iTower == 19  || iTower == 28 ) && iCard<6)
    hcal = 0xE6>>oneZero;
   if(iTower == 3 || iTower == 12)
    hcal = 0xE6>>oneZero;
   if((iTower == 17 || iTower == 31) && iCard==6)
    hcal = 0xE6>>oneZero;
   if((iTower == 25 && iCard<6) || iTower == 9 || (iCard==6 && iTower == 21))
    hcal = 0xDE>>oneZero;
   break;
 }
}

// void L1RCTPatternProducer::writeEcalFiles(ofstream& of, int TCC) {

//   std::string ecalFileName;
//   ecalFileName = "data/" + testName + "/ecal_tcc" + TCC + ".txt";

//   of.open(ecalFileName.c_str());
//   if(!of)
//     {
//       std::cerr << "Could not create " << fileName << std::endl;
//       exit(1);
//     }



// 
//
//
//

//define this as a plug-in
DEFINE_FWK_MODULE(L1RCTPatternProducer);

