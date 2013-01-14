import FWCore.ParameterSet.Config as cms

process = cms.Process("GLAUBERPRODUCER")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/mnt/hadoop/cms/store/user/icali/HIMinBiasUPC/Skim_HLT_HIMinBiasHfOrBSC_v2_RECO/0e004a3212cb57168d8309f8827de16c/step2_RAW2DIGI_L1Reco_RECO_999_1_hYn.root'
    )
)

process.GlauberProducer = cms.EDProducer('GlauberProducer'
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('myOutputFile.root')
)

process.load('Configuration.StandardSequences.Services_cff')

process.RandomNumberGeneratorService.GlauberProducer = process.RandomNumberGeneratorService.generator.clone(initialSeed = 12345)

process.p = cms.Path(process.GlauberProducer)

process.e = cms.EndPath(process.out)
