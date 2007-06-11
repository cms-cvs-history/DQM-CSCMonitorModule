#include "DQM/CSCMonitorModule/interface/CSCMonitor.h"

#include "DQMServices/Daemon/interface/MonitorDaemon.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

CSCMonitor::CSCMonitor(const edm::ParameterSet& iConfig )
{

  setParameters();

  printout = iConfig.getUntrackedParameter<bool>("monitorVerbosity", false);
  xmlHistosBookingCfgFile = iConfig.getUntrackedParameter<std::string>("BookingFile", "emuDQMBooking_new.xml"); 
  fSaveHistos  = iConfig.getUntrackedParameter<bool>("CSCDQMSaveRootFile", false);
  saveRootFileEventsInterval  = iConfig.getUntrackedParameter<int>("EventsInterval", 20000);
  RootHistoFile  = iConfig.getUntrackedParameter<std::string>("RootFileName", "CSCMonitor.root");
      
  this->loadBooking();

  dbe = edm::Service<DaqMonitorBEInterface>().operator->();

  edm::Service<MonitorDaemon> daemon;
  daemon.operator->();

  dbe->showDirStructure();
  gStyle->SetPalette(1,0);
}


void CSCMonitor::setParameters() 
{
  nEvents = 0;
  L1ANumber = 0;
  BXN = 0;
  unpackMask = UNPACK_ALL;
  nDMBEvents.clear();
  unpackedDMBcount = 0;
  logger_ = "CSC_DQM:";
  dduCheckMask = 0xFFFFFFFF;
  binCheckMask = 0xFFFFFFFF;
  xmlHistosBookingCfgFile = "";
}

void CSCMonitor::clearMECollection(ME_List & collection) 
{

  if (collection.size() > 0) {
    for (ME_List_iterator itr = collection.begin();itr != collection.end(); ++itr) {
      delete itr->second;
    }
    collection.clear();
  }

}
void CSCMonitor::printMECollection(ME_List & collection)
{
  int i = 0;
  for (ME_List_iterator itr = collection.begin();itr != collection.end(); ++itr) {
    LOG4CPLUS_DEBUG(logger_, ++i << ":" << itr->first << ":" << itr->second->getFullName());
  }

}


CSCMonitor::~CSCMonitor() {
  std::map<std::string, ME_List >::iterator itr;	
  for (itr = MEs.begin(); itr != MEs.end(); ++itr) {
    clearMECollection(itr->second);
  }

  MEs.clear();
  clearMECollection(commonMEfactory);
  clearMECollection(chamberMEfactory);
  clearMECollection(dduMEfactory);
}

void CSCMonitor::loadBooking() {
  if (MEs.size() > 0) {
    std::map<std::string, ME_List >::iterator itr;
    for (itr = MEs.begin(); itr != MEs.end(); ++itr) {
      clearMECollection(itr->second);
    }
    MEs.clear();
  }

  if (loadXMLBookingInfo(xmlHistosBookingCfgFile) == 0) 
    {
      setParameters();
    }

}

bool CSCMonitor::isMEvalid(ME_List& MEs, string name, CSCMonitorObject*& me, uint32_t mask)
{
  if ((unpackMask & mask)==0) return false;
  ME_List_iterator res = MEs.find(name);
  if (res != MEs.end() && (res->second != 0)) {
    me = res->second;
    return true;
  } else {
		
    // edm::LogWarning ("CSC DQM: ") << "Can not find ME " << name;
    LOG4CPLUS_WARN(logger_, "Can not find ME " << name);
    me = 0;
    return false;
  }
	
}

#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"

using namespace edm::serviceregistry;

typedef ParameterSetMaker<CSCMonitorInterface,CSCMonitor> maker;

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_SERVICE_MAKER(CSCMonitor,maker);



