import FWCore.ParameterSet.Config as cms

from CondCore.DBCommon.CondDBSetup_cfi import *
cscConditions = cms.ESSource("PoolDBESSource",
    CondDBSetup,
    siteLocalConfig = cms.untracked.bool(False),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('CSCChamberIndexRcd'),
        tag = cms.string('CSCChamberIndex')
    ), 
        cms.PSet(
            record = cms.string('CSCChamberMapRcd'),
            tag = cms.string('CSCChamberMap')
        ), 
        cms.PSet(
            record = cms.string('CSCCrateMapRcd'),
            tag = cms.string('CSCCrateMap')
        ), 
        cms.PSet(
            record = cms.string('CSCDDUMapRcd'),
            tag = cms.string('CSCDDUMap')
        )),
    messagelevel = cms.untracked.uint32(0),
    timetype = cms.string('runnumber'),
    connect = cms.string('frontier://FrontierDev/CMS_COND_CSC'),
    authenticationMethod = cms.untracked.uint32(1)
)

dqmClient = cms.EDFilter("CSCMonitorModule",
    FractUpdateKey = cms.untracked.uint32(5),
    FractUpdateEventFreq = cms.untracked.uint32(10),
    BookingFile = cms.FileInPath('DQM/CSCMonitorModule/data/emuDQMBooking.xml'),
    InputObjects = cms.untracked.InputTag("source"),
    ExaminerMask = cms.untracked.uint32(384562166),
    ExaminerOutput = cms.untracked.bool(False),
    AddressMask = cms.untracked.vstring(),
    ExaminerCRCKey = cms.untracked.uint32(7),
    monitorName = cms.untracked.string('CSC'),
    hitBookDDU = cms.untracked.bool(True)
)

MessageLogger = cms.Service("MessageLogger",
    suppressInfo = cms.untracked.vstring('source'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO'),
        WARNING = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        noLineBreaks = cms.untracked.bool(False)
    ),
    detailedInfo = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    ),
    critical = cms.untracked.PSet(
        threshold = cms.untracked.string('ERROR')
    ),
    debugModules = cms.untracked.vstring('*'),
    destinations = cms.untracked.vstring('detailedInfo', 
        'critical', 
        'cout')
)



