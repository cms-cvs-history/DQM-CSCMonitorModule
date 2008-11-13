import FWCore.ParameterSet.Config as cms

#--------------------------
# DQM Module
#--------------------------

cscDQMEvF = cms.EDFilter("CSCHLTMonitorModule",
  InputObjects =   cms.untracked.InputTag("source"),
  ExaminerMask =   cms.untracked.uint32(384562166),
  ExaminerOutput = cms.untracked.bool(False),
  ExaminerCRCKey = cms.untracked.uint32(7),
  monitorName =    cms.untracked.string('CSC'),
  FEDIdMin =       cms.untracked.uint32(750),
  FEDIdMax =       cms.untracked.uint32(757)
)

