/*
 * =====================================================================================
 *
 *       Filename:  CSCMonitorModule_updateFracHistos.cc
 *
 *    Description:  Method updateFracHistos of CSCMonitorModule implementation.
 *    This method should be called after run or on demand.  
 *
 *        Version:  1.0
 *        Created:  04/23/2008 01:46:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Valdas Rapsevicius (VR), Valdas.Rapsevicius@cern.ch
 *        Company:  CERN, CH
 *
 * =====================================================================================
 */

#include "DQM/CSCMonitorModule/interface/CSCMonitorModule.h"
#include "csc_utilities.cc"

void CSCMonitorModule::updateFracHistos() {

  MonitorElement *me1 = NULL, *me2 = NULL, *me3 = NULL;

  //
  // Calculate Aggregate Histograms
  //

  if (MEEMU("DMB_Reporting", me1) && MEEMU("DMB_Format_Errors", me2) && MEEMU("DMB_Unpacked", me3)) {
    me1->getTH1()->Add(me2->getTH1(), me3->getTH1());
    if (MEEMU("DMB_Unpacked_with_errors", me2)) {
      me1->getTH1()->Add(me2->getTH1(), -1);
    }
  }

  if (MEEMU("CSC_Reporting", me1) && MEEMU("CSC_Format_Errors", me2) && MEEMU("CSC_Unpacked", me3)) {
    me1->getTH1()->Add(me2->getTH1(), me3->getTH1());
    if (MEEMU("CSC_Unpacked_with_errors", me2)) {
      me1->getTH1()->Add(me2->getTH1(), -1);
    }
  }

  //
  // Calculate Fractional Histograms
  //

  if (MEEMU("DMB_Format_Errors_Fract", me1) && MEEMU("DMB_Reporting", me2) && MEEMU("DMB_Format_Errors", me3)) 
    me1->getTH1()->Divide(me3->getTH1(), me2->getTH1());

  if (MEEMU("CSC_Format_Errors_Fract", me1) && MEEMU("CSC_Reporting", me2) && MEEMU("CSC_Format_Errors", me3)) 
    me1->getTH1()->Divide(me3->getTH1(), me2->getTH1());

  if (MEEMU("DMB_Unpacked_Fract", me1) && MEEMU("DMB_Reporting", me2) && MEEMU("DMB_Unpacked", me3)) 
    me1->getTH1()->Divide(me3->getTH1(), me2->getTH1());

  if (MEEMU("CSC_Unpacked_Fract", me1) && MEEMU("CSC_Reporting", me2) && MEEMU("CSC_Unpacked", me3)) 
    me1->getTH1()->Divide(me3->getTH1(), me2->getTH1());

  if (MEEMU("DMB_wo_ALCT_Fract", me1) && MEEMU("DMB_Reporting", me2) && MEEMU("DMB_wo_ALCT", me3)) 
    me1->getTH1()->Divide(me3->getTH1(), me2->getTH1());

  if (MEEMU("CSC_wo_ALCT_Fract", me1) && MEEMU("CSC_Reporting", me2) && MEEMU("CSC_wo_ALCT", me3)) 
    me1->getTH1()->Divide(me3->getTH1(), me2->getTH1());

  if (MEEMU("DMB_wo_CLCT_Fract", me1) && MEEMU("DMB_Reporting", me2) && MEEMU("DMB_wo_CLCT", me3)) 
    me1->getTH1()->Divide(me3->getTH1(), me2->getTH1());

  if (MEEMU("CSC_wo_CLCT_Fract", me1) && MEEMU("CSC_Reporting", me2) && MEEMU("CSC_wo_CLCT", me3)) 
    me1->getTH1()->Divide(me3->getTH1(), me2->getTH1());

  if (MEEMU("DMB_wo_CFEB_Fract", me1) && MEEMU("DMB_Reporting", me2) && MEEMU("DMB_wo_CFEB", me3)) 
    me1->getTH1()->Divide(me3->getTH1(), me2->getTH1());

  if (MEEMU("CSC_wo_CFEB_Fract", me1) && MEEMU("CSC_Reporting", me2) && MEEMU("CSC_wo_CFEB", me3)) 
    me1->getTH1()->Divide(me3->getTH1(), me2->getTH1());

  if (MEEMU("CSC_DMB_input_fifo_full_Fract", me1) && MEEMU("CSC_Reporting", me2) && MEEMU("CSC_DMB_input_fifo_full", me3)) 
    me1->getTH1()->Divide(me3->getTH1(), me2->getTH1());

  if (MEEMU("DMB_input_fifo_full_Fract", me1) && MEEMU("DMB_Reporting", me2) && MEEMU("DMB_input_fifo_full", me3)) 
    me1->getTH1()->Divide(me3->getTH1(), me2->getTH1());

  if (MEEMU("CSC_DMB_input_timeout_Fract", me1) && MEEMU("CSC_Reporting", me2) && MEEMU("CSC_DMB_input_timeout", me3)) 
    me1->getTH1()->Divide(me3->getTH1(), me2->getTH1());

  if (MEEMU("DMB_input_timeout_Fract", me1) && MEEMU("DMB_Reporting", me2) && MEEMU("DMB_input_timeout", me3)) 
    me1->getTH1()->Divide(me3->getTH1(), me2->getTH1());

  if (MEEMU("DMB_Format_Warnings_Fract", me1) && MEEMU("DMB_Format_Warnings", me2) && MEEMU("DMB_Unpacked", me3)) {
    TH1* tmp=dynamic_cast<TH1*>(me3->getTH1()->Clone());
    tmp->Add(me2->getTH1());
    if (MEEMU("DMB_Unpacked_with_warnings", me3)) tmp->Add(me3->getTH1(), -1);
    me1->getTH1()->Divide(me2->getTH1(), tmp);
    delete tmp;
  }

  if (MEEMU("CSC_Format_Warnings_Fract", me1) && MEEMU("CSC_Format_Warnings", me2) && MEEMU("CSC_Unpacked", me3)) {
    TH1* tmp=dynamic_cast<TH1*>(me3->getTH1()->Clone());
    tmp->Add(me2->getTH1());
    if (MEEMU("CSC_Unpacked_with_warnings", me3)) tmp->Add(me3->getTH1(), -1);
    me1->getTH1()->Divide(me2->getTH1(), tmp);
    delete tmp;
  }

  //
  // Set detector information
  //

  if (MEEMU("CSC_Unpacked_Fract", me1)){
    TH2* tmp=dynamic_cast<TH2*>(me1->getTH1());
    summary.ReadChambers(tmp, 0.75);
  }

  //
  // Write summary information
  //

  if (MEEMU("Summary_ME1", me1)){
    TH1* tmp = me1->getTH1();
    summary.Write(tmp, 1);
  }

  if (MEEMU("Summary_ME2", me1)){
    TH1* tmp = me1->getTH1();
    summary.Write(tmp, 2);
  }

  if (MEEMU("Summary_ME3", me1)){
    TH1* tmp = me1->getTH1();
    summary.Write(tmp, 3);
  }

  if (MEEMU("Summary_ME4", me1)){
    TH1* tmp = me1->getTH1();
    summary.Write(tmp, 4);
  }

  if (MEEventInfo("reportSummaryMap", me1)){

    TH2* tmp=dynamic_cast<TH2*>(me1->getTH1());
    float rs = summary.WriteMap(tmp);
    float he = summary.GetEfficiencyHW();
    TString title = Form("EMU Status: Physics Efficiency %.2f (HW Efficiency %.2f)", rs, he);
    tmp->SetTitle(title);

    if(MEEventInfo("reportSummary", me1))  me1->Fill(rs);
    if(MEReportSummaryContents("EMUPhysicsEfficiency", me1)) me1->Fill(rs);
    if(MEReportSummaryContents("EMUHWEfficiency", me1))      me1->Fill(he);
    if(MEReportSummaryContents("ME1PhysicsEfficiency", me1)) me1->Fill(summary.GetEfficiencyArea(1));
    if(MEReportSummaryContents("ME2PhysicsEfficiency", me1)) me1->Fill(summary.GetEfficiencyArea(2));
    if(MEReportSummaryContents("ME3PhysicsEfficiency", me1)) me1->Fill(summary.GetEfficiencyArea(3));
    if(MEReportSummaryContents("ME4PhysicsEfficiency", me1)) me1->Fill(summary.GetEfficiencyArea(4)); 
    if(MEReportSummaryContents("ME1HWEfficiency", me1))      me1->Fill(summary.GetEfficiencyHW(1));
    if(MEReportSummaryContents("ME2HWEfficiency", me1))      me1->Fill(summary.GetEfficiencyHW(2));
    if(MEReportSummaryContents("ME3HWEfficiency", me1))      me1->Fill(summary.GetEfficiencyHW(3));
    if(MEReportSummaryContents("ME4HWEfficiency", me1))      me1->Fill(summary.GetEfficiencyHW(4));

  }

}

