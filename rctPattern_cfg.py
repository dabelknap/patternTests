import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
process.load("SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff")

process.load("L1TriggerConfig.RCTConfigProducers.L1RCTConfig_cff")

process.load("L1Trigger.RegionalCaloTrigger.rctDigis_cfi")

process.load("L1Trigger.RegionalCaloTrigger.L1RCTTestAnalyzer_cfi")

process.load("L1TriggerConfig.L1ScalesProducers.L1CaloScalesConfig_cff")

process.load("Configuration.EventContent.EventContent_cff")

process.load("CalibCalorimetry.EcalTPGTools.ecalTPGScale_cff")

# For ECAL link tests

process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")

process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")

process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

process.load("CalibCalorimetry.Configuration.Ecal_FakeConditions_cff")

process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")

process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")

process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

process.load("CalibCalorimetry.Configuration.Ecal_FakeConditions_cff")


#-#-# ECAL TPG
process.load("Geometry.EcalMapping.EcalMapping_cfi")

process.load("SimCalorimetry.EcalTrigPrimProducers.ecalTriggerPrimitiveDigis_cfi")

#-#-# Digi -> Flat
process.load("SimCalorimetry.EcalElectronicsEmulation.EcalSimRawData_cfi")

#process.EcalTrigPrimESProducer = cms.ESProducer("EcalTrigPrimESProducer",
#    DatabaseFileEE = cms.untracked.string('TPG_EE_25.txt'),
#    DatabaseFileEB = cms.untracked.string('TPG_EB_25.txt')
#)

process.tpparams6 = cms.ESSource("EmptyESSource",
    recordName = cms.string('EcalTPGLutGroupRcd'),
    iovIsRunNotTime = cms.bool(True),
    firstValid = cms.vuint32(1)
)

process.tpparams7 = cms.ESSource("EmptyESSource",
    recordName = cms.string('EcalTPGLutIdMapRcd'),
    iovIsRunNotTime = cms.bool(True),
    firstValid = cms.vuint32(1)
)

process.eegeom = cms.ESSource("EmptyESSource",
    recordName = cms.string('EcalMappingRcd'),
    iovIsRunNotTime = cms.bool(False),
    firstValid = cms.vuint32(1)
)



# L1 GT EventSetup
from L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff import *
from L1TriggerConfig.L1GtConfigProducers.Luminosity.startup.L1Menu_startup_v3_Unprescaled_cff import *

# Makes HCAL link test file
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.GlobalTag.globaltag='CRUZET4_V1::All' #'CRAFT_V3P::All'
#process.load("IORawData.CaloPatterns.HtrXmlPattern_cfi")
process.htr_xml = cms.EDFilter("HtrXmlPattern",
    sets_to_show = cms.untracked.int32(0),
    show_errors = cms.untracked.bool(True),

    presamples_per_event = cms.untracked.int32(4),
    samples_per_event = cms.untracked.int32(10),

    write_root_file = cms.untracked.bool(True),
    XML_file_mode = cms.untracked.int32(3), #0=no-output; 1=one-file; 2=one-file-per-crate; 3=one-file-per-fiber
    file_tag = cms.untracked.string('tplf'),
    user_output_directory = cms.untracked.string('/tmp/grogg'),

    fill_by_hand = cms.untracked.bool(False),
    hand_pattern_number = cms.untracked.int32(3)
)

process.RCTConfigProducers.eGammaLSB = 1
process.RCTConfigProducers.jetMETLSB = 1
process.rctDigis.ecalDigisLabel = 'rctPattern'
process.rctDigis.hcalDigisLabel = 'rctPattern'
process.rctDigis.useDebugTpgScales = True
process.L1RCTTestAnalyzer.showRegionSums = True #True
process.L1RCTTestAnalyzer.testName = 'tplf'
process.l1CaloScales.L1CaloEmEtScaleLSB = 1
process.CaloTPGTranscoder.hcalLUT2 = 'TPGcalcDecompress2Identity.txt'
process.EcalTrigPrimESProducer.DatabaseFile = 'TPG_RCT_identity-21X.txt'

#process.load("L1TriggerConfig.RCTConfigProducers.Rct-EEG_EHSUMS_TAU3_DECO_25_CRAFT1_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(64)
)
process.source = cms.Source("EmptySource")

process.rctPattern = cms.EDProducer("L1RCTPatternProducer",
    fgEcalE = cms.untracked.int32(4),
    randomPercent = cms.untracked.int32(35),
    randomSeed = cms.untracked.int32(12345),
    rctTestInputFile = cms.untracked.string('rctInput'),
    testName = cms.untracked.string('tplf'),
    regionSums = cms.untracked.bool(True)  #True)
)

#maybe not
process.hcalPatternSource = cms.EDProducer("HcalPatternSource")

process.rctDigiToSourceCardText = cms.EDFilter("RctDigiToSourceCardText",
    RctInputLabel = cms.InputTag("rctDigis"),
    TextFileName = cms.string('RctDigiToSourceCardwalkingOnes.dat')
)

process.FEVT = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    fileName = cms.untracked.string('rct.root')
)

process.input = cms.Path(process.rctPattern)
process.p = cms.Path(process.rctDigis * process.L1RCTTestAnalyzer)# * process.htr_xml *  process.ecalSimRawData ) 
#* process.htr_xml *  process.ecalSimRawData
process.outpath = cms.EndPath(process.FEVT)
process.schedule = cms.Schedule(process.input,process.p)

# Makes ECAL link test file
process.ecalSimRawData.tcc2dccData = False# True #
process.ecalSimRawData.srp2dccData = False#True #
process.ecalSimRawData.fe2dccData = False# True # 
process.ecalSimRawData.trigPrimProducer = 'rctPattern' #'ecalTriggerPrimitiveDigis' #
#process.ecalSimRawData.trigPrimDigiCollection ='rctPattern' #'simEcalTriggerPrimitiveDigis' #
process.ecalSimRawData.tcpDigiCollection = '' # 'rctPattern' #'formatTCP'  #
process.ecalSimRawData.tpVerbose = False
process.ecalSimRawData.tccInDefaultVal = 0
process.ecalSimRawData.tccNum = -1
process.ecalSimRawData.outputBaseName = 'data/tplf/ecal'

