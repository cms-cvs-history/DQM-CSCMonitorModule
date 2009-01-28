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

process.source = cms.Source("DaqSource",
    readerPluginName = cms.untracked.string('CSCFileReader'),
    readerPset = cms.untracked.PSet(
        firstEvent = cms.untracked.int32(0),
        RUI01 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI01_Monitor_001.raw'),
        RUI02 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI02_Monitor_001.raw'),
        RUI03 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI03_Monitor_001.raw'),
        RUI04 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI04_Monitor_001.raw'),
        #RUI05 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI05_Monitor_001.raw'),
        RUI06 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI06_Monitor_001.raw'),
        RUI07 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI07_Monitor_001.raw'),
        RUI08 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI08_Monitor_001.raw'),
        #RUI09 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI09_Monitor_001.raw'),
        #RUI10 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI10_Monitor_001.raw'),
        RUI11 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI11_Monitor_001.raw'),
        RUI12 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI12_Monitor_001.raw'),
        #RUI13 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI13_Monitor_001.raw'),
        #RUI14 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI14_Monitor_001.raw'),
        RUI15 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI15_Monitor_001.raw'),
        RUI16 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI16_Monitor_001.raw'),
        RUI17 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI17_Monitor_001.raw'),
        RUI18 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI18_Monitor_001.raw'),
        RUI19 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI19_Monitor_001.raw'),
        RUI20 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI20_Monitor_001.raw'),
        RUI21 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI21_Monitor_001.raw'),
        RUI22 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI22_Monitor_001.raw'),
        #RUI23 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI23_Monitor_001.raw'),
        RUI24 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI24_Monitor_001.raw'),
        RUI25 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI25_Monitor_001.raw'),
        RUI26 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI26_Monitor_001.raw'),
        RUI27 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI27_Monitor_001.raw'),
        #RUI28 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI28_Monitor_001.raw'),
        #RUI29 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI29_Monitor_001.raw'),
        RUI30 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI30_Monitor_001.raw'),
        RUI31 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI31_Monitor_001.raw'),
        RUI32 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI32_Monitor_001.raw'),
        #RUI33 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI33_Monitor_001.raw'),
        RUI34 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI34_Monitor_001.raw'),
        RUI35 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI35_Monitor_001.raw'),
        RUI36 = cms.untracked.vstring('/tmp/valdo/csc_00069997_EmuRUI36_Monitor_001.raw'),
        FED750 = cms.untracked.vstring(
            'RUI01', 
            'RUI02', 
            'RUI03', 
            'RUI04', 
            #'RUI05', 
            'RUI06', 
            'RUI07', 
            'RUI08', 
            #'RUI09', 
            #'RUI10', 
            'RUI11', 
            'RUI12', 
            #'RUI13', 
            #'RUI14', 
            'RUI15', 
            'RUI16', 
            'RUI17', 
            'RUI18', 
            'RUI19', 
            'RUI20', 
            'RUI21', 
            'RUI22', 
            #'RUI23', 
            'RUI24', 
            'RUI25', 
            'RUI26', 
            'RUI27', 
            #'RUI28', 
            #'RUI29', 
            'RUI30', 
            'RUI31', 
            'RUI32', 
            #'RUI33', 
            'RUI34', 
            'RUI35', 
            'RUI36')
    )
)

#----------------------------
# DQM Environment
#-----------------------------

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")

#process.DQMStore.referenceFileName = '/home/dqmdevlocal/reference/csc_reference.root'
process.DQMStore.referenceFileName = '/afs/cern.ch/user/v/valdo/data/csc_reference.root'
#process.DQMStore.referenceFileName = '/nfshome0/valdo/CMSSW_2_1_0/src/DQM/CSCMonitorModule/data/csc_reference.root'

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


