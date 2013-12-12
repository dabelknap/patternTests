import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
process.load("SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff")

process.load("L1TriggerConfig.RCTConfigProducers.L1RCTConfig_cff")

process.load("L1Trigger.RegionalCaloTrigger.rctDigis_cfi")

process.load("L1Trigger.RegionalCaloTrigger.L1RCTTestAnalyzer_cfi")

process.load("L1TriggerConfig.L1ScalesProducers.L1CaloScalesConfig_cff")

process.load("Configuration.EventContent.EventContent_cff")

process.load("CalibCalorimetry.EcalTPGTools.ecalTPGScale_cff")

process.RCTConfigProducers.eGammaLSB = 1
process.RCTConfigProducers.jetMETLSB = 1
process.rctDigis.ecalDigisLabel = 'rctPattern'
process.rctDigis.hcalDigisLabel = 'rctPattern'
process.rctDigis.useDebugTpgScales = True
process.L1RCTTestAnalyzer.showRegionSums = True
process.L1RCTTestAnalyzer.testName = 'ttbar'
process.l1CaloScales.L1CaloEmEtScaleLSB = 1
process.CaloTPGTranscoder.hcalLUT2 = 'TPGcalcDecompress2Identity.txt'
process.EcalTrigPrimESProducer.DatabaseFile = 'TPG_RCT_identity-21X.txt'

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.source = cms.Source("EmptySource")

process.rctPattern = cms.EDProducer("L1RCTPatternProducer",
    fgEcalE = cms.untracked.int32(4),
    randomPercent = cms.untracked.int32(10),
    rctTestInputFile = cms.untracked.string('rctInput'),
    testName = cms.untracked.string('ttbar'),
    regionSums = cms.untracked.bool(True)
)

process.rctDigiToSourceCardText = cms.EDFilter("RctDigiToSourceCardText",
    RctInputLabel = cms.InputTag("rctDigis"),
    TextFileName = cms.string('RctDigiToSourceCardwalkingOnes.dat')
)

process.FEVT = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    fileName = cms.untracked.string('rct.root')
)

process.input = cms.Path(process.rctPattern)
process.p = cms.Path(process.rctDigis*process.L1RCTTestAnalyzer)
process.outpath = cms.EndPath(process.FEVT)
process.schedule = cms.Schedule(process.input,process.p)


