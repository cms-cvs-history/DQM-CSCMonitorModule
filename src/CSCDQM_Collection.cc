/*  =====================================================================================
 *
 *       Filename:  CSCDQM_Collection.cc
 *
 *    Description:  Histogram booking code
 *
 *        Version:  1.0
 *        Created:  04/18/2008 03:39:49 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Valdas Rapsevicius (VR), Valdas.Rapsevicius@cern.ch
 *        Company:  CERN, CH
 *
 *  =====================================================================================
 */

#include "DQM/CSCMonitorModule/interface/CSCDQM_Collection.h"

namespace cscdqm {

  Collection::Collection() {
  }

  
  /**
   * @brief  Load XML file and create definitions
   * @param  bookingFile Booking file to load
   * @return 
   */
  void Collection::load(const std::string bookingFile) {

    LOG_INFO << "Booking histograms from " << bookingFile;

    try {

      XMLPlatformUtils::Initialize();

      boost::shared_ptr<XercesDOMParser> parser(new XercesDOMParser());

      parser->setValidationScheme(XercesDOMParser::Val_Always);
      parser->setDoNamespaces(true);
      parser->setDoSchema(true);
      parser->setExitOnFirstFatalError(true);
      parser->setValidationConstraintFatal(true);
      BookingFileErrorHandler eh;
      parser->setErrorHandler(&eh);

      parser->parse(bookingFile.c_str());
      DOMDocument *doc = parser->getDocument();
      DOMNode *docNode = (DOMNode*) doc->getDocumentElement();

      DOMNodeList *itemList = docNode->getChildNodes();

      /// Load histogram definitions
      CoHisto definitions;
      for(uint32_t i = 0; i < itemList->getLength(); i++) {

        DOMNode* node = itemList->item(i);
        if (!isNodeElement(node) || !isNodeName(node, XML_BOOK_DEFINITION)) { continue; }

        CoHistoProps dp;
        getNodeProperties(node, dp);

        DOMElement* el = dynamic_cast<DOMElement*>(node);
        std::string id(XMLString::transcode(el->getAttribute(XMLString::transcode(XML_BOOK_DEFINITION_ID))));
        definitions.insert(make_pair(id, dp));

      }

      /// Loading histograms
      for(uint32_t i = 0; i < itemList->getLength(); i++) {

        DOMNode* node = itemList->item(i);
        if (!isNodeElement(node) || !isNodeName(node, XML_BOOK_HISTOGRAM)) { continue; }

        CoHistoProps hp;

        DOMElement* el = dynamic_cast<DOMElement*>(node);
        if (el->hasAttribute(XMLString::transcode(XML_BOOK_DEFINITION_REF))) {
          std::string id(XMLString::transcode(el->getAttribute(XMLString::transcode(XML_BOOK_DEFINITION_REF))));
          CoHistoProps d = definitions[id];
          for (CoHistoProps::iterator it = d.begin(); it != d.end(); it++) {
            hp[it->first] = it->second;
          }
        }

        getNodeProperties(node, hp);

        std::string name   = hp["Name"];
        std::string prefix = hp["Prefix"];

        CoHistoMap::iterator it = collection.find(prefix);
        if (it == collection.end()) {
          CoHisto h;
          h[name] = hp;
          collection[prefix] = h; 
        } else {
          it->second.insert(make_pair(name, hp));
        }

      }

    } catch (XMLException& e) {
      char* message = XMLString::transcode( e.getMessage() );
      throw Exception(message);
    }

  }
  
  void Collection::getNodeProperties(DOMNode*& node, CoHistoProps& p) const {
    DOMNodeList *props  = node->getChildNodes();
    for(uint32_t j = 0; j < props->getLength(); j++) {
      DOMNode* node = props->item(j);
      if(isNodeElement(node)) { continue; }
      std::string name  = XMLString::transcode(node->getNodeName());
      std::string value = XMLString::transcode(node->getTextContent());
      p[name] = value;
    }
  }

  const bool Collection::isNodeElement(DOMNode*& node) const {
    return (node->getNodeType() == DOMNode::ELEMENT_NODE);
  }

  const bool Collection::isNodeName(DOMNode*& node, const std::string name) const {
    std::string nodeName = XMLString::transcode(node->getNodeName());
    return (nodeName.compare(name) == 0);
  }

  /**
   * @brief  Find string histogram value in map
   * @param  h Histogram map
   * @param  name parameter name
   * @param  value handler for parameter value
   * @return true if parameter found and filled, false - otherwise
   */
  const bool Collection::getHistoValue(const CoHistoProps& h, const std::string& name, std::string& value, const std::string& def_value) {
    CoHistoProps::const_iterator i = h.find(name);
    if(i == h.end()) {
      value = def_value;
      return false;
    } else {
      value = i->second;
    }
    return true;
  }
  
  /**
   * @brief  get Histogram int value out of the map and 
   * @param  h Histogram map
   * @param  name parameter name
   * @param  value handler for parameter value
   * @param  default value if parameter not found 
   * @return pointer to value
   */
  const bool Collection::getHistoValue(const CoHistoProps& h, const std::string& name, int& value, const int& def_value) {
    CoHistoProps::const_iterator i = h.find(name);
    if(i == h.end()) {
      value = def_value;
      return false;
    } else {
      if(EOF == std::sscanf(i->second.c_str(), "%d", &value)) {
        return false;
      }
    }
    return true;
  }
  
  /**
   * @brief  get Histogram double value out of the map and 
   * @param  h Histogram map
   * @param  name parameter name
   * @param  value handler for parameter value
   * @param  default value if parameter not found 
   * @return pointer to value
   */
  const bool Collection::getHistoValue(const CoHistoProps& h, const std::string name, double& value, const int def_value) {
    CoHistoProps::const_iterator i = h.find(name);
    if(i == h.end()) {
      value = def_value;
      return false;
    } else {
      if(EOF == std::sscanf(i->second.c_str(), "%lf", &value)) {
        return false;
      }
    }
    return true;
  }
  
  
  /**
   * @brief  Parse Axis label string and return values in vector
   * @param  s source string to parse
   * @param  labels pointer to result vector
   * @return number of labels found
   */
  const int Collection::ParseAxisLabels(const std::string& s, std::map<int, std::string>& labels) {
    std::string tmp = s;
    std::string::size_type pos = tmp.find("|");
    char* stopstring = NULL;
  
    while (pos != std::string::npos) {
      std::string label_pair = tmp.substr(0, pos);
      tmp.replace(0,pos+1,"");
      if (label_pair.find("=") != std::string::npos) {
        int nbin = strtol(label_pair.substr(0,label_pair.find("=")).c_str(),  &stopstring, 10);
        std::string label = label_pair.substr(label_pair.find("=")+1, label_pair.length());
        while (label.find("\'") != std::string::npos) {
          label.erase(label.find("\'"),1);
        }
        labels[nbin] = label;
      }
      pos = tmp.find("|");
    }
    return labels.size();
  }
  
  /**
   * @brief  Construct CSC bin map
   * @param  tmap pointer to result vector
   * @return 
   */
  void Collection::getCSCTypeToBinMap(std::map<std::string, int>& tmap) {
    tmap["ME-4/2"] = 0;
    tmap["ME-4/1"] = 1;
    tmap["ME-3/2"] = 2;
    tmap["ME-3/1"] = 3;
    tmap["ME-2/2"] = 4;
    tmap["ME-2/1"] = 5;
    tmap["ME-1/3"] = 6;
    tmap["ME-1/2"] = 7;
    tmap["ME-1/1"] = 8;
    tmap["ME+1/1"] = 9;
    tmap["ME+1/2"] = 10;
    tmap["ME+1/3"] = 11;
    tmap["ME+2/1"] = 12;
    tmap["ME+2/2"] = 13;
    tmap["ME+3/1"] = 14;
    tmap["ME+3/2"] = 15;
    tmap["ME+4/1"] = 16;
    tmap["ME+4/2"] = 17;
  }
  
  
  /**
   * @brief  Get CSC label from CSC parameters
   * @param  endcap Endcap number
   * @param  station Station number
   * @param  ring Ring number
   * @return chamber label
   */
  const std::string Collection::getCSCTypeLabel(int endcap, int station, int ring ) {
    std::string label = "Unknown";
    std::ostringstream st;
    if ((endcap > 0) && (station > 0) && (ring > 0)) {
      if (endcap == 1) {
        st << "ME+" << station << "/" << ring;
        label = st.str();
      } else if (endcap==2) {
        st << "ME-" << station << "/" << ring;
        label = st.str();
      } else {
        label = "Unknown";
      }
    }
    return label;
  }
  
  
  /**
   * @brief  Break string into tokens
   * @param  str source string to break
   * @param  tokens pointer to result vector
   * @param  delimiters delimiter string, default " "
   * @return 
   */
  const int Collection::tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters) {
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos) {
      tokens.push_back(str.substr(lastPos, pos - lastPos));
      lastPos = str.find_first_not_of(delimiters, pos);
      pos = str.find_first_of(delimiters, lastPos);
    }
    return tokens.size();
  }
  
  /**
   * @brief  Split string according to delimiter
   * @param  str String to split
   * @param  delim Delimiter
   * @param  results Vector to write results to
   * @return 
   */
  void Collection::splitString(std::string str, const std::string delim, std::vector<std::string>& results) {
    unsigned int cutAt;
    while ((cutAt = str.find_first_of(delim)) != str.npos) {
      if(cutAt > 0) {
        results.push_back(str.substr(0, cutAt));
      }
      str = str.substr(cutAt + 1);
    }
    if(str.length() > 0) {
      results.push_back(str);
    }
  }
  
  
  /**
   * @brief  Trim string
   * @param  str string to trim
   * @return 
   */
  void Collection::trimString(std::string& str) {
    std::string::size_type pos = str.find_last_not_of(' ');
    if(pos != std::string::npos) {
      str.erase(pos + 1);
      pos = str.find_first_not_of(' ');
      if(pos != std::string::npos) 
        str.erase(0, pos);
    } else 
      str.erase(str.begin(), str.end());
  }
  
}