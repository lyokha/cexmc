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

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include "CexmcSimpleTrackPointInfoStore.hh"
#include "CexmcCommon.hh"

class  CexmcTrackPointInfo;


class  CexmcEventSObject
{
    friend class  boost::serialization::access;
    friend class  CexmcRunManager;

    public:
        CexmcEventSObject();

        CexmcEventSObject( G4int  eventId, G4double  monitorED,
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
                const CexmcTrackPointInfo &  calorimeterTPRight );

    private:
        template  < typename  Archive >
        void  serialize( Archive &  archive, const unsigned int  version );

    private:
        G4int                                    eventId;

        G4double                                 monitorED;

        G4double                                 vetoCounterEDLeft;

        G4double                                 vetoCounterEDRight;

        G4double                                 calorimeterEDLeft;

        G4double                                 calorimeterEDRight;

        CexmcEnergyDepositCalorimeterCollection  calorimeterEDLeftCollection;

        CexmcEnergyDepositCalorimeterCollection  calorimeterEDRightCollection;

        CexmcSimpleTrackPointInfoStore           monitorTP;

        CexmcSimpleTrackPointInfoStore           targetTPIncidentParticle;

        CexmcSimpleTrackPointInfoStore           targetTPOutputParticle;

        CexmcSimpleTrackPointInfoStore           targetTPNucleusParticle;

        CexmcSimpleTrackPointInfoStore
                                    targetTPOutputParticleDecayProductParticle1;

        CexmcSimpleTrackPointInfoStore
                                    targetTPOutputParticleDecayProductParticle2;

        CexmcSimpleTrackPointInfoStore           vetoCounterTPLeft;

        CexmcSimpleTrackPointInfoStore           vetoCounterTPRight;

        CexmcSimpleTrackPointInfoStore           calorimeterTPLeft;

        CexmcSimpleTrackPointInfoStore           calorimeterTPRight;
};


template  < typename  Archive >
void  CexmcEventSObject::serialize( Archive &  archive, const unsigned int )
{
    archive & eventId;
    archive & monitorED;
    archive & vetoCounterEDLeft;
    archive & vetoCounterEDRight;
    archive & calorimeterEDLeft;
    archive & calorimeterEDRight;
    archive & calorimeterEDLeftCollection;
    archive & calorimeterEDRightCollection;
    archive & monitorTP;
    archive & targetTPIncidentParticle;
    archive & targetTPOutputParticle;
    archive & targetTPNucleusParticle;
    archive & targetTPOutputParticleDecayProductParticle1;
    archive & targetTPOutputParticleDecayProductParticle2;
    archive & vetoCounterTPLeft;
    archive & vetoCounterTPRight;
    archive & calorimeterTPLeft;
    archive & calorimeterTPRight;
}


#endif

