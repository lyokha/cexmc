/*
 * =============================================================================
 *
 *       Filename:  CexmcTrackPointsStore.hh
 *
 *    Description:  store const references of track points of interest
 *
 *        Version:  1.0
 *        Created:  25.11.2009 13:41:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_TRACK_POINTS_STORE_HH
#define CEXMC_TRACK_POINTS_STORE_HH

#include "CexmcTrackPointInfo.hh"


struct  CexmcTrackPointsStore
{
    CexmcTrackPointsStore( const  CexmcTrackPointInfo &  monitorTP,
    const  CexmcTrackPointInfo &  targetTPIncidentParticle,
    const  CexmcTrackPointInfo &  targetTPOutputParticle,
    const  CexmcTrackPointInfo &  targetTPNucleusParticle,
    const  CexmcTrackPointInfo &  targetTPOutputParticleDecayProductParticle1,
    const  CexmcTrackPointInfo &  targetTPOutputParticleDecayProductParticle2,
    const  CexmcTrackPointInfo &  vetoCounterTPLeft,
    const  CexmcTrackPointInfo &  vetoCounterTPRight,
    const  CexmcTrackPointInfo &  calorimeterTPLeft,
    const  CexmcTrackPointInfo &  calorimeterTPRight ) :
        monitorTP( monitorTP ),
        targetTPIncidentParticle( targetTPIncidentParticle ),
        targetTPOutputParticle( targetTPOutputParticle ),
        targetTPNucleusParticle( targetTPNucleusParticle ),
        targetTPOutputParticleDecayProductParticle1 (
                                targetTPOutputParticleDecayProductParticle1 ),
        targetTPOutputParticleDecayProductParticle2(
                                targetTPOutputParticleDecayProductParticle2 ),
        vetoCounterTPLeft( vetoCounterTPLeft ),
        vetoCounterTPRight( vetoCounterTPRight ),
        calorimeterTPLeft( calorimeterTPLeft ),
        calorimeterTPRight( calorimeterTPRight )
    {}

    const CexmcTrackPointInfo &  monitorTP;
    
    const CexmcTrackPointInfo &  targetTPIncidentParticle;
    
    const CexmcTrackPointInfo &  targetTPOutputParticle;
    
    const CexmcTrackPointInfo &  targetTPNucleusParticle;
    
    const CexmcTrackPointInfo &  targetTPOutputParticleDecayProductParticle1;
    
    const CexmcTrackPointInfo &  targetTPOutputParticleDecayProductParticle2;

    const CexmcTrackPointInfo &  vetoCounterTPLeft;

    const CexmcTrackPointInfo &  vetoCounterTPRight;

    const CexmcTrackPointInfo &  calorimeterTPLeft;

    const CexmcTrackPointInfo &  calorimeterTPRight;
};


#endif

