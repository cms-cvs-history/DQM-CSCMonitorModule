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

}

