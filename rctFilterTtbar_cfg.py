import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.load("SimCalorimetry.EcalSimProducers.ecaldigi_cfi")
process.load("SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff")
process.load("SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cfi")

process.load("L1TriggerConfig.RCTConfigProducers.L1RCTConfig_cff")

process.load("L1Trigger.RegionalCaloTrigger.rctDigis_cfi")

process.load("L1Trigger.RegionalCaloTrigger.L1RCTTestAnalyzer_cfi")

process.load("L1TriggerConfig.L1ScalesProducers.L1CaloScalesConfig_cff")

process.load("Configuration.EventContent.EventContent_cff")

process.load("CalibCalorimetry.EcalTPGTools.ecalTPGScale_cff")

process.load("SimCalorimetry.EcalTrigPrimProducers.ecalTriggerPrimitiveDigis_cff")

# To make ECAL link test file

process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")

process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")

process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

process.load("CalibCalorimetry.Configuration.Ecal_FakeConditions_cff")

#-#-# ECAL TPG
process.load("Geometry.EcalMapping.EcalMapping_cfi")

process.load("SimCalorimetry.EcalTrigPrimProducers.ecalTriggerPrimitiveDigis_cfi")

#-#-# Digi -> Flat
process.load("SimCalorimetry.EcalElectronicsEmulation.EcalSimRawData_cfi")

from SimCalorimetry.EcalTrigPrimProducers.ecalTrigPrimESProducer_cff import *
#import SimCalorimetry.EcalTrigPrimProducers.ecalTriggerPrimitiveDigis_cfi

EcalTrigPrimESProducer.DatabaseFile = 'TPG_RCT_identity-21X.txt' #'TPG_startup.txt.gz'
#EcalTrigPrimESProducer.DatabaseFileEB = "TPG_EB_20.txt"
#EcalTrigPrimESProducer.DatabaseFileEE = "TPG_EE_20.txt"

## process.EcalTrigPrimESProducer = cms.ESProducer("EcalTrigPrimESProducer",
##    DatabaseFileEE = cms.untracked.string('TPG_EE.txt'),
##    DatabaseFileEB = cms.untracked.string('TPG_EB.txt')
## )

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

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.GlobalTag.globaltag='CRUZET4_V1::All' #'CRAFT_V3P::All'
process.load("IORawData.CaloPatterns.HtrXmlPattern_cfi")
process.htr_xml.fill_by_hand = False
process.htr_xml.hand_pattern_number = 1
process.htr_xml.file_tag = 'ttbar'


#process.RCTConfigProducers.eGammaLSB = 1
#process.RCTConfigProducers.jetMETLSB = 1
process.rctDigis.ecalDigisLabel = 'simEcalTriggerPrimitiveDigis'
process.rctDigis.hcalDigisLabel = 'simHcalTriggerPrimitiveDigis' #hcalTriggerPrimitiveDigis'
#process.rctDigis.useDebugTpgScales = True
process.L1RCTTestAnalyzer.showRegionSums = True
process.L1RCTTestAnalyzer.limitTo64 = True
process.L1RCTTestAnalyzer.testName = 'ttbar'
process.l1CaloScales.L1CaloEmEtScaleLSB = 1
process.CaloTPGTranscoder.hcalLUT2 = 'TPGcalcDecompress2Identity.txt'
process.EcalTrigPrimESProducer.DatabaseFile = 'TPG_RCT_identity-21X.txt'

process.load("L1TriggerConfig.RCTConfigProducers.Rct-EEG_EHSUMS_TAU3_DECO_25_CRAFT1_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(349)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#    '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/0067EE3B-7D6C-DD11-8CFD-000423D99896.root', #ttbar19
#        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/06D212C3-7E6C-DD11-AF46-000423D98EC4.root', #ttbar20
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/186AD7AA-7D6C-DD11-9A25-000423D991F0.root', #21
#        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/34BC69E1-7D6C-DD11-9403-000423D9517C.root', #22
#        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/42C24772-7D6C-DD11-A37B-001617E30E28.root',#23
#    '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/4C56909F-7D6C-DD11-8437-001617E30F50.root',#24
#        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/5670F295-7E6C-DD11-8769-000423D99CEE.root',#25
#        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/60D93067-7E6C-DD11-AED8-000423D9517C.root',#26
#        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/68D80573-7D6C-DD11-992E-001617DBD332.root', #27
#        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/6AE79D69-7E6C-DD11-9F97-000423D98834.root',#28
#        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/70E53943-7D6C-DD11-AFBA-000423D94990.root',#29
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/747D2CAA-7D6C-DD11-B385-001617C3B64C.root',
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/7CC22405-7E6C-DD11-971F-001617DBD472.root',
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/828F1371-7D6C-DD11-858E-000423D94E1C.root',
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/8C30A0F7-7D6C-DD11-B330-000423D991F0.root',
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/94379868-7E6C-DD11-8AE7-000423D98BC4.root',
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/9667BC77-7D6C-DD11-8240-001617E30D06.root',
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/A0FF9669-7D6C-DD11-B5CA-000423D99896.root',
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/A6EEA047-7D6C-DD11-87C5-0016177CA7A0.root',
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/B0EC238A-7D6C-DD11-8CF5-000423D99F1E.root',
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/B463DA7B-7D6C-DD11-ACF2-000423D991F0.root',
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/C4C7D877-7D6C-DD11-B3E8-000423D98844.root',
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/D4F88AFC-7D6C-DD11-A44D-000423D99AAE.root',
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/D6468EA4-7D6C-DD11-9728-001617C3B6DC.root',
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/D6AC476C-7D6C-DD11-8696-000423D9A212.root',
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/DAFCEA3D-7D6C-DD11-B6A8-0019DB29C5FC.root',
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/DCBA1EA9-7D6C-DD11-8ADB-000423D94E70.root',
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/E6673508-7E6C-DD11-9E24-000423D9939C.root',
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/F4EB4D77-7D6C-DD11-ADA0-000423D94990.root',
        '/store/relval/CMSSW_2_1_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0005/A48301B6-AC6C-DD11-9A22-000423D992A4.root'
	)
    )
#maybe not
process.hcalPatternSource = cms.EDProducer("HcalPatternSource")

process.rctSaveInput = cms.EDFilter("L1RCTSaveInput",
    hcalDigisLabel = cms.InputTag("simHcalTriggerPrimitiveDigis"),#hcalTriggerPrimitiveDigis"),
    useDebugTpgScales = cms.bool(True),
    rctTestInputFile = cms.untracked.string('ttbarInput.txt'),
    useEcal = cms.bool(True),
    useHcal = cms.bool(True),
    ecalDigisLabel = cms.InputTag("simEcalTriggerPrimitiveDigis")#ecalTriggerPrimitiveDigis")
)

process.filter = cms.EDFilter("L1RCTFilter",
        cardNumber = cms.untracked.vint32(4,5,6),
	pTMin = cms.untracked.double(15),
        rctDigisLabel = cms.InputTag("rctDigis")
)
process.FEVT = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    fileName = cms.untracked.string('rct.root')
)
        # crateNumber is the card where there is energy required
        # particle ID value is the pythia number. Using 999 allows any particle
process.input = cms.Path(process.rctDigis)
process.p = cms.Path(process.filter*process.rctSaveInput*process.L1RCTTestAnalyzer)#
#* process.ecalSimRawData * process.htr_xml)
#process.outpath = cms.EndPath(process.FEVT)
process.schedule = cms.Schedule(process.input,process.p)

#For ECAL pattern/link tests
process.simEcalTriggerPrimitiveDigis.TcpOutput = True
process.simEcalTriggerPrimitiveDigis.BarrelOnly = True
process.simEcalTriggerPrimitiveDigis.Label = 'rctPattern' #'simEcalTriggerPrimitiveDigis' #'simEcalUnsuppressedDigis'
#process.simEcalTriggerPrimitiveDigis.InstanceEB = 'ebDigis'
#process.simEcalTriggerPrimitiveDigis.InstanceEE = 'eeDigis'
process.simEcalTriggerPrimitiveDigis.Debug = False

process.ecalSimRawData.tcc2dccData = False
process.ecalSimRawData.srp2dccData = False
process.ecalSimRawData.fe2dccData = False
process.ecalSimRawData.trigPrimProducer = 'simEcalTriggerPrimitiveDigis'
process.ecalSimRawData.tcpDigiCollection = '' #'formatTCP'
process.ecalSimRawData.tpVerbose = False
process.ecalSimRawData.tccInDefaultVal = 0
process.ecalSimRawData.tccNum = -1
process.ecalSimRawData.outputBaseName = 'ttbardata/ecal'
