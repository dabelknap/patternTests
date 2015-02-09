import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryIdeal_cff')
process.load('L1Trigger.L1TCalorimeter.L1TCaloStage1_PPFromRaw_cff')

# Select the Message Logger output you would like to see:
process.load('FWCore.MessageService.MessageLogger_cfi')

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag.connect = cms.string('frontier://FrontierProd/CMS_COND_31X_GLOBALTAG')
process.GlobalTag.globaltag = cms.string('POSTLS162_V2::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(64)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#"file:/hdfs/store/user/laura/SinglePi0Pt/SinglePi0Pt40/SinglePi0Pt40-0000.root" 
 
    "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/00309507-AB75-E311-AB10-0025905A60B2.root"
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/007939EF-8075-E311-B675-0025905938AA.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/007FC5FC-7775-E311-83F4-00304867BED8.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/00861A49-B175-E311-8185-003048678B06.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/00A8B114-9675-E311-AF99-0025905A48B2.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/00DF0DA9-EB75-E311-A8BD-001BFCDBD1BC.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/00F65CD5-E075-E311-8873-0025905A6132.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/020064E5-3175-E311-874B-002590593872.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/020A6FC5-B583-E311-8D71-0025905A609A.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/0263E88C-8E75-E311-8216-002590593878.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/02678931-F975-E311-B662-003048678DD6.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/0298259D-B175-E311-8034-00261894386D.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/02A85E15-D775-E311-A4B3-0025905A60F4.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/02CAF241-8175-E311-9512-003048FFD79C.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/02F82E20-8475-E311-81F0-002618943957.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/040574A6-A475-E311-870A-003048678F62.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/041C1CED-F375-E311-BDA3-0025905964BE.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/0450435C-2F75-E311-95A0-0025905A48F0.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/04971BA0-4975-E311-8850-002618943980.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/04BCA7E3-B375-E311-ABDD-00304867BFF2.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/04C6E895-DE75-E311-A51E-0025905938B4.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/04DE9627-4B75-E311-850F-0025905964C2.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/04E46CA3-5C75-E311-A069-002618943874.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/060A3FE0-B675-E311-B78A-003048679182.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/0617FBBD-B575-E311-A4AB-003048678F74.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/0629EA4F-2D75-E311-B962-002590596490.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/0649614F-A575-E311-99AF-003048679006.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/0653F528-B075-E311-967E-002618943807.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/0654BD33-2D75-E311-80A0-00304867BFAE.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/067DF0B5-4E75-E311-A29D-0026189438CE.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/06AAA0A0-9F75-E311-B383-0026189438DC.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/06BA3405-E775-E311-9877-0025905A612E.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/06D2CD1E-8175-E311-A7F2-003048678AE4.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/06F5B289-D675-E311-804D-002590596486.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/08245C9E-F675-E311-847A-0026189438D2.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/084C5218-AA75-E311-97B9-00304867929E.root",
  #  "/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/0859DDC1-E575-E311-9F7B-0025905A612A.root",
 

 
)
)


process.load("Configuration.StandardSequences.RawToDigi_Data_cff")

from SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff import *
process.simHcalTriggerPrimitiveDigis.inputLabel = cms.VInputTag(
    cms.InputTag('hcalDigis'),
    cms.InputTag('hcalDigis')
)

process.HcalTPGCoderULUT.LUTGenerationMode = cms.bool(True)

process.L1TRerunHCALTP_FromRAW = cms.Sequence(
    process.hcalDigis
    * process.simHcalTriggerPrimitiveDigis
)



# RCT
# HCAL input would be from hcalDigis if hack not needed
process.load("L1Trigger.Configuration.SimL1Emulator_cff")
process.simRctDigis.ecalDigis = cms.VInputTag( cms.InputTag( 'ecalDigis:EcalTriggerPrimitives' ) )
process.simRctDigis.hcalDigis = cms.VInputTag( cms.InputTag( 'simHcalTriggerPrimitiveDigis' ) )


process.rctPattern = cms.EDProducer("L1RCTPatternProducer",
    fgEcalE = cms.untracked.int32(4),
    randomPercent = cms.untracked.int32(10),
    rctTestInputFile = cms.untracked.string('rctInput'),
    testName = cms.untracked.string('testCard6'),
    regionSums = cms.untracked.bool(True)
)


process.output = cms.OutputModule(
    "PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
#    outputCommands = cms.untracked.vstring('drop *','keep *_*_*_TEST'),
    outputCommands = cms.untracked.vstring('keep *'),
    fileName = cms.untracked.string('rct64TTBarEventsFilter09.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('')
    )
                                           )

process.rctSaveInput = cms.EDAnalyzer("L1RCTCreateInput",
    hcalDigisLabel = cms.InputTag("simHcalTriggerPrimitiveDigis"),#hcalTriggerPrimitiveDigis"),
    useDebugTpgScales = cms.bool(False),
    rctTestInputFile = cms.untracked.string('ttbarInputNewFilter09.txt'),
    useEcal = cms.bool(True),
    useHcal = cms.bool(True),
    ecalDigisLabel = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),#ecalTriggerPrimitiveDigis"),
        crateNumber = cms.vint32(9),
        cardNumber = cms.vint32(-1),
        includeHF=cms.bool(True)
)

process.filteredSimRctDigis = cms.EDProducer("L1RCTFilteredDigis",
        crateNumber = cms.vint32(9),
        cardNumber = cms.vint32(-1),
         includeHF=cms.bool(True),
        rctDigisLabel = cms.InputTag("simRctDigis")
)
        # crateNumber is the crate where there is energy required
        # particle ID value is the pythia number. Using 999 allows any particle



process.L1RCTTestAnalyzer=cms.EDAnalyzer("L1RCTAnalyzer",
showEmCands=cms.untracked.bool(True),
showRegionSums=cms.untracked.bool(True),
ecalDigisLabel=cms.InputTag('ecalDigis:EcalTriggerPrimitives'),
hcalDigisLabel=cms.InputTag('simHcalTriggerPrimitiveDigis'),
rctDigisLabel=cms.InputTag('simRctDigis'),
testName = cms.untracked.string('ttbarLogFilter9'),
        crateNumber = cms.vint32(9),
        cardNumber = cms.vint32(-1),
        includeHF=cms.bool(True)
)

process.TFileService = cms.Service(
  "TFileService",
  fileName = cms.string("test.root")
  )

process.runpath = cms.Path(process.L1TRerunHCALTP_FromRAW*process.ecalDigis*process.simRctDigis*process.rctSaveInput*process.filteredSimRctDigis*process.L1RCTTestAnalyzer)#
process.outpath = cms.EndPath(process.output)

process.RCTConfigProducers.eGammaLSB = 1
process.RCTConfigProducers.jetMETLSB = 1
process.l1CaloScales.L1CaloEmEtScaleLSB = 1
process.l1CaloScales.L1CaloRegionEtScaleLSB = 1
process.RCTConfigProducers.eGammaECalScaleFactors = [1., 1., 1., 1., 1., 1., 1., 1.,1., 1., 1., 1.,1., 1., 1., 1.,1., 1., 1., 1.,1., 1., 1., 1.,1., 1., 1., 1.,1., 1., 1., 1. ]
process.RCTConfigProducers.jetMETECalScaleFactors = [1., 1., 1., 1., 1., 1., 1., 1.,1., 1., 1., 1.,1., 1., 1., 1.,1., 1., 1., 1.,1., 1., 1., 1.,1., 1., 1., 1.,1., 1., 1., 1. ]

process.RCTConfigProducers.eActivityCut= 0.0
process.RCTConfigProducers.hActivityCut= 0.0
process.RCTConfigProducers.eMaxForFGCut= 1024.0
process.RCTConfigProducers.hOeCut= 1024.0




