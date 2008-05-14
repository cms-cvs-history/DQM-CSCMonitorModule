def csclayout(i, p, *rows): i["Shift/CSC/" + p] = DQMItem(layout=rows)

csclayout(dqmitems,"DDUs in Readout",
        [{ 'path': "CSC/Summary/All_DDUs_in_Readout", 'description': "DESC <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#All_DDUs_in_Readout>more information</a>"}],
        [{ 'path': "CSC/Summary/All_DDUs_L1A_Increment", 'description': "DESC <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#All_DDUs_in_Readout>more information</a>"}])

csclayout(dqmitems,"DDU Reported Errors",
  	[{ 'path': "CSC/Summary/All_DDUs_Trailer_Errors", 'description': "DESC <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#All_DDUs_in_Readout>more information</a>"}])

csclayout(dqmitems,"DDU Format Errors",
  	[{ 'path': "CSC/Summary/All_DDUs_Format_Errors", 'description': "DESC <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#All_DDUs_in_Readout>more information</a>"}])

csclayout(dqmitems,"DDU Inputs Status",
  	[{ 'path': "CSC/Summary/All_DDUs_Live_Inputs", 'description': "DESC <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#All_DDUs_in_Readout>more information</a>"}],
	[{ 'path': "CSC/Summary/All_DDUs_Inputs_with_Data", 'description': "DESC <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#All_DDUs_in_Readout>more information</a>"}])

csclayout(dqmitems,"DDU Inputs in ERROR-WARNING State",
  	[{ 'path': "CSC/Summary/All_DDUs_Inputs_Errors", 'description': "DESC <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#All_DDUs_in_Readout>more information</a>"}],
	[{ 'path': "CSC/Summary/All_DDUs_Inputs_Warnings", 'description': "DESC <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#All_DDUs_in_Readout>more information</a>"}])

csclayout(dqmitems,"CSCs Reporting Data and Unpacked",
  	[{ 'path': "CSC/Summary/CSC_Reporting", 'description': "DESC <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#All_DDUs_in_Readout>more information</a>"}],
	[{ 'path': "CSC/Summary/CSC_Unpacked_Fract", 'description': "DESC <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#All_DDUs_in_Readout>more information</a>"}])

csclayout(dqmitems,"CSCs with Errors and Warnings (Fractions)",
  	[{ 'path': "CSC/Summary/CSC_Format_Errors_Fract", 'description': "DESC <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#All_DDUs_in_Readout>more information</a>"},
	 { 'path': "CSC/Summary/CSC_Format_Warnings_Fract", 'description': "DESC <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#All_DDUs_in_Readout>more information</a>"}],
	[{ 'path': "CSC/Summary/CSC_DMB_input_fifo_full_Fract", 'description': "DESC <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#All_DDUs_in_Readout>more information</a>"},
	 { 'path': "CSC/Summary/CSC_DMB_input_timeout_Fract", 'description': "DESC <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#All_DDUs_in_Readout>more information</a>"}])

csclayout(dqmitems,"CSCs without Data Blocks",
  	[{ 'path': "CSC/Summary/CSC_wo_ALCT_Fract", 'description': "DESC <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#All_DDUs_in_Readout>more information</a>"},
	 { 'path': "CSC/Summary/CSC_wo_CLCT_Fract", 'description': "DESC <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#All_DDUs_in_Readout>more information</a>"}],
	[{ 'path': "CSC/Summary/CSC_wo_CFEB_Fract", 'description': "DESC <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#All_DDUs_in_Readout>more information</a>"},
         ""])

