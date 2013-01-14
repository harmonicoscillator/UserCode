import FWCore.ParameterSet.Config as cms

process = cms.Process("FLOWCALCULATOR")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:../GlauberProducer/myOutputFile.root'
    )
)

process.FlowCalculator = cms.EDProducer('FlowCalculator'
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('myOutputFile2.root')
)

  
process.p = cms.Path(process.FlowCalculator)

process.e = cms.EndPath(process.out)
