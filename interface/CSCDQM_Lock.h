/*
 * =====================================================================================
 *
 *       Filename:  CSCDQM_Lock.h
 *
 *    Description:  Monitor Object interface
 *
 *        Version:  1.0
 *        Created:  10/06/2008 01:49:51 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Valdas Rapsevicius, valdas.rapsevicius@cern.ch
 *        Company:  CERN, CH
 *
 * =====================================================================================
 */

#ifndef CSCDQM_Lock_H
#define CSCDQM_Lock_H

#include <boost/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace cscdqm {

  /**
   * @class Lock
   * @brief Lockable interface that blocks thread
   */
  class Lock {
  
    private:

    public: 

      Lock() { 
        unlock(); 
        lockedByOther = false;
      }

      virtual ~Lock() { }

      void lock() { 
      }

      void unlock() { 
      }

      const bool isLocked() const { 
        return false;
      }

      const bool isLockedByOther() const {
        return false;
      }

  };

}

#endif
