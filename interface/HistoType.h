/*
 * =====================================================================================
 *
 *       Filename:  HistoType.h
 *
 *    Description:  Histo Type Constants
 *
 *        Version:  1.0
 *        Created:  10/03/2008 11:54:31 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Valdas Rapsevicius (VR), Valdas.Rapsevicius@cern.ch
 *        Company:  CERN, CH
 *
 * =====================================================================================
 */

#ifndef HistoType_H
#define HistoType_H 

namespace cscdqm {

  typedef enum HistoType {

    EMU_ALL_DDU_AVERAGE_EVENT_SIZE,
    EMU_ALL_DDUS_AVERAGE_INPUTS_WITH_DATA,
    EMU_ALL_DDUS_AVERAGE_LIVE_INPUTS,
    EMU_ALL_DDUS_EVENT_SIZE,
    EMU_ALL_DDUS_INPUTS_ERRORS,
    EMU_ALL_DDUS_INPUTS_WARNINGS,
    EMU_ALL_DDUS_INPUTS_WITH_DATA,
    EMU_ALL_DDUS_IN_READOUT,
    EMU_ALL_DDUS_L1A_INCREMENT,
    EMU_ALL_DDUS_LIVE_INPUTS,
    EMU_ALL_DDUS_TRAILER_ERRORS,
    EMU_CSC_DMB_INPUT_FIFO_FULL,
    EMU_CSC_DMB_INPUT_TIMEOUT,
    EMU_CSC_FORMAT_ERRORS,
    EMU_CSC_FORMAT_WARNINGS,
    EMU_CSC_L1A_OUT_OF_SYNC,
    EMU_CSC_REPORTING,
    EMU_CSC_UNPACKED,
    EMU_CSC_UNPACKED_WITH_ERRORS,
    EMU_CSC_WO_ALCT,
    EMU_CSC_WO_CFEB,
    EMU_CSC_WO_CLCT,
    EMU_DDU_FORMAT_ERROR,
    EMU_DMB_FORMAT_ERRORS,
    EMU_DMB_FORMAT_WARNINGS,
    EMU_DMB_INPUT_FIFO_FULL,
    EMU_DMB_INPUT_TIMEOUT,
    EMU_DMB_L1A_OUT_OF_SYNC,
    EMU_DMB_REPORTING,
    EMU_DMB_UNPACKED,
    EMU_DMB_UNPACKED_WITH_ERRORS,
    EMU_DMB_WO_ALCT,
    EMU_DMB_WO_CFEB,
    EMU_DMB_WO_CLCT,
    EMU_READOUT_ERROR,
    EMU_CSC_DMB_INPUT_FIFO_FULL_FRACT,
    EMU_CSC_DMB_INPUT_TIMEOUT_FRACT,
    EMU_CSC_FORMAT_ERRORS_FRACT,
    EMU_CSC_FORMAT_WARNINGS_FRACT,
    EMU_CSC_L1A_OUT_OF_SYNC_FRACT,
    EMU_CSC_UNPACKED_FRACT,
    EMU_CSC_WO_ALCT_FRACT,
    EMU_CSC_WO_CFEB_FRACT,
    EMU_CSC_WO_CLCT_FRACT,
    EMU_DMB_FORMAT_ERRORS_FRACT,
    EMU_DMB_FORMAT_WARNINGS_FRACT,
    EMU_DMB_INPUT_FIFO_FULL_FRACT,
    EMU_DMB_INPUT_TIMEOUT_FRACT,
    EMU_DMB_L1A_OUT_OF_SYNC_FRACT,
    EMU_DMB_UNPACKED_FRACT,
    EMU_DMB_WO_ALCT_FRACT,
    EMU_DMB_WO_CFEB_FRACT,
    EMU_DMB_WO_CLCT_FRACT,
    EMU_CSC_STATS_CFEB_BWORDS,
    EMU_CSC_STATS_FIFOFULL_ERR,
    EMU_CSC_STATS_FORMAT_ERR,
    EMU_CSC_STATS_INPUTTO_ERR,
    EMU_CSC_STATS_L1SYNC_ERR,
    EMU_CSC_STATS_OCCUPANCY,
    EMU_CSC_STATS_SUMMARY,
    EMU_CSC_STATS_WO_ALCT,
    EMU_CSC_STATS_WO_CFEB,
    EMU_CSC_STATS_WO_CLCT,
    EMU_PHYSICS_EMU,
    EMU_PHYSICS_ME,
    EMU_PHYSICS_ME1,
    EMU_PHYSICS_ME3,
    EMU_PHYSICS_ME4,

    DDU_BUFFER_SIZE,
    DDU_BXN,
    DDU_CSC_ERRORS,
    DDU_CSC_ERRORS_RATE,
    DDU_CSC_WARNINGS,
    DDU_CSC_WARNINGS_RATE,
    DDU_DMB_ACTIVE_HEADER_COUNT,
    DDU_DMB_CONNECTED_INPUTS,
    DDU_DMB_CONNECTED_INPUTS_RATE,
    DDU_DMB_DAV_HEADER_COUNT_VS_DMB_ACTIVE_HEADER_COUNT,
    DDU_DMB_DAV_HEADER_OCCUPANCY,
    DDU_DMB_DAV_HEADER_OCCUPANCY_RATE,
    DDU_DMB_UNPACKED_VS_DAV,
    DDU_L1A_INCREMENT,
    DDU_TRAILER_ERRORSTAT_FREQUENCY,
    DDU_TRAILER_ERRORSTAT_RATE,
    DDU_TRAILER_ERRORSTAT_TABLE,
    DDU_WORD_COUNT,

    CSC_ACTUAL_DMB_CFEB_DAV_FREQUENCY,
    CSC_ACTUAL_DMB_CFEB_DAV_MULTIPLICITY_FREQUENCY,
    CSC_ACTUAL_DMB_CFEB_DAV_MULTIPLICITY_RATE,
    CSC_ACTUAL_DMB_CFEB_DAV_RATE,
    CSC_ACTUAL_DMB_FEB_COMBINATIONS_DAV_FREQUENCY,
    CSC_ACTUAL_DMB_FEB_COMBINATIONS_DAV_RATE,
    CSC_ACTUAL_DMB_FEB_DAV_FREQUENCY,
    CSC_ACTUAL_DMB_FEB_DAV_RATE,
    CSC_ALCT1_VS_ALCT0_KEYWG,
    CSC_ALCT_BXN,
    CSC_ALCT_BXN_VS_DMB_BXN,
    CSC_ALCT_DMB_BXN_DIFF,
    CSC_ALCT_DMB_L1A_DIFF,
    CSC_ALCT_L1A,
    CSC_ALCT_LYXX_EFFICIENCY,
    CSC_ALCT_LYXX_RATE,
    CSC_ALCT_MATCH_TIME,
    CSC_ALCT_NUMBER_EFFICIENCY,
    CSC_ALCT_NUMBER_OF_LAYERS_WITH_HITS,
    CSC_ALCT_NUMBER_OF_WIREGROUPS_WITH_HITS,
    CSC_ALCT_NUMBER_RATE,
    CSC_ALCTTIME_LYXX,
    CSC_ALCTTIME_LYXX_PROFILE,
    CSC_ALCT_WORD_COUNT,
    CSC_ALCTXX_BXN,
    CSC_ALCTXX_DTIME,
    CSC_ALCTXX_DTIME_PROFILE,
    CSC_ALCTXX_DTIME_VS_KEYWG,
    CSC_ALCTXX_KEYWG,
    CSC_ALCTXX_PATTERN,
    CSC_ALCTXX_QUALITY,
    CSC_ALCTXX_QUALITY_PROFILE,
    CSC_BINCHECK_DATAFLOW_PROBLEMS_FREQUENCY,
    CSC_BINCHECK_DATAFLOW_PROBLEMS_TABLE,
    CSC_BINCHECK_ERRORS_FREQUENCY,
    CSC_BINCHECK_ERRORSTAT_TABLE,
    CSC_CFEB_ACTIVE_SAMPLES_VS_STRIP_LYXX,
    CSC_CFEB_ACTIVE_SAMPLES_VS_STRIP_LYXX_PROFILE,
    CSC_CFEB_ACTIVESTRIPS_LYXX,
    CSC_CFEB_CLUSTER_DURATION_LY_XX,
    CSC_CFEB_CLUSTERS_CHARGE_LY_XX,
    CSC_CFEB_NUMBER_OF_CLUSTERS_LY_XX,
    CSC_CFEB_OUT_OFF_RANGE_STRIPS_LYXX,
    CSC_CFEB_PEDESTALRMS_SAMPLE_01_LYXX,
    CSC_CFEB_PEDESTAL__WITHEMV__SAMPLE_01_LYXX,
    CSC_CFEB_PEDESTAL_WITHEMV_SAMPLE_01_LYXX,
    CSC_CFEB_PEDESTAL__WITHRMS__SAMPLE_01_LYXX,
    CSC_CFEB_PEDESTAL_WITHRMS_SAMPLE_01_LYXX,
    CSC_CFEB_SCA_CELL_PEAK_LY_XX,
    CSC_CFEB_WIDTH_OF_CLUSTERS_LY_XX,
    CSC_CFEBXX_DMB_L1A_DIFF,
    CSC_CFEBXX_FREE_SCA_CELLS,
    CSC_CFEBXX_L1A_SYNC_TIME,
    CSC_CFEBXX_L1A_SYNC_TIME_DMB_DIFF,
    CSC_CFEBXX_L1A_SYNC_TIME_VS_DMB,
    CSC_CFEBXX_LCT_PHASE_VS_L1A_PHASE,
    CSC_CFEBXX_SCA_BLOCK_OCCUPANCY,
    CSC_CFEBXX_SCA_BLOCKS_LOCKED_BY_LCTS,
    CSC_CFEBXX_SCA_BLOCKS_LOCKED_BY_LCTXL1,
    CSC_CLCT0_CLCT1_CLSSIFICATION,
    CSC_CLCT0_CLSSIFICATION,
    CSC_CLCT0_KEYDISTRIP_VS_ALCT0_KEYWIREGROUP,
    CSC_CLCT1_VS_CLCT0_KEY_STRIP,
    CSC_CLCT_BXN,
    CSC_CLCT_BXN_VS_DMB_BXN,
    CSC_CLCT_DMB_BXN_DIFF,
    CSC_CLCT_DMB_L1A_DIFF,
    CSC_CLCT_L1A,
    CSC_CLCT_LYXX_EFFICIENCY,
    CSC_CLCT_LYXX_RATE,
    CSC_CLCT_NUMBER,
    CSC_CLCT_NUMBER_OF_HALFSTRIPS_WITH_HITS,
    CSC_CLCT_NUMBER_OF_LAYERS_WITH_HITS,
    CSC_CLCT_NUMBER_RATE,
    CSC_CLCTTIME_LYXX,
    CSC_CLCTTIME_LYXX_PROFILE,
    CSC_CLCTXX_BXN,
    CSC_CLCTXX_DISTRIP_PATTERN,
    CSC_CLCTXX_DISTRIP_QUALITY,
    CSC_CLCTXX_DISTRIP_QUALITY_PROFILE,
    CSC_CLCTXX_DTIME,
    CSC_CLCTXX_DTIME_VS_DISTRIP,
    CSC_CLCTXX_DTIME_VS_HALF_STRIP,
    CSC_CLCTXX_HALF_STRIP_PATTERN,
    CSC_CLCTXX_HALF_STRIP_QUALITY,
    CSC_CLCTXX_HALF_STRIP_QUALITY_PROFILE,
    CSC_CLCTXX_KEYDISTRIP,
    CSC_CLCTXX_KEYHALFSTRIP,
    CSC_CSC_EFFICIENCY,
    CSC_CSC_RATE,
    CSC_DMB_BXN_DISTRIB,
    CSC_DMB_BXN_VS_DDU_BXN,
    CSC_DMB_CFEB_ACTIVE,
    CSC_DMB_CFEB_ACTIVE_VS_DAV,
    CSC_DMB_CFEB_DAV,
    CSC_DMB_CFEB_DAV_MULTIPLICITY,
    CSC_DMB_CFEB_DAV_MULTIPLICITY_UNPACKING_INEFFICIENCY,
    CSC_DMB_CFEB_DAV_UNPACKING_INEFFICIENCY,
    CSC_DMB_CFEB_MOVLP,
    CSC_DMB_CFEB_SYNC,
    CSC_DMB_DDU_BXN_DIFF,
    CSC_DMB_DDU_L1A_DIFF,
    CSC_DMB_FEB_COMBINATIONS_DAV_EFFICIENCY,
    CSC_DMB_FEB_COMBINATIONS_DAV_RATE,
    CSC_DMB_FEB_COMBINATIONS_DAV_UNPACKING_INEFFICIENCY,
    CSC_DMB_FEB_COMBINATIONS_UNPACKED_VS_DAV,
    CSC_DMB_FEB_DAV_EFFICIENCY,
    CSC_DMB_FEB_DAV_RATE,
    CSC_DMB_FEB_DAV_UNPACKING_INEFFICIENCY,
    CSC_DMB_FEB_TIMEOUTS,
    CSC_DMB_FEB_UNPACKED_VS_DAV,
    CSC_DMB_FIFO_STATS,
    CSC_DMB_L1A_DISTRIB,
    CSC_DMB_L1A_VS_ALCT_L1A,
    CSC_DMB_L1A_VS_CLCT_L1A,
    CSC_DMB_L1A_VS_DDU_L1A,
    CSC_DMB_L1_PIPE,
    CSC_LCT0_MATCH_BXN_DIFFERENCE,
    CSC_LCT1_MATCH_BXN_DIFFERENCE,
    CSC_LCT_MATCH_STATUS,
    CSC_TMB_ALCT_BXN_DIFF,
    CSC_TMB_ALCT_L1A_DIFF,
    CSC_TMB_BXN_VS_ALCT_BXN,
    CSC_TMB_L1A_VS_ALCT_L1A,
    CSC_TMB_WORD_COUNT,
    CSC_UNPACKED

  };

}

#endif