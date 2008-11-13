import FWCore.ParameterSet.Config as cms

#--------------------------
# DQM Module
#--------------------------

dqmClient = cms.EDFilter("CSCMonitorModule",
  FractUpdateKey = cms.untracked.uint32(5),
  FractUpdateEventFreq = cms.untracked.uint32(100),
  BookingFile = cms.FileInPath('DQM/CSCMonitorModule/data/emuDQMBooking.xml'),
  InputObjects = cms.untracked.InputTag("source"),
  ExaminerMask = cms.untracked.uint32(384562166),
  ExaminerOutput = cms.untracked.bool(False),
  AddressMask = cms.untracked.vstring(),
  ExaminerCRCKey = cms.untracked.uint32(7),
  monitorName = cms.untracked.string('CSC'),
  hitBookDDU = cms.untracked.bool(True),
  effParameters = cms.untracked.PSet( 
    threshold_cold   = cms.untracked.double(0.1),
    threshold_hot    = cms.untracked.double(2.0),
    threshold_err    = cms.untracked.double(0.1),
    threshold_nodata = cms.untracked.double(1.0),
    sigfail_cold     = cms.untracked.double(5.0),
    sigfail_hot      = cms.untracked.double(5.0),
    sigfail_err      = cms.untracked.double(5.0),
    sigfail_nodata   = cms.untracked.double(5.0)
  )
)

