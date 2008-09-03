/*
 * =====================================================================================
 *
 *       Filename:  CSCDetector.h
 *
 *    Description:  CSC detector functions.
 *
 *        Version:  1.0
 *        Created:  05/19/2008 10:52:21 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Valdas Rapsevicius (VR), Valdas.Rapsevicius@cern.ch
 *        Company:  CERN, CH
 *
 * =====================================================================================
 */

#ifndef CSCDetector_H
#define CSCDetector_H

#include <math.h>
#include <float.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

#define N_SIDES    2
#define N_STATIONS 4
#define N_RINGS    3
#define N_CHAMBERS 36
#define N_LAYERS   6
#define N_CFEBS    5
#define N_HVS      5

#define ADDR_SIZE  7

#define N_ELEMENTS 7740
#define PARTITIONX 50
#define PARTITIONY 50

struct CSCAddressMask {
  bool side;
  bool station;
  bool ring;
  bool chamber;
  bool layer;
  bool cfeb;
  bool hv;
};

struct CSCAddress {

  unsigned int side;
  unsigned int station;
  unsigned int ring;
  unsigned int chamber;
  unsigned int layer;
  unsigned int cfeb;
  unsigned int hv;

  CSCAddressMask mask;

  const bool operator== (const CSCAddress& a) const {
    if (mask.side    == a.mask.side    && mask.side    == true && side    != a.side)    return false;
    if (mask.station == a.mask.station && mask.station == true && station != a.station) return false;
    if (mask.ring    == a.mask.ring    && mask.ring    == true && ring    != a.ring)    return false;
    if (mask.chamber == a.mask.chamber && mask.chamber == true && chamber != a.chamber) return false;
    if (mask.layer   == a.mask.layer   && mask.layer   == true && layer   != a.layer)   return false;
    if (mask.cfeb    == a.mask.cfeb    && mask.cfeb    == true && cfeb    != a.cfeb)    return false;
    if (mask.hv      == a.mask.hv      && mask.hv      == true && hv      != a.hv)      return false;
    return true;
  };

  CSCAddress* operator= (const CSCAddress& a) {
    mask.side    = a.mask.side;
    side         = a.side;
    mask.station = a.mask.station;
    station      = a.station;
    mask.ring    = a.mask.ring;
    ring         = a.ring;
    mask.chamber = a.mask.chamber;
    chamber      = a.chamber;
    mask.layer   = a.mask.layer;
    layer        = a.layer;
    mask.cfeb    = a.mask.cfeb;
    cfeb         = a.cfeb;
    mask.hv      = a.mask.hv;
    hv           = a.hv;
    return this;
  };

};

struct CSCAddressBox {
  CSCAddress adr;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
};

struct CSCAddressBoxPartition {
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  std::vector<unsigned int> boxes;
};

class CSCDetector {

  public:

    CSCDetector();

    const bool NextAddress(unsigned int& i, const CSCAddress*& adr, const CSCAddress& mask) const;
    const bool NextAddressBox(unsigned int& i, const CSCAddressBox*& box, const CSCAddress& mask) const;
    const bool NextAddressBoxByPartition(unsigned int& i, unsigned int& px, unsigned int& py, const CSCAddressBox*& box, const CSCAddress& mask, const float xmin, const float xmax, const float ymin, const float ymax) const;

    const float Area(const unsigned int station) const;
    const float Area(const CSCAddress& adr) const;

    void PrintAddress(const CSCAddress& adr) const;
    const std::string AddressName(const CSCAddress& adr) const;
    const bool AddressFromString(const std::string str_address, CSCAddress& adr) const;

    const unsigned int NumberOfRings(const unsigned int station) const;
    const unsigned int NumberOfChambers(const unsigned int station, const unsigned int ring) const;
    const unsigned int NumberOfChamberCFEBs(const unsigned int station, const unsigned int ring) const;
    const unsigned int NumberOfChamberHVs(const unsigned int station, const unsigned int ring) const;

  private:

    const float Eta(const float r, const float z) const;
    const float EtaToX(const float eta) const;
    const float PhiToY(const float phi) const;
    const float Z(const int station, const int ring) const;
    const float RMinHV(const int station, const int ring, const int n_hv) const;
    const float RMaxHV(const int station, const int ring, const int n_hv) const;
    const float PhiMinCFEB(const int station, const int ring, const int chamber, const int cfeb) const;
    const float PhiMaxCFEB(const int station, const int ring, const int chamber, const int cfeb) const;

    CSCAddressBox boxes[N_ELEMENTS];
    CSCAddressBoxPartition partitions[PARTITIONX][PARTITIONY];
    float station_area[N_STATIONS];

};

#endif
