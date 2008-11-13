import FWCore.ParameterSet.Config as cms

from DQMOffline.Muon.CSCMonitor_cfi import *
from DQM.CSCMonitorModule.test.csc_dqm_sourceclient_cfi import *
from DQM.CSCMonitorModule.test.csc_hlt_dqm_sourceclient_cfi import *

cscSources = cms.Sequence(cscMonitor * dqmClient * cscDQMEvF)
