import FWCore.ParameterSet.Config as cms


#--------------------------
# DQM Module
#--------------------------

dqmClient = cms.EDFilter("CSCHLTMonitorModule",
  InputObjects = cms.untracked.InputTag("source"),
  ExaminerMask = cms.untracked.uint32(384562166),
  ExaminerOutput = cms.untracked.bool(False),
  ExaminerCRCKey = cms.untracked.uint32(7),
  monitorName = cms.untracked.string('CSC'),
  FEDIds = cms.untracked.vuint32(750, 751, 752, 753, 754, 755, 756, 757)
)

#--------------------------
# Message Logger
#--------------------------

MessageLogger = cms.Service("MessageLogger",

  suppressInfo = cms.untracked.vstring('source'),
  suppressDebug = cms.untracked.vstring('source'),
  suppressWarning = cms.untracked.vstring('source'),

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

  debug = cms.untracked.PSet(
    threshold = cms.untracked.string('DEBUG')
  ),

  debugModules = cms.untracked.vstring('CSCHLTMonitormodule'),

  destinations = cms.untracked.vstring(
#    'debug',
#    'detailedInfo', 
#    'critical', 
#    'cout'
  )

)



