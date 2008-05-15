def csclayout(i, p, *rows): i["Shift/CSC/" + p] = DQMItem(layout=rows)

csclayout(dqmitems,"DDUs in Readout",
        [{ 'path': "CSC/Summary/All_DDUs_in_Readout", 'description': "Please click <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#test01>here</a> for description."}],
        [{ 'path': "CSC/Summary/All_DDUs_L1A_Increment", 'description': "Please click <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#test01>here</a> for description."}])

csclayout(dqmitems,"DDU Reported Errors",
  	[{ 'path': "CSC/Summary/All_DDUs_Trailer_Errors", 'description': "Please click <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#test03>here</a> for description."}])

csclayout(dqmitems,"DDU Format Errors",
        [{ 'path': "CSC/Summary/All_DDUs_Format_Errors", 'description': "Please click <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#test04>here</a> for description."}])

csclayout(dqmitems,"DDU Inputs Status",
  	[{ 'path': "CSC/Summary/All_DDUs_Live_Inputs", 'description': "Please click <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#test05>here</a> for description."}],
	[{ 'path': "CSC/Summary/All_DDUs_Inputs_with_Data", 'description': "Please click <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#test05>here</a> for description."}])

csclayout(dqmitems,"DDU Inputs in ERROR-WARNING State",
  	[{ 'path': "CSC/Summary/All_DDUs_Inputs_Errors", 'description': "Please click <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#test06>here</a> for description."}],
	[{ 'path': "CSC/Summary/All_DDUs_Inputs_Warnings", 'description': "Please click <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#test06>here</a> for description."}])

csclayout(dqmitems,"CSCs Reporting Data and Unpacked",
  	[{ 'path': "CSC/Summary/CSC_Reporting", 'description': "Please click <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#test08>here</a> for description."}],
	[{ 'path': "CSC/Summary/CSC_Unpacked_Fract", 'description': "Please click <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#test08>here</a> for description."}])

csclayout(dqmitems,"CSCs with Errors and Warnings (Fractions)",
  	[{ 'path': "CSC/Summary/CSC_Format_Errors_Fract", 'description': "Please click <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#test10>here</a> for description."},
	 { 'path': "CSC/Summary/CSC_Format_Warnings_Fract", 'description': "Please click <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#test10>here</a> for description."}],
	[{ 'path': "CSC/Summary/CSC_DMB_input_fifo_full_Fract", 'description': "Please click <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#test10>here</a> for description."},
	 { 'path': "CSC/Summary/CSC_DMB_input_timeout_Fract", 'description': "Please click <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#test10>here</a> for description."}])

csclayout(dqmitems,"CSCs without Data Blocks",
  	[{ 'path': "CSC/Summary/CSC_wo_ALCT_Fract", 'description': "Please click <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#test11>here</a> for description."},
	 { 'path': "CSC/Summary/CSC_wo_CLCT_Fract", 'description': "Please click <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#test11>here</a> for description."}],
	[{ 'path': "CSC/Summary/CSC_wo_CFEB_Fract", 'description': "Please click <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftCSC#test11>here</a> for description."},
         ""])

