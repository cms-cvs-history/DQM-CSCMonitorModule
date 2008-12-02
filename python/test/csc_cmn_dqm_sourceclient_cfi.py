import FWCore.ParameterSet.Config as cms

#--------------------------
# DQM Module
#--------------------------

dqmCSCClient = cms.EDFilter("CSCMonitorModuleCmn",

  FractUpdateKey = cms.untracked.uint32(5),
  FractUpdateEventFreq = cms.untracked.uint32(1),

  BookingFile = cms.FileInPath('DQM/CSCMonitorModule/data/emuDQMBookingCmn.xml'),

  ExaminerMask = cms.untracked.uint32(384562166),
  ExaminerOutput = cms.untracked.bool(False),
  ExaminerCRCKey = cms.untracked.uint32(7),

  hitBookDDU = cms.untracked.bool(True),

  AddressMask = cms.untracked.vstring(),

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

