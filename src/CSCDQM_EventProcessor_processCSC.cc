/*
 * =====================================================================================
 *
 *       Filename:  EventProcessor_processCSC.cc
 *
 *    Description:  Process Chamber
 *
 *        Version:  1.0
 *        Created:  10/03/2008 11:58:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Valdas Rapsevicius, valdas.rapsevicius@cern.ch
 *        Company:  CERN, CH
 *
 * =====================================================================================
 */

#include "DQM/CSCMonitorModule/interface/CSCDQM_EventProcessor.h"

namespace cscdqm {


  void EventProcessor::processCSC(const CSCEventData& data, const int dduID) {

    if (&data == 0) {
      LOG_ERROR << "Zero pointer. DMB data are not available for unpacking"; //KK is->are
      return;
    }

    int FEBunpacked   = 0;
    int alct_unpacked = 0;
    int tmb_unpacked  = 0;
    int cfeb_unpacked = 0;
  
    int alct_keywg = -1;
    int clct_kewdistrip = -1;
  
    bool L1A_out_of_sync = false;
  
    MonitorObject* mo = NULL;
  
    //  DMB Found
    //  Unpacking of DMB Header and trailer
    const CSCDMBHeader* dmbHeader = data.dmbHeader();
    const CSCDMBTrailer* dmbTrailer = data.dmbTrailer();
    if (!dmbHeader && !dmbTrailer) {
      LOG_ERROR << "Can not unpack DMB Header or/and Trailer";
      return;
    }
  
    // Unpacking of Chamber Identification number
    unsigned int crateID = 0xFF;
    unsigned int dmbID = 0xF;
    unsigned int chamberID = 0xFFF;
    
    crateID = dmbHeader->crateID();
    dmbID = dmbHeader->dmbID();
    chamberID = (((crateID) << 4) + dmbID) & 0xFFF;
  
    std::string cscTag = CSCHistoDef::getPath(crateID, dmbID);
  
    unsigned long errors = binChecker.errorsForChamber(chamberID);
    if ((errors & config->BINCHECK_MASK) > 0 ) {
      LOG_WARN << "Format Errors " << cscTag << ": 0x" << hex << errors << " Skipped CSC Unpacking";
      return;
    }
  
    if (crateID == 0 || dmbID == 0 || crateID > 60 || dmbID > 10) {
      LOG_ERROR << "Invalid CSC: " << cscTag << ". Skipping";
      return;
    }
  
    uint32_t detRawId = config->fnGetCSCDetId(crateID, dmbID).rawId();
    if (!detRawId) {
      return;
    }
    
    unsigned int cscType = 0;
    unsigned int cscPosition = 0;
    getCSCFromMap(crateID, dmbID, cscType, cscPosition );
  
    // if (cscName != "") cscTag+=" ["+cscName+"]";
    // nDMBEvents[cscTag]++;
    double DMBEvents  = 0.0;
    DMBEvents = nDMBEvents[cscTag];  
  
    CSCCounters& trigCnts = cscCntrs[cscTag];
    trigCnts["DMB"] = nDMBEvents[cscTag];
  
    if (cscType && cscPosition && getEMUHisto(h::EMU_CSC_UNPACKED, mo)){
      mo->Fill(cscPosition, cscType);
    }
  
    //    Efficiency of the chamber
    float DMBEff = float(DMBEvents) / float(config->getNEvents());
    if(DMBEff > 1.0) {
      LOG_ERROR <<   cscTag  << " has efficiency " << DMBEff << " which is greater than 1";
    }
    
    // Unpacking L1A number from DMB header
    // DMB L1A: 8bits (256)
    // DDU L1A: 24bits
    int dmbHeaderL1A      = dmbHeader->l1a()%64;
    // Calculate difference between L1A numbers from DDU and DMB
    int dmb_ddu_l1a_diff  = (int)(dmbHeaderL1A-(int)(L1ANumber%64));
    if (dmb_ddu_l1a_diff != 0) L1A_out_of_sync = true;

    LOG_DEBUG << dmbHeaderL1A << " : DMB L1A - DDU L1A = " << dmb_ddu_l1a_diff;
  
    if (getCSCHisto(h::CSC_DMB_L1A_DISTRIB, crateID, dmbID, mo)) mo->Fill(dmbHeaderL1A);
  
    if (getCSCHisto(h::CSC_DMB_DDU_L1A_DIFF, crateID, dmbID, mo)) {
      if(dmb_ddu_l1a_diff < -32) {
        mo->Fill(dmb_ddu_l1a_diff + 64);
      } else {
        if(dmb_ddu_l1a_diff > 32) mo->Fill(dmb_ddu_l1a_diff - 64);
        else mo->Fill(dmb_ddu_l1a_diff);
      }
      mo->SetAxisRange(0.1, 1.1 * (1.0 + mo->GetBinContent(mo->GetMaximumBin())), "Y");
    }
  
    if (getCSCHisto(h::CSC_DMB_L1A_VS_DDU_L1A, crateID, dmbID, mo)) mo->Fill((int)(L1ANumber & 0xFF), (int)dmbHeaderL1A);
  
    //    Unpacking BXN number from DMB header
    int dmbHeaderBXN      = 0;
    int dmb_ddu_bxn_diff  = 0;
    
    // == DMB BXN: 12bits (4096) call bxn12(), bxn() return 7bits value
    // == DDU BXN: 12bits (4096) 
  
    // == Use 6-bit BXN
    dmbHeaderBXN = dmbHeader->bxn12();
    // Calculation difference between BXN numbers from DDU and DMB
  
    //  dmb_ddu_bxn_diff = (int)(dmbHeaderBXN-(int)(BXN&0x7F)); // For older DMB
    dmb_ddu_bxn_diff = dmbHeaderBXN%64-BXN%64;
    LOG_DEBUG << dmbHeaderBXN << " : DMB BXN - DDU BXN = " << dmb_ddu_bxn_diff;
    if (getCSCHisto(h::CSC_DMB_BXN_DISTRIB, crateID, dmbID, mo)) mo->Fill((int)(dmbHeader->bxn12()));
  
    if (getCSCHisto(h::CSC_DMB_DDU_BXN_DIFF, crateID, dmbID, mo)) {
      if(dmb_ddu_bxn_diff < -32) mo->Fill(dmb_ddu_bxn_diff + 64);
      else {
        if(dmb_ddu_bxn_diff > 32)  mo->Fill(dmb_ddu_bxn_diff - 64);
        else mo->Fill(dmb_ddu_bxn_diff);
      }  
      mo->SetAxisRange(0.1, 1.1 * (1.0 + mo->GetBinContent(mo->GetMaximumBin())), "Y");
    }
  
    //  if (getCSCHisto(h::CSC_DMB_BXN_VS_DDU_BXN, crateID, dmbID, mo)) mo->Fill((int)(BXN), (int)dmbHeaderBXN);
    if (getCSCHisto(h::CSC_DMB_BXN_VS_DDU_BXN, crateID, dmbID, mo)) mo->Fill(((int)(BXN)) % 256, ((int)dmbHeaderBXN) % 256);
  
    //    Unpacking CFEB information from DMB header
    int cfeb_dav = 0;
    int cfeb_dav_num = 0;
    int cfeb_movlp = 0;
    int dmb_cfeb_sync = 0;
  
    cfeb_dav = (int)dmbHeader->cfebAvailable();
    for (int i = 0; i < 5; i++) cfeb_dav_num += (cfeb_dav >> i) & 0x1;
    cfeb_movlp    = (int)dmbHeader->cfebMovlp();
    dmb_cfeb_sync = (int)dmbHeader->dmbCfebSync();
  
    if (getCSCHisto(h::CSC_DMB_CFEB_DAV, crateID, dmbID, mo)) {
      for (int i = 0; i < 5; i++) {
        int cfeb_present = (cfeb_dav >> i) & 0x1;
        if (cfeb_present) {
          mo->Fill(i);
        }
      }
    }

    if (getCSCHisto(h::CSC_DMB_CFEB_DAV_MULTIPLICITY, crateID, dmbID, mo)) mo->Fill(cfeb_dav_num);
    if (getCSCHisto(h::CSC_DMB_CFEB_MOVLP, crateID, dmbID, mo)) mo->Fill(cfeb_movlp);
    if (getCSCHisto(h::CSC_DMB_CFEB_SYNC, crateID, dmbID, mo)) mo->Fill(dmb_cfeb_sync);
  
    if (getEMUHisto(h::EMU_DMB_UNPACKED, mo)) { 
      mo->Fill(crateID, dmbID);
      //  mo->SetEntries(config->getNEvents());
    }
  
    // if (getCSCHisto(h::CSC_DMB_CFEB_ACTIVE, crateID, dmbID, mo) mo->Fill((dmbTrailer->header_1a>>5)&0x1F); //KK
    if (getCSCHisto(h::CSC_DMB_CFEB_ACTIVE, crateID, dmbID, mo)) mo->Fill(dmbHeader->cfebActive()); //KK
  
    //if (getCSCHisto(h::CSC_DMB_CFEB_ACTIVE_VS_DAV, crateID, dmbID, mo)) mo->Fill(dmbHeader->cfebAvailable(),(int)((dmbTrailer->header_1a>>5)&0x1F)); //KK
    // if (getCSCHisto(h::CSC_DMB_CFEB_ACTIVE_VS_DAV, crateID, dmbID, mo)) mo->Fill(dmbHeader->cfebAvailable(),dmbHeader->cfebActive()); //KK
  
    if (getCSCHisto(h::CSC_DMB_L1_PIPE, crateID, dmbID, mo)) mo->Fill(dmbTrailer->dmb_l1pipe);
  
    // DMB input (7 in total) FIFO stuff goes here
    if (getCSCHisto(h::CSC_DMB_FIFO_STATS, crateID, dmbID, mo)) {
      if (dmbTrailer->tmb_empty == 1) mo->Fill(1.0, 0.0); //KK
      if (dmbTrailer->tmb_half == 0) mo->Fill(1.0, 1.0);
      if (dmbTrailer->tmb_full == 1) mo->Fill(1.0, 2.0); //KK
      if (dmbTrailer->alct_empty == 1) mo->Fill(0.0, 0.0);
      if (dmbTrailer->alct_half == 0) mo->Fill(0.0, 1.0);
      if (dmbTrailer->alct_full == 1) mo->Fill(0.0, 2.0); //KK 0->1
      for (int i = 0; i < 5; i++) {
        if ((int)((dmbTrailer->cfeb_empty>>i)&0x1) == 1) mo->Fill(i + 2, 0.0);
        if ((int)((dmbTrailer->cfeb_half>>i)&0x1) == 0) mo->Fill(i + 2, 1);
        if ((int)((dmbTrailer->cfeb_full>>i)&0x1) == 1) {
          mo->Fill(i + 2, 2);
        }
      }
      mo->SetEntries((int)DMBEvents);
    }
  
    // DMB input timeout (total 15 bits) goes here
    if (getCSCHisto(h::CSC_DMB_FEB_TIMEOUTS, crateID, dmbID, mo)) {
      if ((dmbTrailer->tmb_timeout == 0) && (dmbTrailer->alct_timeout == 0) && (dmbTrailer->cfeb_starttimeout == 0) && (dmbTrailer->cfeb_endtimeout == 0)) {
        mo->Fill(0.0);
      }else{
        if (dmbTrailer->alct_timeout) mo->Fill(1);
        if (dmbTrailer->tmb_timeout) mo->Fill(2);
        if (dmbTrailer->alct_endtimeout) mo->Fill(8); // KK
        if (dmbTrailer->tmb_endtimeout) mo->Fill(9);  // KK
      }
      for (int i = 0; i < 5; i++) {
        if ((dmbTrailer->cfeb_starttimeout >> i) & 0x1) {
          mo->Fill(i + 3);
        }
        if ((dmbTrailer->cfeb_endtimeout >> i) & 0x1) {
          mo->Fill(i + 10); // KK 8->10
        }
      }
      mo->SetEntries((int)DMBEvents);
    }
  
    //      Get FEBs Data Available Info
    int alct_dav  = dmbHeader->nalct();
    int tmb_dav   = dmbHeader->nclct();
    int cfeb_dav2 = 0;
    for (int i = 0; i < 5; i++) cfeb_dav2 = cfeb_dav2 + (int)((dmbHeader->cfebAvailable() >> i) & 0x1);
  
    //      Fill Hisogram for FEB DAV Efficiency
  
    if ((alct_dav  > 0) && (getCSCHisto(h::CSC_DMB_FEB_DAV_RATE, crateID, dmbID, mo))) {
      mo->Fill(0.0);
      float alct_dav_number   = mo->GetBinContent(1);
      if (getCSCHisto(h::CSC_DMB_FEB_DAV_EFFICIENCY, crateID, dmbID, mo)) {
        mo->SetBinContent(1, ((float)alct_dav_number / (float)(DMBEvents) * 100.0));
        mo->SetEntries((int)DMBEvents);
      }
    }
  
    if ((tmb_dav > 0) && (getCSCHisto(h::CSC_DMB_FEB_DAV_RATE, crateID, dmbID, mo))) {
      mo->Fill(1.0);
      float tmb_dav_number = mo->GetBinContent(2);
      if (getCSCHisto(h::CSC_DMB_FEB_DAV_EFFICIENCY, crateID, dmbID, mo)) {
        mo->SetBinContent(2, ((float)tmb_dav_number / (float)(DMBEvents) * 100.0));
        mo->SetEntries((int)DMBEvents);
      }
    }
    
    if ((cfeb_dav2 > 0) && (getCSCHisto(h::CSC_DMB_FEB_DAV_RATE, crateID, dmbID, mo))) {
      mo->Fill(2.0);
      float cfeb_dav2_number = mo->GetBinContent(3);
      if (getCSCHisto(h::CSC_DMB_FEB_DAV_EFFICIENCY, crateID, dmbID, mo)) {
        mo->SetBinContent(3, ((float)cfeb_dav2_number / (float)(DMBEvents) * 100.0));
        mo->SetEntries((int)DMBEvents);
      }
    }
  
    float feb_combination_dav = -1.0;
    //      Fill Hisogram for Different Combinations of FEB DAV Efficiency
    if (getCSCHisto(h::CSC_DMB_FEB_COMBINATIONS_DAV_RATE, crateID, dmbID, mo)) {
       if(alct_dav == 0 && tmb_dav == 0 && cfeb_dav2 == 0) feb_combination_dav = 0.0; // Nothing
    if(alct_dav >  0 && tmb_dav == 0 && cfeb_dav2 == 0) feb_combination_dav = 1.0; // ALCT Only
    if(alct_dav == 0 && tmb_dav >  0 && cfeb_dav2 == 0) feb_combination_dav = 2.0; // TMB Only
    if(alct_dav == 0 && tmb_dav == 0 && cfeb_dav2 >  0) feb_combination_dav = 3.0; // CFEB Only
    if(alct_dav == 0 && tmb_dav >  0 && cfeb_dav2 >  0) feb_combination_dav = 4.0; // TMB+CFEB
    if(alct_dav >  0 && tmb_dav >  0 && cfeb_dav2 == 0) feb_combination_dav = 5.0; // ALCT+TMB
    if(alct_dav >  0 && tmb_dav == 0 && cfeb_dav2 >  0) feb_combination_dav = 6.0; // ALCT+CFEB
    if(alct_dav >  0 && tmb_dav >  0 && cfeb_dav2 >  0) feb_combination_dav = 7.0; // ALCT+TMB+CFEB
      mo->Fill(feb_combination_dav);
      float feb_combination_dav_number = mo->GetBinContent((int)(feb_combination_dav + 1.0));
      if (getCSCHisto(h::CSC_DMB_FEB_COMBINATIONS_DAV_EFFICIENCY, crateID, dmbID, mo)) {
        mo->SetBinContent((int)(feb_combination_dav + 1.0), ((float)feb_combination_dav_number / (float)(DMBEvents) * 100.0));
        mo->SetEntries((int)DMBEvents);
      }
    }
  
    //ALCT Found
    if (data.nalct()) {
      const CSCALCTHeader* alctHeader = data.alctHeader();
      const CSCALCTTrailer* alctTrailer = data.alctTrailer();
      const CSCAnodeData* alctData = data.alctData();
      if (alctHeader && alctTrailer) {
        std::vector<CSCALCTDigi> alctsDatasTmp = alctHeader->ALCTDigis();
        std::vector<CSCALCTDigi> alctsDatas;
  
        for (uint32_t lct = 0; lct < alctsDatasTmp.size(); lct++) {
          if (alctsDatasTmp[lct].isValid())
            alctsDatas.push_back(alctsDatasTmp[lct]);
        }
  
        FEBunpacked = FEBunpacked + 1;
        alct_unpacked = 1;
  
        // Set number of ALCT-events to third bin
        if (getCSCHisto(h::CSC_CSC_RATE, crateID, dmbID, mo)) { 
          mo->Fill(2);
          // trigCnts.ALCTcnt++;
          uint32_t ALCTEvent = (uint32_t)mo->GetBinContent(3);
          trigCnts["ALCT"] = ALCTEvent;
          if (getCSCHisto(h::CSC_CSC_EFFICIENCY, crateID, dmbID, mo)){
            if(config->getNEvents() > 0) {
              //KK
              //h[hname]->SetBinContent(3, ((float)ALCTEvent/(float)(config->getNEvents()) * 100.0));
              mo->SetBinContent(1, ((float)ALCTEvent / (float)(DMBEvents) * 100.0));
              //KKend
              mo->SetEntries((int)DMBEvents);
            }
          }
        }
  
        if ((alct_dav >0) && (getCSCHisto(h::CSC_DMB_FEB_UNPACKED_VS_DAV, crateID, dmbID, mo))) {
          mo->Fill(0.0, 0.0);
        }
    
        // == ALCT2007 L1A: 12bits (4096)
        // == ALCT2006 L1A: 4bits (16)
        if (getCSCHisto(h::CSC_ALCT_L1A, crateID, dmbID, mo)) mo->Fill((int)(alctHeader->L1Acc()));
    
        // == Use 6-bit L1A      
        if (getCSCHisto(h::CSC_ALCT_DMB_L1A_DIFF, crateID, dmbID, mo)) {
          // int alct_dmb_l1a_diff = (int)((dmbHeader->l1a()&0xF)-alctHeader->L1Acc());
          int alct_dmb_l1a_diff = (int)(alctHeader->L1Acc() % 64 - dmbHeader->l1a() % 64);
          if (alct_dmb_l1a_diff != 0) L1A_out_of_sync = true;
          if(alct_dmb_l1a_diff < -32) mo->Fill(alct_dmb_l1a_diff + 64);
          else {
            if(alct_dmb_l1a_diff > 32) mo->Fill(alct_dmb_l1a_diff - 64);
            else mo->Fill(alct_dmb_l1a_diff);
          }
          mo->SetAxisRange(0.1, 1.1 * (1.0 + mo->GetBinContent(mo->GetMaximumBin())), "Y");
        }
  
        // if (getCSCHisto(h::CSC_DMB_L1A_VS_ALCT_L1A, crateID, dmbID, mo)) mo->Fill(alctHeader->L1Acc(),dmbHeader->l1a());
        if (getCSCHisto(h::CSC_DMB_L1A_VS_ALCT_L1A, crateID, dmbID, mo)) mo->Fill(alctHeader->L1Acc() % 256, dmbHeader->l1a());
  
        // === ALCT BXN: 12bits (4096)
        // === Use 6-bit BXN
        if (getCSCHisto(h::CSC_ALCT_DMB_BXN_DIFF, crateID, dmbID, mo)) {
          int alct_dmb_bxn_diff = (int)(alctHeader->BXNCount() % 64 - dmbHeader->bxn12() % 64);
          if(alct_dmb_bxn_diff < -32) mo->Fill(alct_dmb_bxn_diff + 64);
          else {
            if(alct_dmb_bxn_diff > 32)  mo->Fill(alct_dmb_bxn_diff - 64);
            else mo->Fill(alct_dmb_bxn_diff);
          }
          mo->SetAxisRange(0.1, 1.1 * (1.0 + mo->GetBinContent(mo->GetMaximumBin())), "Y");
        }
  
        if (getCSCHisto(h::CSC_ALCT_BXN, crateID, dmbID, mo)) mo->Fill(alctHeader->BXNCount());
  
        // if (getCSCHisto(h::CSC_ALCT_BXN_VS_DMB_BXN, crateID, dmbID, mo)) mo->Fill((int)((alctHeader->BXNCount())), (int)(dmbHeader->bxn()));
        if (getCSCHisto(h::CSC_ALCT_BXN_VS_DMB_BXN, crateID, dmbID, mo)) mo->Fill((int)((alctHeader->BXNCount()) % 256), (int)(dmbHeader->bxn12()) % 256);
  
        if (getCSCHisto(h::CSC_ALCT_NUMBER_RATE, crateID, dmbID, mo)) {
          mo->Fill(alctsDatas.size());
          int nALCT = (int)mo->GetBinContent((int)(alctsDatas.size() + 1));
          if (getCSCHisto(h::CSC_ALCT_NUMBER_EFFICIENCY, crateID, dmbID, mo)) 
            mo->SetBinContent((int)(alctsDatas.size() + 1), (float)(nALCT) / (float)(DMBEvents) * 100.0);
        }
  
        if (getCSCHisto(h::CSC_ALCT_WORD_COUNT, crateID, dmbID, mo)) mo->Fill((int)(alctTrailer->wordCount()));
  
        LOG_DEBUG <<  "ALCT Trailer Word Count = " << dec << (int)alctTrailer->wordCount();
  
        if (alctsDatas.size() == 2) {
          if (getCSCHisto(h::CSC_ALCT1_VS_ALCT0_KEYWG, crateID, dmbID, mo)) 
            mo->Fill(alctsDatas[0].getKeyWG(),alctsDatas[1].getKeyWG());
        }
  
        for (uint32_t lct = 0; lct < alctsDatas.size(); lct++) {
          // TODO: Add support for more than 2 ALCTs
          if (lct >= 2) continue;	
          if (getCSCHisto(h::CSC_ALCTXX_KEYWG, crateID, dmbID, lct, mo)) 
            mo->Fill(alctsDatas[lct].getKeyWG());
          if(lct == 0) alct_keywg  = alctsDatas[lct].getKeyWG();
          int alct_dtime = (int)(alctsDatas[lct].getBX() - (alctHeader->BXNCount() & 0x1F));
          if (getCSCHisto(h::CSC_ALCTXX_DTIME, crateID, dmbID, lct, mo)) {
            if(alct_dtime < -16) {
              mo->Fill(alct_dtime + 32);
            } else {
              if(alct_dtime > 16)  mo->Fill(alct_dtime - 32);
              else mo->Fill(alct_dtime);
            }
            mo->SetAxisRange(0.1, 1.1 * (1.0 + mo->GetBinContent(mo->GetMaximumBin())), "Y");
          }
  
          if (getCSCHisto(h::CSC_ALCTXX_DTIME_VS_KEYWG, crateID, dmbID, lct, mo)) {
            if(alct_dtime < -16) {
              mo->Fill(alctsDatas[lct].getKeyWG(), alct_dtime + 32);
            } else {
              if(alct_dtime > 16) mo->Fill(alctsDatas[lct].getKeyWG(), alct_dtime - 32);
              else mo->Fill(alctsDatas[lct].getKeyWG(), alct_dtime);
            }
          }
  
          if (getCSCHisto(h::CSC_ALCTXX_DTIME_PROFILE, crateID, dmbID, lct, mo)) {
            if(alct_dtime < -16) {
              mo->Fill(alctsDatas[lct].getKeyWG(), alct_dtime + 32);
            } else {
              if(alct_dtime > 16) mo->Fill(alctsDatas[lct].getKeyWG(), alct_dtime - 32);
              else mo->Fill(alctsDatas[lct].getKeyWG(), alct_dtime);
            }
          }
    
          if (getCSCHisto(h::CSC_ALCTXX_BXN, crateID, dmbID, lct, mo)) mo->Fill(alctsDatas[lct].getBX());
    
          if (getCSCHisto(h::CSC_ALCTXX_QUALITY, crateID, dmbID, lct, mo)) mo->Fill(alctsDatas[lct].getKeyWG(), alctsDatas[lct].getQuality());
  
          if (getCSCHisto(h::CSC_ALCTXX_QUALITY_PROFILE, crateID, dmbID, lct, mo)) mo->Fill(alctsDatas[lct].getKeyWG(), alctsDatas[lct].getQuality());
  
          if (getCSCHisto(h::CSC_ALCTXX_PATTERN, crateID, dmbID, lct, mo)) {
            int pattern = (alctsDatas[lct].getAccelerator() << 1) + alctsDatas[lct].getCollisionB();
            int keywg = alctsDatas[lct].getKeyWG();
            mo->Fill(keywg, pattern );
          }
        }

        int NumberOfLayersWithHitsInALCT = 0;
        int NumberOfWireGroupsWithHitsInALCT = 0;

        if (alctData) {
          for (int nLayer=1; nLayer<=6; nLayer++) {
            int wg_previous   = -1;
            int tbin_previous = -1;
            bool CheckLayerALCT = true;
            std::vector<CSCWireDigi> wireDigis = alctData->wireDigis(nLayer);
            for (std::vector<CSCWireDigi>:: iterator wireDigisItr = wireDigis.begin(); wireDigisItr != wireDigis.end(); ++wireDigisItr) {
              int wg = wireDigisItr->getWireGroup();
              // int tbin = wireDigisItr->getBeamCrossingTag();
              std::vector<int> tbins = wireDigisItr->getTimeBinsOn();
              int tbin = wireDigisItr->getTimeBin();
              if(CheckLayerALCT) {
                NumberOfLayersWithHitsInALCT = NumberOfLayersWithHitsInALCT + 1;
                CheckLayerALCT = false;
              }
              for (uint32_t n=0; n < tbins.size(); n++) {
                tbin = tbins[n];
                if(wg != wg_previous || (tbin != tbin_previous + 1 && tbin != tbin_previous - 1) ) {
                  if (getCSCHisto(h::CSC_ALCTTIME_LYXX, crateID, dmbID, nLayer, mo)) mo->Fill(wg, tbin);
                  if (getCSCHisto(h::CSC_ALCTTIME_LYXX_PROFILE, crateID, dmbID, nLayer, mo)) mo->Fill(wg, tbin);
                  if (getCSCHisto(h::CSC_ALCT_LYXX_RATE, crateID, dmbID, nLayer, mo)) { 
                    mo->Fill(wg);
                    int number_wg = (int)(mo->GetBinContent(wg+1));
                    Double_t Number_of_entries_ALCT = mo->GetEntries();
                    if (getCSCHisto(h::CSC_ALCT_LYXX_EFFICIENCY, crateID, dmbID, nLayer, mo)) {
                      mo->SetBinContent(wg + 1, ((float) number_wg));
                      if((Double_t)(DMBEvents) > 0.0) {
                        mo->SetNormFactor(100.0*Number_of_entries_ALCT/(Double_t)(DMBEvents));
                      } else {
                        mo->SetNormFactor(100.0);
                      }
                      mo->SetEntries((int)DMBEvents);
                    }
                  }
                }
                if(wg != wg_previous) {
                  NumberOfWireGroupsWithHitsInALCT = NumberOfWireGroupsWithHitsInALCT + 1;
                }
  
                wg_previous   = wg;
                tbin_previous = tbin;
              }
            }
          }
        } else {
          LOG_ERROR << cscTag << " Can not unpack Anode Data";
        }
        if (getCSCHisto(h::CSC_ALCT_NUMBER_OF_LAYERS_WITH_HITS, crateID, dmbID, mo)) mo->Fill(NumberOfLayersWithHitsInALCT);    
        if (getCSCHisto(h::CSC_ALCT_NUMBER_OF_WIREGROUPS_WITH_HITS, crateID, dmbID, mo)) mo->Fill(NumberOfWireGroupsWithHitsInALCT);
      } else {
        LOG_ERROR << cscTag << " Can not unpack ALCT Header or/and Trailer";
      }
    } else {
      //  ALCT not found
  
      if (getCSCHisto(h::CSC_ALCT_NUMBER_RATE, crateID, dmbID, mo)) { 
        mo->Fill(0);
        int nALCT = (int)mo->GetBinContent(1);
        if (getCSCHisto(h::CSC_ALCT_NUMBER_EFFICIENCY, crateID, dmbID, mo)) 
          mo->SetBinContent(1, (float)(nALCT) / (float)(DMBEvents) * 100.0);
      }
  
      if ((alct_dav  > 0) && (getCSCHisto(h::CSC_DMB_FEB_UNPACKED_VS_DAV, crateID, dmbID, mo))) {
        mo->Fill(0.0, 1.0);
      }
    }
  
    //ALCT and CLCT coinsidence
    if(data.nclct() && data.nalct()) {
    
      CSCALCTHeader* alctHeader = data.alctHeader();
      
      if (alctHeader) {
        std::vector<CSCALCTDigi> alctsDatasTmp = alctHeader->ALCTDigis();
        std::vector<CSCALCTDigi> alctsDatas;
  
        for (uint32_t lct = 0; lct < alctsDatasTmp.size(); lct++) {
          if (alctsDatasTmp[lct].isValid())
            alctsDatas.push_back(alctsDatasTmp[lct]);
        }
  
        CSCTMBData* tmbData = data.tmbData();
        if (tmbData) {
          CSCTMBHeader* tmbHeader = tmbData->tmbHeader();
          if (tmbHeader) {
            // if (getCSCHisto(h::CSC_TMB_BXN_VS_ALCT_BXN, crateID, dmbID, mo)) mo->Fill((int)((alctHeader->BXNCount())),(int)(tmbHeader->BXNCount()));
            if (getCSCHisto(h::CSC_TMB_BXN_VS_ALCT_BXN, crateID, dmbID, mo))
              mo->Fill( ((int)(alctHeader->BXNCount())) % 256, ((int)(tmbHeader->BXNCount())) % 256 );
  
            if (getCSCHisto(h::CSC_TMB_ALCT_BXN_DIFF, crateID, dmbID, mo)) {
              int clct_alct_bxn_diff = (int)(alctHeader->BXNCount() - tmbHeader->BXNCount());
              if(clct_alct_bxn_diff < -2048) mo->Fill(clct_alct_bxn_diff + 4096);
              else {
                if(clct_alct_bxn_diff > 2048) mo->Fill(clct_alct_bxn_diff - 4096);
                else mo->Fill(clct_alct_bxn_diff);
              }
              mo->SetAxisRange(0.1, 1.1*(1.0+mo->GetBinContent(mo->GetMaximumBin())), "Y");
            }
  
            if (getCSCHisto(h::CSC_TMB_L1A_VS_ALCT_L1A, crateID, dmbID, mo)) 
              mo->Fill((int)(alctHeader->L1Acc()%256),(int)(tmbHeader->L1ANumber()%256));
  
            if (getCSCHisto(h::CSC_TMB_ALCT_L1A_DIFF, crateID, dmbID, mo)) {
              int clct_alct_l1a_diff = (int)(tmbHeader->L1ANumber() - alctHeader->L1Acc());
              if(clct_alct_l1a_diff < -2048) mo->Fill(clct_alct_l1a_diff + 4096);
              else {
                if(clct_alct_l1a_diff > 2048)  mo->Fill(clct_alct_l1a_diff - 4096);
                else mo->Fill(clct_alct_l1a_diff);
              }
              mo->SetAxisRange(0.1, 1.1*(1.0+mo->GetBinContent(mo->GetMaximumBin())), "Y");
            }
          } else {
            LOG_ERROR << cscTag << " Can not unpack TMB Header";
          }
    
        } else {
          LOG_ERROR << cscTag << " Can not unpack TMB Data";
        }
      } else {
        LOG_ERROR << cscTag << " Can not unpack ALCT Header";
      }
    }
  
    //CLCT Found
    if (data.nclct()) {
  
      //LOG_WARN << "TMB CRC calc: 0x" << hex << data.tmbData().TMBCRCcalc() << " trailer: 0x" << hex << data.tmbData().tmbTrailer().crc22();
  
      CSCTMBData* tmbData = data.tmbData();
      if (tmbData) {
  
        CSCTMBHeader* tmbHeader = tmbData->tmbHeader();
        CSCTMBTrailer* tmbTrailer = tmbData->tmbTrailer();
  
        if (tmbHeader && tmbTrailer) {
  
          CSCCLCTData* clctData = data.clctData();
  
          std::vector<CSCCLCTDigi> clctsDatasTmp = tmbHeader->CLCTDigis(detRawId);
          std::vector<CSCCLCTDigi> clctsDatas;
  
          for (uint32_t lct = 0; lct < clctsDatasTmp.size(); lct++) {
            if (clctsDatasTmp[lct].isValid()) clctsDatas.push_back(clctsDatasTmp[lct]);
          }
  
          FEBunpacked = FEBunpacked +1;
          tmb_unpacked = 1;
  
          if (getCSCHisto(h::CSC_ALCT_MATCH_TIME, crateID, dmbID, mo)) mo->Fill(tmbHeader->ALCTMatchTime());
  
          if (getCSCHisto(h::CSC_LCT_MATCH_STATUS, crateID, dmbID, mo)) {
            if (tmbHeader->CLCTOnly()) mo->Fill(0.0,0.0);
            if (tmbHeader->ALCTOnly()) mo->Fill(0.0,1.0);
            if (tmbHeader->TMBMatch()) mo->Fill(0.0,2.0);
          }
    
          if (getCSCHisto(h::CSC_LCT0_MATCH_BXN_DIFFERENCE, crateID, dmbID, mo)) mo->Fill(tmbHeader->Bxn0Diff());
          if (getCSCHisto(h::CSC_LCT1_MATCH_BXN_DIFFERENCE, crateID, dmbID, mo)) mo->Fill(tmbHeader->Bxn1Diff());
  
          if ((tmb_dav  > 0) && (getCSCHisto(h::CSC_DMB_FEB_UNPACKED_VS_DAV, crateID, dmbID, mo))) {
            mo->Fill(1.0, 0.0);
          }
  
          // Set number of CLCT-events to forth bin
          if (getCSCHisto(h::CSC_CSC_RATE, crateID, dmbID, mo)) {
            mo->Fill(3);
            uint32_t CLCTEvent = (uint32_t)mo->GetBinContent(4);
            trigCnts["CLCT"] = CLCTEvent;
            if (getCSCHisto(h::CSC_CSC_EFFICIENCY, crateID, dmbID, mo)) {
              if(config->getNEvents() > 0) {
                mo->SetBinContent(2,((float)CLCTEvent/(float)(DMBEvents)*100.0));
                mo->SetEntries(DMBEvents);
              }
            }
          }
  
          if (getCSCHisto(h::CSC_CLCT_L1A, crateID, dmbID, mo)) mo->Fill(tmbHeader->L1ANumber());
  
          // Use 6-bit L1A
          if (getCSCHisto(h::CSC_CLCT_DMB_L1A_DIFF, crateID, dmbID, mo)) {
            int clct_dmb_l1a_diff = (int)((tmbHeader->L1ANumber() % 64)-dmbHeader->l1a() % 64);
            if (clct_dmb_l1a_diff != 0) L1A_out_of_sync = true;
            if(clct_dmb_l1a_diff < -32) mo->Fill(clct_dmb_l1a_diff + 64);
            else {
              if(clct_dmb_l1a_diff > 32)  mo->Fill(clct_dmb_l1a_diff - 64);
              else mo->Fill(clct_dmb_l1a_diff);
            }
            mo->SetAxisRange(0.1, 1.1*(1.0+mo->GetBinContent(mo->GetMaximumBin())), "Y");
          }
  
          // if (getCSCHisto(h::CSC_DMB_L1A_VS_CLCT_L1A, crateID, dmbID, mo)) mo->Fill(tmbHeader->L1ANumber(),dmbHeader->l1a());
          if (getCSCHisto(h::CSC_DMB_L1A_VS_CLCT_L1A, crateID, dmbID, mo)) mo->Fill(tmbHeader->L1ANumber()%256,dmbHeader->l1a());
  
          if (getCSCHisto(h::CSC_CLCT_DMB_BXN_DIFF, crateID, dmbID, mo)) {
            int clct_dmb_bxn_diff = (int)(tmbHeader->BXNCount()%64-dmbHeader->bxn12()%64);
            if(clct_dmb_bxn_diff < -32) mo->Fill(clct_dmb_bxn_diff + 64);
            else {
              if(clct_dmb_bxn_diff > 32)  mo->Fill(clct_dmb_bxn_diff - 64);
              else mo->Fill(clct_dmb_bxn_diff);
            }
            mo->SetAxisRange(0.1, 1.1*(1.0+mo->GetBinContent(mo->GetMaximumBin())), "Y");
          }
  
          if (getCSCHisto(h::CSC_CLCT_BXN, crateID, dmbID, mo)) mo->Fill((int)(tmbHeader->BXNCount()));
  
          // if (getCSCHisto(h::CSC_CLCT_BXN_VS_DMB_BXN, crateID, dmbID, mo)) mo->Fill(tmbHeader->BXNCount(),dmbHeader->bxn());
          if (getCSCHisto(h::CSC_CLCT_BXN_VS_DMB_BXN, crateID, dmbID, mo)) mo->Fill(tmbHeader->BXNCount()%256,dmbHeader->bxn12()%256);
  
          if (getCSCHisto(h::CSC_CLCT_NUMBER_RATE, crateID, dmbID, mo)) {
            mo->Fill(clctsDatas.size());
            int nCLCT = (int)mo->GetBinContent((int)(clctsDatas.size()+1));
            if (getCSCHisto(h::CSC_CLCT_NUMBER, crateID, dmbID, mo)) mo->SetBinContent((int)(clctsDatas.size() + 1), (float)(nCLCT) / (float)(DMBEvents) * 100.0);
          }
    
          if (clctsDatas.size() == 1) {
            if (getCSCHisto(h::CSC_CLCT0_CLSSIFICATION, crateID, dmbID, mo)) {
              if (clctsDatas[0].getStripType()) mo->Fill(0.0);
              else mo->Fill(1.0);
            }
          }
      
          if (clctsDatas.size() == 2) {
            if (getCSCHisto(h::CSC_CLCT1_VS_CLCT0_KEY_STRIP, crateID, dmbID, mo)) 
              mo->Fill(clctsDatas[0].getKeyStrip(),clctsDatas[1].getKeyStrip());
            if (getCSCHisto(h::CSC_CLCT0_CLCT1_CLSSIFICATION, crateID, dmbID, mo)) {
              if ( clctsDatas[0].getStripType() &&  clctsDatas[1].getStripType())       mo->Fill(0.0);
              if ( clctsDatas[0].getStripType() && !clctsDatas[1].getStripType())       mo->Fill(1.0);
              if (!clctsDatas[0].getStripType() &&  clctsDatas[1].getStripType())       mo->Fill(2.0);
              if (!clctsDatas[0].getStripType() &&  !clctsDatas[1].getStripType())      mo->Fill(3.0);
            }
          }
  
          if (getCSCHisto(h::CSC_TMB_WORD_COUNT, crateID, dmbID, mo)) mo->Fill((int)(tmbTrailer->wordCount()));
          LOG_DEBUG <<  "TMB Trailer Word Count = " << dec << (int)tmbTrailer->wordCount();
    
          for (uint32_t lct = 0; lct < clctsDatas.size(); lct++) {
  
          if (getCSCHisto(h::CSC_CLCTXX_BXN, crateID, dmbID, lct, mo)) mo->Fill(clctsDatas[lct].getBX());
  
          int clct_dtime = (int)(clctsDatas[lct].getBX() - (tmbHeader->BXNCount()&0x3));
          if (getCSCHisto(h::CSC_CLCTXX_DTIME, crateID, dmbID, lct, mo)) {
              if(clct_dtime < -2) mo->Fill(clct_dtime + 4);
            else {
              if(clct_dtime > 2)  mo->Fill(clct_dtime - 4);
              else mo->Fill(clct_dtime);
            }
            mo->SetAxisRange(0.1, 1.1 * (1.0 + mo->GetBinContent(mo->GetMaximumBin())), "Y");
          }
  
          LOG_DEBUG << "CLCT BX = " << clctsDatas[lct].getBX() << " TMB BX = " << tmbHeader->BXNCount() << " 03 = " << (int)(tmbHeader->BXNCount()&0x3);
          LOG_DEBUG <<  "diff = " << clctsDatas[lct].getBX()-(tmbHeader->BXNCount()&0x3);
          LOG_DEBUG <<  "LCT:" << lct << " Type:" << clctsDatas[lct].getStripType() << " Strip:" << clctsDatas[lct].getKeyStrip();
          
          if (clctsDatas[lct].getStripType()) { // HalfStrip Type
            if (getCSCHisto(h::CSC_CLCTXX_KEYHALFSTRIP, crateID, dmbID, lct, mo)) 
              mo->Fill(clctsDatas[lct].getKeyStrip());
            if (getCSCHisto(h::CSC_CLCTXX_DTIME_VS_HALF_STRIP, crateID, dmbID, lct, mo)) {
              if(clct_dtime < -2) mo->Fill((int)(clctsDatas[lct].getKeyStrip()), clct_dtime + 4);
              else {
                if(clct_dtime > 2) mo->Fill((int)(clctsDatas[lct].getKeyStrip()), clct_dtime - 4);
                else mo->Fill((int)(clctsDatas[lct].getKeyStrip()), clct_dtime);
              }
            }
    
            if (getCSCHisto(h::CSC_CLCTXX_HALF_STRIP_PATTERN, crateID, dmbID, lct, mo)) {
              int pattern_clct = clctsDatas[lct].getPattern();
              // int pattern_clct = (int)((clctsDatas[lct].getPattern()>>1)&0x3);
              // pattern_clct = Number of patterns in CLCT
              // Last (left) bit is bend. Positive bend = 1, negative bend = 0
              double tbin = -1;
              switch (pattern_clct) {
                case 0:  tbin=0.; break;
                case 1:  tbin=1.; break;
                case 2:  tbin=2.; break;
                case 3:  tbin=10.; break;
                case 4:  tbin=3.; break;
                case 5:  tbin=9.; break;
                case 6:  tbin=4.; break;
                case 7:  tbin=8.; break;
                case 8:  tbin=5.; break;
                case 9:  tbin=7.; break;
                case 10: tbin=6.; break;
              }
              if (tbin >= 0) mo->Fill(clctsDatas[lct].getKeyStrip(), tbin);
            }
  
            if (getCSCHisto(h::CSC_CLCTXX_HALF_STRIP_QUALITY, crateID, dmbID, lct, mo)) 
              mo->Fill((int)(clctsDatas[lct].getKeyStrip()),(int)(clctsDatas[lct].getQuality()));
  
            if (getCSCHisto(h::CSC_CLCTXX_HALF_STRIP_QUALITY_PROFILE, crateID, dmbID, lct, mo))
              mo->Fill((int)(clctsDatas[lct].getKeyStrip()), (int)(clctsDatas[lct].getQuality()));
  
          } else { // DiStrip Type
  
            if (getCSCHisto(h::CSC_CLCTXX_KEYDISTRIP, crateID, dmbID, lct, mo)) mo->Fill(clctsDatas[lct].getKeyStrip());
    
            if(lct == 0) clct_kewdistrip = clctsDatas[lct].getKeyStrip();
  
            if (getCSCHisto(h::CSC_CLCTXX_DTIME_VS_DISTRIP, crateID, dmbID, lct, mo)) {
              if(clct_dtime < -2) mo->Fill((int)(clctsDatas[lct].getKeyStrip()), clct_dtime + 4);
              else {
                if(clct_dtime > 2) mo->Fill((int)(clctsDatas[lct].getKeyStrip()), clct_dtime - 4);
                else mo->Fill((int)(clctsDatas[lct].getKeyStrip()), clct_dtime);
              }
            }
    
            if (getCSCHisto(h::CSC_CLCTXX_DISTRIP_PATTERN, crateID, dmbID, lct, mo)) {
              int pattern_clct = (int)((clctsDatas[lct].getPattern() >> 1) & 0x3);
              // pattern_clct = Number of patterns in CLCT
              // Last (left) bit is bend. Positive bend = 1, negative bend = 0
              if(pattern_clct == 1) mo->Fill(clctsDatas[lct].getKeyStrip(), 7.0);
              if(pattern_clct == 3) mo->Fill(clctsDatas[lct].getKeyStrip(), 6.0);
              if(pattern_clct == 5) mo->Fill(clctsDatas[lct].getKeyStrip(), 5.0);
              if(pattern_clct == 7) mo->Fill(clctsDatas[lct].getKeyStrip(), 4.0);
              if(pattern_clct == 6) mo->Fill(clctsDatas[lct].getKeyStrip(), 3.0);
              if(pattern_clct == 4) mo->Fill(clctsDatas[lct].getKeyStrip(), 2.0);
              if(pattern_clct == 2) mo->Fill(clctsDatas[lct].getKeyStrip(), 1.0);
              if(pattern_clct == 0) mo->Fill(clctsDatas[lct].getKeyStrip(), 0.0);
            }
  
            if (getCSCHisto(h::CSC_CLCTXX_DISTRIP_QUALITY, crateID, dmbID, lct, mo)) 
              mo->Fill((int)(clctsDatas[lct].getKeyStrip()),(int)(clctsDatas[lct].getQuality()));
  
            if (getCSCHisto(h::CSC_CLCTXX_DISTRIP_QUALITY_PROFILE, crateID, dmbID, lct, mo))
              mo->Fill((int)(clctsDatas[lct].getKeyStrip()), (int)(clctsDatas[lct].getQuality()));
          }
        }
      
        int N_CFEBs = 5;
  
        int NumberOfLayersWithHitsInCLCT = 0;
        int NumberOfHalfStripsWithHitsInCLCT = 0;

        if (clctData && clctData->check()) {
          for(int nCFEB = 0; nCFEB < N_CFEBs; ++nCFEB) {   
            for (int nLayer = 1; nLayer <= 6; nLayer++) {
              int hstrip_previous    = -1;
              int tbin_clct_previous = -1;
              bool CheckLayerCLCT = true;
              std::vector<CSCComparatorDigi> compOutData = clctData->comparatorDigis(nLayer, nCFEB);
              for (std::vector<CSCComparatorDigi>::iterator compOutDataItr = compOutData.begin(); compOutDataItr != compOutData.end(); ++compOutDataItr) {
                // =VB= Fix to get right hafstrip
                int hstrip = 2 * (compOutDataItr->getStrip() - 1) + compOutDataItr->getComparator();
                std::vector<int> tbins_clct = compOutDataItr->getTimeBinsOn();
                int tbin_clct = (int)compOutDataItr->getTimeBin();
                if(CheckLayerCLCT) {
                  NumberOfLayersWithHitsInCLCT = NumberOfLayersWithHitsInCLCT + 1;
                  CheckLayerCLCT = false;
                }
  
                for (uint32_t n=0; n < tbins_clct.size(); n++) {
                  tbin_clct = tbins_clct[n];
                  if(hstrip != hstrip_previous || (tbin_clct != tbin_clct_previous + 1 && tbin_clct != tbin_clct_previous - 1) ) {
                    if (getCSCHisto(h::CSC_CLCTTIME_LYXX, crateID, dmbID, nLayer, mo)) mo->Fill(hstrip, tbin_clct);
                    if (getCSCHisto(h::CSC_CLCTTIME_LYXX_PROFILE, crateID, dmbID, nLayer, mo)) mo->Fill(hstrip, tbin_clct);
                    if (getCSCHisto(h::CSC_CLCT_LYXX_RATE, crateID, dmbID, nLayer, mo)) { 
                      mo->Fill(hstrip);
  
                      double number_hstrip = mo->GetBinContent(hstrip+1);
                      double Number_of_entries_CLCT = mo->GetEntries();
       
                      if (getCSCHisto(h::CSC_CLCT_LYXX_EFFICIENCY, crateID, dmbID, nLayer, mo)) {
                        mo->SetBinContent(hstrip + 1, number_hstrip);
                        if(DMBEvents > 0) {
                          double norm = (100.0 * Number_of_entries_CLCT) / ((double)(DMBEvents));
                          // if (norm < 1.0) norm=1;
                          mo->SetNormFactor(norm);
                        } else {
                          mo->SetNormFactor(100.0);
                        }
                        mo->SetEntries(DMBEvents);
                      }
                    }
                  }
      
                  if(hstrip != hstrip_previous) {
                    NumberOfHalfStripsWithHitsInCLCT = NumberOfHalfStripsWithHitsInCLCT + 1;
                  }
                  hstrip_previous    = hstrip;
                  tbin_clct_previous = tbin_clct;
                }
              }
            }
          }
        } else {
          LOG_ERROR << cscTag << " Can not unpack CLCT Data";
        }

        if (getCSCHisto(h::CSC_CLCT_NUMBER_OF_LAYERS_WITH_HITS, crateID, dmbID, mo)) 
          mo->Fill(NumberOfLayersWithHitsInCLCT);
        if (getCSCHisto(h::CSC_CLCT_NUMBER_OF_HALFSTRIPS_WITH_HITS, crateID, dmbID, mo)) 
          mo->Fill(NumberOfHalfStripsWithHitsInCLCT);
        } else {
          LOG_ERROR << cscTag << " Can not unpack TMB Header or/and Trailer";
        }
      } else {
        LOG_ERROR << cscTag << " Can not unpack TMB Data";
      }
  
    } else {
      //  CLCT not found
  
      if (getCSCHisto(h::CSC_CLCT_NUMBER_RATE, crateID, dmbID, mo)) {
        mo->Fill(0);
        int nCLCT = (int)mo->GetBinContent(1);
        if (getCSCHisto(h::CSC_CLCT_NUMBER, crateID, dmbID, mo)) mo->SetBinContent(1, (float)(nCLCT) / (float)(DMBEvents) * 100.0);
      }    
      if ((tmb_dav  > 0) && (getCSCHisto(h::CSC_DMB_FEB_UNPACKED_VS_DAV, crateID, dmbID, mo))) {
        mo->Fill(1.0, 1.0);
      }
    }

    // CFEB found
    int NumberOfUnpackedCFEBs = 0;
    const int N_CFEBs = 5, N_Samples = 16, N_Layers = 6, N_Strips = 16;
    int ADC = 0, OutOffRange, Threshold = 30;
    // bool DebugCFEB = false;
    CSCCFEBData * cfebData[5];
    CSCCFEBTimeSlice *  timeSlice[5][16];
    CSCCFEBDataWord * timeSample[5][16][6][16];
    int Pedestal[5][6][16];
    std::pair<int,int> CellPeak[5][6][16];
    memset(CellPeak, 0, sizeof(CellPeak));
    float PedestalError[5][6][16];
    CSCCFEBSCAControllerWord scaControllerWord[5][16][6];
    bool CheckCFEB = true;
    //--------------B
    float Clus_Sum_Charge;
    int TrigTime, L1APhase, UnpackedTrigTime, LCTPhase, SCA_BLK, NmbTimeSamples;
    // int NmbCell, SCA_Nmb_FC;
    int  FreeCells, LCT_Pipe_Empty, LCT_Pipe_Full, LCT_Pipe_Count, L1_Pipe_Empty, L1_Pipe_Full, Buffer_Count;
    //--------------E
  
    bool CheckThresholdStripInTheLayer[6][80];
    for(int i=0; i<6; i++) {
      for(int j = 0; j < 80; j++) CheckThresholdStripInTheLayer[i][j] = true;
    }
    
    bool CheckOutOffRangeStripInTheLayer[6][80];  
    for(int i=0; i<6; i++) {
      for(int j=0; j<80; j++) CheckOutOffRangeStripInTheLayer[i][j] = true;
    }
  
    //--------------B
    float cscdata[N_CFEBs * 16][N_Samples][N_Layers];
  //  int TrigTimeData[N_CFEBs*16][N_Samples][N_Layers];
    int SCABlockData[N_CFEBs * 16][N_Samples][N_Layers];
    memset(cscdata, 0, sizeof(cscdata));
  //  memset(TrigTimeData, 0, sizeof(TrigTimeData));
    memset(SCABlockData, 0, sizeof(SCABlockData));
    //--------------E
    
    char hbuf[255];
    memset(hbuf, 0, sizeof(hbuf));
  
    for(int nCFEB = 0; nCFEB < N_CFEBs; ++nCFEB) {
      cfebData[nCFEB] = data.cfebData(nCFEB);
      if (cfebData[nCFEB] !=0) {
        if (!cfebData[nCFEB]->check()) continue;
        //                        CFEB Found
        FEBunpacked = FEBunpacked +1; // Increment number of unpacked FED
        NumberOfUnpackedCFEBs = NumberOfUnpackedCFEBs + 1; // Increment number of unpaked CFEB
        cfeb_unpacked = 1;
        if(CheckCFEB == true){
          if (getCSCHisto(h::CSC_CSC_RATE, crateID, dmbID, mo)) {
            mo->Fill(4);
            uint32_t CFEBEvent = (uint32_t)mo->GetBinContent(5);
            trigCnts["CFEB"] = CFEBEvent;
            if (getCSCHisto(h::CSC_CSC_EFFICIENCY, crateID, dmbID, mo)) {
              if(config->getNEvents() > 0) {
                mo->SetBinContent(3, ((float)CFEBEvent/(float)(DMBEvents)*100.0));
                mo->SetEntries((int)DMBEvents);
              }
            }
          }
  
          if ((cfeb_dav2  > 0) && (getCSCHisto(h::CSC_DMB_FEB_UNPACKED_VS_DAV, crateID, dmbID, mo))) {
            mo->Fill(2.0, 0.0);
          }
          CheckCFEB = false;
        }
        //-------------B
        NmbTimeSamples= (cfebData[nCFEB])->nTimeSamples();
        //-------------E
        LOG_DEBUG <<  "NEvents = " << config->getNEvents();
        LOG_DEBUG <<  "Chamber ID = "<< cscTag << " Crate ID = "<< crateID << " DMB ID = " << dmbID << "nCFEB =" << nCFEB;
  
        // =VB= Optimizations for faster histogram object access 
        MonitorObject* mo_CFEB_SCA_Block_Occupancy = 0;
        getCSCHisto(h::CSC_CFEBXX_SCA_BLOCK_OCCUPANCY, crateID, dmbID, nCFEB, mo_CFEB_SCA_Block_Occupancy);
        MonitorObject*  mo_CFEB_Free_SCA_Cells = 0;      
        getCSCHisto(h::CSC_CFEBXX_FREE_SCA_CELLS, crateID, dmbID, nCFEB, mo_CFEB_Free_SCA_Cells);
        MonitorObject* mo_CFEB_SCA_Blocks_Locked_by_LCTs = 0;
        getCSCHisto(h::CSC_CFEBXX_SCA_BLOCKS_LOCKED_BY_LCTS, crateID, dmbID, nCFEB, mo_CFEB_SCA_Blocks_Locked_by_LCTs);
        MonitorObject* mo_CFEB_SCA_Blocks_Locked_by_LCTxL1 = 0;
        getCSCHisto(h::CSC_CFEBXX_SCA_BLOCKS_LOCKED_BY_LCTXL1, crateID, dmbID, nCFEB, mo_CFEB_SCA_Blocks_Locked_by_LCTxL1);
        MonitorObject* mo_CFEB_DMB_L1A_diff = 0;
        getCSCHisto(h::CSC_CFEBXX_DMB_L1A_DIFF, crateID, dmbID, nCFEB, mo_CFEB_DMB_L1A_diff);
        
        //LOG_DEBUG << " nSample = " << nSample;
  
        for(int nLayer = 1; nLayer <= N_Layers; ++nLayer) {
          //  =VB= Optimizations for faster histogram object access
          MonitorObject * mo_CFEB_Out_Off_Range_Strips = 0;
          getCSCHisto(h::CSC_CFEB_OUT_OFF_RANGE_STRIPS_LYXX, crateID, dmbID, nLayer, mo_CFEB_Out_Off_Range_Strips);
          MonitorObject * mo_CFEB_Active_Samples_vs_Strip = 0;
          getCSCHisto(h::CSC_CFEB_ACTIVE_SAMPLES_VS_STRIP_LYXX, crateID, dmbID, nLayer, mo_CFEB_Active_Samples_vs_Strip);
          MonitorObject * mo_CFEB_Active_Samples_vs_Strip_Profile = 0;
          getCSCHisto(h::CSC_CFEB_ACTIVE_SAMPLES_VS_STRIP_LYXX_PROFILE, crateID, dmbID, nLayer, mo_CFEB_Active_Samples_vs_Strip_Profile);
          MonitorObject * mo_CFEB_ActiveStrips = 0;
          getCSCHisto(h::CSC_CFEB_ACTIVESTRIPS_LYXX, crateID, dmbID, nLayer, mo_CFEB_ActiveStrips);
          MonitorObject * mo_CFEB_SCA_Cell_Peak = 0;
          getCSCHisto(h::CSC_CFEB_SCA_CELL_PEAK_LY_XX, crateID, dmbID, nLayer, mo_CFEB_SCA_Cell_Peak);
  
          MonitorObject * mo_CFEB_Pedestal_withEMV_Sample = 0;
          getCSCHisto(h::CSC_CFEB_PEDESTAL_WITHEMV_SAMPLE_01_LYXX, crateID, dmbID, nLayer, mo_CFEB_Pedestal_withEMV_Sample);
          MonitorObject * mo_CFEB_Pedestal_withRMS_Sample = 0;
          getCSCHisto(h::CSC_CFEB_PEDESTAL_WITHRMS_SAMPLE_01_LYXX, crateID, dmbID, nLayer, mo_CFEB_Pedestal_withRMS_Sample);
          MonitorObject * mo_CFEB_PedestalRMS_Sample = 0;
          getCSCHisto(h::CSC_CFEB_PEDESTALRMS_SAMPLE_01_LYXX, crateID, dmbID, nLayer, mo_CFEB_PedestalRMS_Sample);
  
          for(int nSample = 0; nSample < NmbTimeSamples; ++nSample) {
            timeSlice[nCFEB][nSample] = (CSCCFEBTimeSlice * )((cfebData[nCFEB])->timeSlice(nSample));
            if (timeSlice[nCFEB][nSample] == 0) {
              LOG_WARN <<  "CFEB" << nCFEB << " nSample: " << nSample << " - B-Word";
              continue;
            }
  
            if (mo_CFEB_DMB_L1A_diff && !fCloseL1As ) {
              // if (mo_CFEB_DMB_L1A_diff) 
              int cfeb_dmb_l1a_diff = (int)((timeSlice[nCFEB][nSample]->get_L1A_number())-dmbHeader->l1a()%64);
              if (cfeb_dmb_l1a_diff != 0) { 
    		L1A_out_of_sync = true;
              }
              if(cfeb_dmb_l1a_diff < -32) mo->Fill(cfeb_dmb_l1a_diff + 64);
              else {
                if(cfeb_dmb_l1a_diff > 32) mo->Fill(cfeb_dmb_l1a_diff - 64);
                else mo_CFEB_DMB_L1A_diff->Fill(cfeb_dmb_l1a_diff);
              }
              mo_CFEB_DMB_L1A_diff->SetAxisRange(0.1, 1.1*(1.0+mo_CFEB_DMB_L1A_diff->GetBinContent(mo_CFEB_DMB_L1A_diff->GetMaximumBin())), "Y");
            }
    
            LOG_DEBUG <<  " nSample = " << nSample;
            // for(int nLayer = 1; nLayer <= N_Layers; ++nLayer) 
            scaControllerWord[nCFEB][nSample][nLayer-1] = (timeSlice[nCFEB][nSample])->scaControllerWord(nLayer);
  
            TrigTime = (int)(scaControllerWord[nCFEB][nSample][nLayer-1]).trig_time;
            //--------------B
            FreeCells = (timeSlice[nCFEB][nSample])->get_n_free_sca_blocks();
            LCT_Pipe_Empty = (timeSlice[nCFEB][nSample])->get_lctpipe_empty();
            LCT_Pipe_Full = (timeSlice[nCFEB][nSample])->get_lctpipe_full();
            LCT_Pipe_Count = (timeSlice[nCFEB][nSample])->get_lctpipe_count();
            L1_Pipe_Empty = (timeSlice[nCFEB][nSample])->get_l1pipe_empty();
            L1_Pipe_Full = (timeSlice[nCFEB][nSample])->get_l1pipe_full();
            // L1_Pipe_Count = (timeSlice[nCFEB][nSample])->get_L1A_number();
            Buffer_Count = (timeSlice[nCFEB][nSample])->get_buffer_count();
      
            SCA_BLK  = (int)(scaControllerWord[nCFEB][nSample][nLayer-1]).sca_blk;
            LOG_DEBUG <<  "SCA BLOCK: Chamber=" << chamberID << " CFEB=" << nCFEB + 1
              <<" TRIGTIME="<<TrigTime<<" TimeSlice="<<nSample+1<<" Layer="<<nLayer<<" SCA_BLK="<<SCA_BLK;
      
            for(int nStrip = 0; nStrip < N_Strips; ++nStrip) {
              SCABlockData[nCFEB*16+nStrip][nSample][nLayer-1] = SCA_BLK;
              // if(res<=1) TrigTimeData[nCFEB*16+nStrip][nSample][nLayer-1] = TrigTime;
            }
            //LOG_DEBUG << "*********"<<" TRIGTIME="<<TrigTime<<" BIT COUNT="<<bit_count;
  
            // SCA Block Occupancy Histograms
            // if (getCSCHisto(h::CSC_CFEBXX_SCA_BLOCK_OCCUPANCY, crateID, dmbID, nCFEB, mo)) mo->Fill(SCA_BLK);
            if (mo_CFEB_SCA_Block_Occupancy) mo_CFEB_SCA_Block_Occupancy->Fill(SCA_BLK);
  
            // Free SCA Cells
            // if (getCSCHisto(h::CSC_CFEBXX_FREE_SCA_CELLS, crateID, dmbID, nCFEB, mo)) 
            if (mo_CFEB_Free_SCA_Cells) {
              if (scaControllerWord[nCFEB][nSample][nLayer-1].sca_full == 1) mo_CFEB_Free_SCA_Cells->Fill(-1);
              mo_CFEB_Free_SCA_Cells->Fill(FreeCells);
            }
  
            // Number of SCA Blocks Locked by LCTs
            // if (getCSCHisto(h::CSC_CFEBXX_SCA_BLOCKS_LOCKED_BY_LCTS, crateID, dmbID, nCFEB, mo)) 
            if (mo_CFEB_SCA_Blocks_Locked_by_LCTs) {
              if (LCT_Pipe_Empty == 1) mo_CFEB_SCA_Blocks_Locked_by_LCTs->Fill(-0.5);
              if (LCT_Pipe_Full == 1) mo_CFEB_SCA_Blocks_Locked_by_LCTs->Fill(16.5);
              mo_CFEB_SCA_Blocks_Locked_by_LCTs->Fill(LCT_Pipe_Count);
            }
  
            // Number of SCA Blocks Locked by LCTxL1
            // if (getCSCHisto(h::CSC_CFEBXX_SCA_BLOCKS_LOCKED_BY_LCTXL1, crateID, dmbID, nCFEB, mo)) 
            if (mo_CFEB_SCA_Blocks_Locked_by_LCTxL1) {
              if (L1_Pipe_Empty == 1) mo_CFEB_SCA_Blocks_Locked_by_LCTxL1->Fill(-0.5);
              if (L1_Pipe_Full == 1) mo_CFEB_SCA_Blocks_Locked_by_LCTxL1->Fill(31.5);
              mo_CFEB_SCA_Blocks_Locked_by_LCTxL1->Fill(Buffer_Count);
            }
  
            //--------------E
            LOG_DEBUG <<  "nCFEB " << nCFEB << " nSample " << nSample << " nLayer " << nLayer << " TrigTime " << TrigTime;
            if(nSample == 0 && nLayer == 1) {
              TrigTime = (int)(scaControllerWord[nCFEB][nSample][nLayer - 1]).trig_time;
              int k = 1;
              while (((TrigTime >> (k-1)) & 0x1) != 1 && k <= 8) {
                k = k + 1;
              }
              L1APhase = (int)(((scaControllerWord[nCFEB][nSample][nLayer - 1]).l1a_phase) & 0x1);
              UnpackedTrigTime = ((k << 1) & 0xE) + L1APhase;
  
              if (getCSCHisto(h::CSC_CFEBXX_L1A_SYNC_TIME, crateID, dmbID, nCFEB, mo)) 
                mo->Fill((int)UnpackedTrigTime);
              LCTPhase = (int)(((scaControllerWord[nCFEB][nSample][nLayer-1]).lct_phase)&0x1);
  
              if (getCSCHisto(h::CSC_CFEBXX_LCT_PHASE_VS_L1A_PHASE, crateID, dmbID, nCFEB, mo)) 
                mo->Fill(LCTPhase, L1APhase);
  
              LOG_DEBUG <<  "L1APhase " << L1APhase << " UnpackedTrigTime " << UnpackedTrigTime;
  
              if (getCSCHisto(h::CSC_CFEBXX_L1A_SYNC_TIME_VS_DMB, crateID, dmbID, nCFEB, mo))  
                mo->Fill((int)(dmbHeader->dmbCfebSync()), (int)UnpackedTrigTime);
  
              if (getCSCHisto(h::CSC_CFEBXX_L1A_SYNC_TIME_DMB_DIFF, crateID, dmbID, nCFEB, mo)) {
                int cfeb_dmb_L1A_sync_time = (int)(dmbHeader->dmbCfebSync()) - (int)UnpackedTrigTime;
                if(cfeb_dmb_L1A_sync_time < -8) mo->Fill(cfeb_dmb_L1A_sync_time+16);
                else {
                  if(cfeb_dmb_L1A_sync_time > 8) mo->Fill(cfeb_dmb_L1A_sync_time-16);
                  else mo->Fill(cfeb_dmb_L1A_sync_time);
                }
                mo->SetAxisRange(0.1, 1.1*(1.0+mo->GetBinContent(mo->GetMaximumBin())), "Y");
              }
  
            }
  
  
            for(int nStrip = 1; nStrip <= N_Strips; ++nStrip) {
              timeSample[nCFEB][nSample][nLayer - 1][nStrip - 1]=(data.cfebData(nCFEB)->timeSlice(nSample))->timeSample(nLayer,nStrip);
              ADC = (int) ((timeSample[nCFEB][nSample][nLayer - 1][nStrip - 1]->adcCounts) & 0xFFF);
              LOG_DEBUG <<  " nStrip="<< dec << nStrip << " ADC=" << hex << ADC;
              OutOffRange = (int) ((timeSample[nCFEB][nSample][nLayer - 1][nStrip - 1]->adcOverflow) & 0x1);
  
              if(nSample == 0) { // nSample == 0
                CellPeak[nCFEB][nLayer-1][nStrip-1] = std::make_pair(nSample,ADC);
                Pedestal[nCFEB][nLayer-1][nStrip-1] = ADC;
                LOG_DEBUG <<  " nStrip="<< dec << nStrip << " Pedestal=" << hex << Pedestal[nCFEB][nLayer-1][nStrip-1];
              }
    
              if(OutOffRange == 1 && CheckOutOffRangeStripInTheLayer[nLayer - 1][nCFEB * 16 + nStrip - 1] == true) {
                // if (getCSCHisto(h::CSC_CFEB_OUT_OFF_RANGE_STRIPS_LYXX, crateID, dmbID, nLayer, mo)) 
                if ( mo_CFEB_Out_Off_Range_Strips)
                  mo_CFEB_Out_Off_Range_Strips->Fill((int)(nCFEB * 16 + nStrip));
                CheckOutOffRangeStripInTheLayer[nLayer - 1][nCFEB * 16 + nStrip - 1] = false;
              }
              if(ADC - Pedestal[nCFEB][nLayer - 1][nStrip - 1] > Threshold && OutOffRange != 1) {	      
                // if (getCSCHisto(h::CSC_CFEB_ACTIVE_SAMPLES_VS_STRIP_LYXX, crateID, dmbID, nLayer, mo))
                if (mo_CFEB_Active_Samples_vs_Strip)
                  mo_CFEB_Active_Samples_vs_Strip->Fill((int)(nCFEB * 16 + nStrip), nSample);
  
                // if (getCSCHisto(h::CSC_CFEB_ACTIVE_SAMPLES_VS_STRIP_LYXX_PROFILE, crateID, dmbID, nLayer, mo))
                if (mo_CFEB_Active_Samples_vs_Strip_Profile)
                  mo_CFEB_Active_Samples_vs_Strip_Profile->Fill((int)(nCFEB * 16 + nStrip), nSample);
  
                if(CheckThresholdStripInTheLayer[nLayer - 1][nCFEB * 16 + nStrip - 1] == true) {
                // if (getCSCHisto(h::CSC_CFEB_ACTIVESTRIPS_LYXX, crateID, dmbID, nLayer, mo))
                  if (mo_CFEB_ActiveStrips)
                    mo_CFEB_ActiveStrips->Fill((int)(nCFEB * 16 + nStrip));
                  CheckThresholdStripInTheLayer[nLayer - 1][nCFEB * 16 + nStrip - 1] = false;
                }
                //--------------B
                if(ADC - Pedestal[nCFEB][nLayer - 1][nStrip - 1] > Threshold) {
                  LOG_DEBUG <<  "Layer="<<nLayer<<" Strip="<<nCFEB*16+nStrip<<" Time="<<nSample << " ADC-PEDEST = "<<ADC - Pedestal[nCFEB][nLayer-1][nStrip-1];
                  cscdata[nCFEB * 16 + nStrip - 1][nSample][nLayer - 1] = ADC - Pedestal[nCFEB][nLayer - 1][nStrip - 1];
                }	
                //--------------E
                if(ADC > CellPeak[nCFEB][nLayer - 1][nStrip - 1].second) { 
                  CellPeak[nCFEB][nLayer - 1][nStrip - 1].first = nSample;
                  CellPeak[nCFEB][nLayer - 1][nStrip - 1].second = ADC;
                }
              }
              // continue;
              //--------------B
              if(nSample == 1) {
                int channel_threshold = 40;
                if (abs(ADC - Pedestal[nCFEB][nLayer - 1][nStrip - 1]) < channel_threshold) {
                  // if (getCSCHisto(h::CSC_CFEB_PEDESTAL__WITHEMV__SAMPLE_01_LYXX, crateID, dmbID, nLayer, mo))
                  if (mo_CFEB_Pedestal_withEMV_Sample)
                    mo_CFEB_Pedestal_withEMV_Sample->Fill((int)(nCFEB * 16 + nStrip), Pedestal[nCFEB][nLayer - 1][nStrip - 1]);
  
                  //if (getCSCHisto(h::CSC_CFEB_PEDESTAL__WITHRMS__SAMPLE_01_LYXX, crateID, dmbID, nLayer, mo)) 
                  if (mo_CFEB_Pedestal_withRMS_Sample) {  
                    mo_CFEB_Pedestal_withRMS_Sample->Fill((int)(nCFEB * 16 + nStrip), Pedestal[nCFEB][nLayer - 1][nStrip - 1]);
                    PedestalError[nCFEB][nLayer - 1][nStrip - 1] = mo_CFEB_Pedestal_withRMS_Sample->GetBinError(nCFEB * 16 + nStrip);
  
                    // if (getCSCHisto(h::CSC_CFEB_PEDESTALRMS_SAMPLE_01_LYXX, crateID, dmbID, nLayer, mo)) 
                    if (mo_CFEB_PedestalRMS_Sample) {
                      mo_CFEB_PedestalRMS_Sample->SetBinContent(nCFEB * 16 + nStrip,PedestalError[nCFEB][nLayer - 1][nStrip - 1]);
                      mo_CFEB_PedestalRMS_Sample->SetBinError(nCFEB * 16 + nStrip, 0.00000000001);
                    }
                  }
                }
              }
              //--------------E
            }
          }

          for(int nStrip = 1; nStrip <= N_Strips; ++nStrip) {
            if (mo_CFEB_SCA_Cell_Peak && CellPeak[nCFEB][nLayer - 1][nStrip - 1].first) 
              mo_CFEB_SCA_Cell_Peak->Fill((int)(nCFEB * 16 + nStrip), CellPeak[nCFEB][nLayer - 1][nStrip - 1].first);
          }

        }
      }

      //--------------B
      float Cathodes[N_CFEBs * N_Strips * N_Samples * N_Layers];
      for(int i = 0; i < N_Layers; ++i) {
        for(int j = 0; j < N_CFEBs * N_Strips; ++j) {
          for(int k = 0; k < N_Samples; ++k) {
            Cathodes[i * N_CFEBs * N_Strips * N_Samples + N_CFEBs * N_Strips * k + j] = cscdata[j][k][i];
          }
        }
      }
      std::vector<StripCluster> Clus;
      Clus.clear();
      StripClusterFinder ClusterFinder(N_Layers, N_Samples, N_CFEBs, N_Strips);
  
      for(int nLayer = 1; nLayer <= N_Layers; ++nLayer) {
      
        // StripClusterFinder *ClusterFinder = new StripClusterFinder(N_Layers, N_Samples, N_CFEBs, N_Strips);
      
        ClusterFinder.DoAction(nLayer - 1, Cathodes);
        Clus = ClusterFinder.getClusters();

        LOG_DEBUG <<  "***  CATHODE PART  DEBUG: Layer=" << nLayer <<"  Number of Clusters=" << Clus.size() << "      ***";
        // Number of Clusters Histograms
        if (getCSCHisto(h::CSC_CFEB_NUMBER_OF_CLUSTERS_LY_XX, crateID, dmbID, nLayer, mo)) {
          if(Clus.size() >= 0)  mo->Fill(Clus.size());
        }
  
        for(uint32_t u = 0; u < Clus.size(); u++){
          Clus_Sum_Charge = 0.0;
          for(uint32_t k = 0;k < Clus[u].ClusterPulseMapHeight.size(); k++) {
            LOG_DEBUG <<  "Strip: " << Clus[u].ClusterPulseMapHeight[k].channel_+1;
            for(int n=Clus[u].LFTBNDTime; n < Clus[u].IRTBNDTime; n++) {
              Clus_Sum_Charge = Clus_Sum_Charge + Clus[u].ClusterPulseMapHeight[k].height_[n];
            }
          }
  
          // Clusters Charge Histograms
          if (getCSCHisto(h::CSC_CFEB_CLUSTERS_CHARGE_LY_XX, crateID, dmbID, nLayer, mo)) 
            mo->Fill(Clus_Sum_Charge);
  
          // Width of Clusters Histograms
          if (getCSCHisto(h::CSC_CFEB_WIDTH_OF_CLUSTERS_LY_XX, crateID, dmbID, nLayer, mo)) 
            mo->Fill(Clus[u].IRTBNDStrip - Clus[u].LFTBNDStrip + 1);
  
          // Cluster Duration Histograms
          if (getCSCHisto(h::CSC_CFEB_CLUSTER_DURATION_LY_XX, crateID, dmbID, nLayer, mo)) 
            mo->Fill(Clus[u].IRTBNDTime - Clus[u].LFTBNDTime + 1);
        }
  
        Clus.clear();
    
        // delete ClusterFinder;
      }
    
      //--------------E
  
      // Fill Hisogram for Different Combinations of FEBs Unpacked vs DAV
      if (getCSCHisto(h::CSC_DMB_FEB_COMBINATIONS_UNPACKED_VS_DAV, crateID, dmbID, mo)) {
        float feb_combination_unpacked = -1.0;
        if(alct_unpacked == 0 && tmb_unpacked == 0 && cfeb_unpacked == 0) feb_combination_unpacked = 0.0;
        if(alct_unpacked >  0 && tmb_unpacked == 0 && cfeb_unpacked == 0) feb_combination_unpacked = 1.0;
        if(alct_unpacked == 0 && tmb_unpacked >  0 && cfeb_unpacked == 0) feb_combination_unpacked = 2.0;
        if(alct_unpacked == 0 && tmb_unpacked == 0 && cfeb_unpacked >  0) feb_combination_unpacked = 3.0;
        if(alct_unpacked >  0 && tmb_unpacked >  0 && cfeb_unpacked == 0) feb_combination_unpacked = 4.0;
        if(alct_unpacked >  0 && tmb_unpacked == 0 && cfeb_unpacked >  0) feb_combination_unpacked = 5.0;
        if(alct_unpacked == 0 && tmb_unpacked >  0 && cfeb_unpacked >  0) feb_combination_unpacked = 6.0;
        if(alct_unpacked >  0 && tmb_unpacked >  0 && cfeb_unpacked >  0) feb_combination_unpacked = 7.0;
        mo->Fill(feb_combination_dav, feb_combination_unpacked);
      }
  
      if((clct_kewdistrip > -1 && alct_keywg > -1) && (getCSCHisto(h::CSC_CLCT0_KEYDISTRIP_VS_ALCT0_KEYWIREGROUP, crateID, dmbID, mo))) {
        mo->Fill(alct_keywg, clct_kewdistrip);
      }
  
      if (L1A_out_of_sync && cscType && cscPosition && getEMUHisto(h::EMU_CSC_L1A_OUT_OF_SYNC, mo)){
        mo->Fill(cscPosition, cscType);
      }
  
      if (L1A_out_of_sync && getEMUHisto(h::EMU_DMB_L1A_OUT_OF_SYNC, mo)){
        mo->Fill(crateID, dmbID);
      }

    }

  }
  
}
