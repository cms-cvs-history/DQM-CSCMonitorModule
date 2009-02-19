import FWCore.ParameterSet.Config as cms

process = cms.Process("CSCDQM")

#-------------------------------------------------
# DQM Module Configuration
#-------------------------------------------------

process.load("DQM.CSCMonitorModule.test.csc_cmn_dqm_sourceclient_cfi")

#-------------------------------------------------
# Offline DQM Module Configuration
#-------------------------------------------------

#process.load("DQMOffline.Muon.CSCMonitor_cfi")
#process.load("Configuration/StandardSequences/MagneticField_cff")
#process.load("Configuration/StandardSequences/RawToDigi_Data_cff")
#process.load("Configuration.StandardSequences.Reconstruction_cff")
#process.csc2DRecHits.readBadChambers = cms.bool(False)

#----------------------------
# Event Source
#-----------------------------

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(50))

process.source = cms.Source("PoolSource",
    fileNames  = cms.untracked.vstring('/store/data/Commissioning08/Cosmics/RAW/v1/000/066/910/8CA64FCF-259F-DD11-B86D-000423D99BF2.root'),
    skipEvents = cms.untracked.uint32(25900)
)

#----------------------------
# DQM Environment
#-----------------------------

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")

#process.DQMStore.referenceFileName = '/home/dqmdevlocal/reference/csc_reference.root'
process.DQMStore.referenceFileName = '/afs/cern.ch/user/v/valdo/data/csc_reference.root'

#----------------------------
# DQM Playback Environment
#-----------------------------

process.load("DQM.Integration.test.environment_playback_cfi")
process.dqmEnv.subSystemFolder    = "CSC"

process.DQM.collectorHost = 'pccmsdqm02.cern.ch'
#process.DQM.collectorHost = 'localhost'
process.dqmSaver.dirName = '.'

#-----------------------------
# Magnetic Field
#-----------------------------

process.load("Configuration/StandardSequences/MagneticField_cff")

#-------------------------------------------------
# GEOMETRY
#-------------------------------------------------

process.load("Configuration.StandardSequences.Geometry_cff")

#-------------------------------------------------
# Global Tag
#-------------------------------------------------

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.connect = "sqlite_file:/nfshome0/malgeri/public/globtag/CRZT210_V1H.db"
#process.GlobalTag.connect = "sqlite_file:/afs/cern.ch/user/v/valdo/CRZT210_V1H.db"
#process.GlobalTag.connect = "frontier://FrontierDev/CMS_COND_CSC"
process.GlobalTag.connect ="frontier://(proxyurl=http://localhost:3128)(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_21X_GLOBALTAG"
process.GlobalTag.globaltag = "CRZT210_V1H::All"
#process.GlobalTag.globaltag = 'CRAFT_V3P::All'
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

#--------------------------
# Web Service
#--------------------------

process.ModuleWebRegistry = cms.Service("ModuleWebRegistry")
process.AdaptorConfig = cms.Service("AdaptorConfig")

#--------------------------
# Message Logger
#--------------------------

process.MessageLogger = cms.Service("MessageLogger",

  suppressInfo = cms.untracked.vstring( 
    'source',
    'CSCGeometryESModule',
    'FakeAlignmentProducer',
    'L1GtConfigProducers',
    'SiStripQualityESProducer',
    'TrackerDigiGeometryESModule',
    'AfterModuleConstruction'
  ),

  cout = cms.untracked.PSet(
    threshold = cms.untracked.string('INFO'),
  ),

  detailedInfo = cms.untracked.PSet(
    threshold = cms.untracked.string('INFO')
  ),

  critical = cms.untracked.PSet(
    threshold = cms.untracked.string('ERROR')
  ),

  debug = cms.untracked.PSet(
    threshold = cms.untracked.string('DEBUG')
  ),

  debugModules = cms.untracked.vstring('CSCMonitorModuleCmn:dqmCSCClient'),

  destinations = cms.untracked.vstring(
    'debug',
    'detailedInfo', 
    'critical', 
    'cout'
  )
)                                                              
                                                              

#--------------------------
# Sequences
#--------------------------

process.p = cms.Path(process.dqmCSCClient + process.dqmEnv + process.dqmSaver)
#process.p = cms.Path(process.muonCSCDigis * process.csc2DRecHits * process.cscSegments * process.cscMonitor * process.dqmCSCClient + process.dqmEnv + process.dqmSaver)


