/*
 * =============================================================================
 *
 *       Filename:  CexmcEventSObject.hh
 *
 *    Description:  event data serialization helper
 *
 *        Version:  1.0
 *        Created:  30.12.2009 16:54:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_EVENT_SOBJECT_HH
#define CEXMC_EVENT_SOBJECT_HH

#ifdef CEXMC_USE_PERSISTENCY

#include <boost/serialization/vector.hpp>
#include "CexmcSimpleTrackPointInfoStore.hh"
#include "CexmcSimpleProductionModelDataStore.hh"
#include "CexmcCommon.hh"


struct  CexmcEventSObject
{
    G4int                                    eventId;

    G4bool                                   edDigitizerMonitorHasTriggered;

    G4double                                 monitorED;

    G4double                                 vetoCounterEDLeft;

    G4double                                 vetoCounterEDRight;

    G4double                                 calorimeterEDLeft;

    G4double                                 calorimeterEDRight;

    CexmcEnergyDepositCalorimeterCollection  calorimeterEDLeftCollection;

    CexmcEnergyDepositCalorimeterCollection  calorimeterEDRightCollection;

    CexmcSimpleTrackPointInfoStore           monitorTP;

    CexmcSimpleTrackPointInfoStore           targetTPBeamParticle;

    CexmcSimpleTrackPointInfoStore           targetTPOutputParticle;

    CexmcSimpleTrackPointInfoStore           targetTPNucleusParticle;

    CexmcSimpleTrackPointInfoStore  targetTPOutputParticleDecayProductParticle1;

    CexmcSimpleTrackPointInfoStore  targetTPOutputParticleDecayProductParticle2;

    CexmcSimpleTrackPointInfoStore           vetoCounterTPLeft;

    CexmcSimpleTrackPointInfoStore           vetoCounterTPRight;

    CexmcSimpleTrackPointInfoStore           calorimeterTPLeft;

    CexmcSimpleTrackPointInfoStore           calorimeterTPRight;

    CexmcSimpleProductionModelDataStore      productionModelData;
};


namespace  boost
{
    namespace  serialization
    {
        template  < typename  Archive >
        void  serialize( Archive &  archive, CexmcEventSObject &  s,
                         const unsigned int )
        {
            archive & s.eventId;
            archive & s.edDigitizerMonitorHasTriggered;
            archive & s.monitorED;
            archive & s.vetoCounterEDLeft;
            archive & s.vetoCounterEDRight;
            archive & s.calorimeterEDLeft;
            archive & s.calorimeterEDRight;
            archive & s.calorimeterEDLeftCollection;
            archive & s.calorimeterEDRightCollection;
            archive & s.monitorTP;
            archive & s.targetTPBeamParticle;
            archive & s.targetTPOutputParticle;
            archive & s.targetTPNucleusParticle;
            archive & s.targetTPOutputParticleDecayProductParticle1;
            archive & s.targetTPOutputParticleDecayProductParticle2;
            archive & s.vetoCounterTPLeft;
            archive & s.vetoCounterTPRight;
            archive & s.calorimeterTPLeft;
            archive & s.calorimeterTPRight;
            archive & s.productionModelData;
        }
    }
}

#endif

#endif

