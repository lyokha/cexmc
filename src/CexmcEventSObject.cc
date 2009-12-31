/*
 * ============================================================================
 *
 *       Filename:  CexmcEventSObject.cc
 *
 *    Description:  event data serialization helper
 *
 *        Version:  1.0
 *        Created:  30.12.2009 17:10:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include "CexmcEventSObject.hh"
#include "CexmcTrackPointInfo.hh"


CexmcEventSObject::CexmcEventSObject()
{
}


CexmcEventSObject::CexmcEventSObject( G4int  eventId, G4double  monitorED,
        G4double  vetoCounterEDLeft, G4double  vetoCounterEDRight,
        G4double  calorimeterEDLeft, G4double  calorimeterEDRight,
        const CexmcEnergyDepositCalorimeterCollection &
                                                calorimeterEDLeftCollection,
        const CexmcEnergyDepositCalorimeterCollection &
                                                calorimeterEDRightCollection,
        const CexmcTrackPointInfo &  monitorTP,
        const CexmcTrackPointInfo &  targetTPIncidentParticle,
        const CexmcTrackPointInfo &  targetTPOutputParticle,
        const CexmcTrackPointInfo &  targetTPNucleusParticle,
        const CexmcTrackPointInfo &
                                    targetTPOutputParticleDecayProductParticle1,
        const CexmcTrackPointInfo &
                                    targetTPOutputParticleDecayProductParticle2,
        const CexmcTrackPointInfo &  vetoCounterTPLeft,
        const CexmcTrackPointInfo &  vetoCounterTPRight,
        const CexmcTrackPointInfo &  calorimeterTPLeft,
        const CexmcTrackPointInfo &  calorimeterTPRight ) :
    eventId( eventId ), monitorED( monitorED ),
    vetoCounterEDLeft( vetoCounterEDLeft ),
    vetoCounterEDRight( vetoCounterEDRight ),
    calorimeterEDLeft( calorimeterEDLeft ),
    calorimeterEDRight( calorimeterEDRight ),
    calorimeterEDLeftCollection( calorimeterEDLeftCollection ),
    calorimeterEDRightCollection( calorimeterEDRightCollection ),
    monitorTP( monitorTP ),
    targetTPIncidentParticle( targetTPIncidentParticle ),
    targetTPOutputParticle( targetTPOutputParticle ),
    targetTPNucleusParticle( targetTPNucleusParticle ),
    targetTPOutputParticleDecayProductParticle1(
                                targetTPOutputParticleDecayProductParticle1 ),
    targetTPOutputParticleDecayProductParticle2(
                                targetTPOutputParticleDecayProductParticle2 ),
    vetoCounterTPLeft( vetoCounterTPLeft ),
    vetoCounterTPRight( vetoCounterTPRight ),
    calorimeterTPLeft( calorimeterTPLeft ),
    calorimeterTPRight( calorimeterTPRight )
{
}

