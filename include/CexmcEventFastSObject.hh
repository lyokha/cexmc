/*
 * =============================================================================
 *
 *       Filename:  CexmcEventFastSObject.hh
 *
 *    Description:  event data serialization helper
 *
 *        Version:  1.0
 *        Created:  02.01.2010 20:21:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_EVENT_FAST_SOBJECT_HH
#define CEXMC_EVENT_FAST_SOBJECT_HH

#ifdef CEXMC_USE_PERSISTENCY

#include <G4Types.hh>


struct  CexmcEventFastSObject
{
    G4int     eventId;

    G4double  opCosThetaSCM;

    G4bool    edDigitizerHasTriggered;

    G4bool    edDigitizerMonitorHasTriggered;
};


namespace  boost
{
    namespace  serialization
    {
        template  < typename  Archive >
        void  serialize( Archive &  archive, CexmcEventFastSObject &  s,
                         const unsigned int )
        {
            archive & s.eventId;
            archive & s.opCosThetaSCM;
            archive & s.edDigitizerHasTriggered;
            archive & s.edDigitizerMonitorHasTriggered;
        }
    }
}

#endif

#endif

