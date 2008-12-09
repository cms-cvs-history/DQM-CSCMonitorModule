import FWCore.ParameterSet.Config as cms

#--------------------------
# DQM Module
#--------------------------

dqmCSCClient = cms.EDFilter("CSCMonitorModuleCmn",

  EventProcessor = cms.untracked.PSet(
    BINCHECKER_CRC_ALCT = cms.untracked.bool(True),
    BINCHECKER_CRC_CLCT = cms.untracked.bool(True),
    BINCHECKER_CRC_CFEB = cms.untracked.bool(True),
    BINCHECKER_OUTPUT   = cms.untracked.bool(False),
    FRAEFF_AUTO_UPDATE  = cms.untracked.bool(True),
    FRAEFF_SEPARATE_THREAD  = cms.untracked.bool(False),
    BOOKING_XML_FILE = cms.untracked.string('DQM/CSCMonitorModule/data/emuDQMBookingCmn.xml'),
    FOLDER_EMU = cms.untracked.string('CSC/Summary/'),
    FOLDER_DDU = cms.untracked.string('CSC/DDU/'),
    FOLDER_CSC = cms.untracked.string('CSC/CSC/'),
    FOLDER_PAR = cms.untracked.string('CSC/EventInfo/reportSummaryContents/'),
    DDU_CHECK_MASK = cms.untracked.uint32(0xFFFFFFFF ),
    DDU_BINCHECK_MASK = cms.untracked.uint32(0x02080016),
    BINCHECK_MASK = cms.untracked.uint32(0xFFFFFFFF),
    FRAEFF_AUTO_UPDATE_START = cms.untracked.uint32(5),
    FRAEFF_AUTO_UPDATE_FREQ = cms.untracked.uint32(1),
    EFF_COLD_THRESHOLD = cms.untracked.double(0.1),
    EFF_COLD_SIGFAIL = cms.untracked.double(5.0),
    EFF_HOT_THRESHOLD = cms.untracked.double(0.1),
    EFF_HOT_SIGFAIL = cms.untracked.double(5.0),
    EFF_ERR_THRESHOLD = cms.untracked.double(0.1),
    EFF_ERR_SIGFAIL = cms.untracked.double(5.0),
    EFF_NODATA_THRESHOLD = cms.untracked.double(0.1),
    EFF_NODATA_SIGFAIL = cms.untracked.double(5.0)
  ),

  # Just to check that this file exists
  BookingFile = cms.FileInPath('DQM/CSCMonitorModule/data/emuDQMBookingCmn.xml'),

)

