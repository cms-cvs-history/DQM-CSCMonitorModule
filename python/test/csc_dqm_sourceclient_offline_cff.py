import FWCore.ParameterSet.Config as cms

from DQM.CSCMonitorModule.test.csc_dqm_sourceclient_cfi import *
from DQMOffline.Muon.CSCMonitor_cfi import *
from Configuration.StandardSequences.MagneticField_cff import *
from Configuration.StandardSequences.RawToDigi_Data_cff import *
from Configuration.StandardSequences.Reconstruction_cff import *

csc2DRecHits.readBadChambers = cms.bool(False)

#cscSources = cms.Sequence(dqmClient)
cscSources = cms.Sequence(muonCSCDigis * csc2DRecHits * cscSegments * cscMonitor * dqmClient)
