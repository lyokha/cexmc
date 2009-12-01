/*
 * =============================================================================
 *
 *       Filename:  CexmcTrackPointInfo.hh
 *
 *    Description:  single track point information
 *
 *        Version:  1.0
 *        Created:  16.11.2009 12:51:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_TRACK_POINT_INFO_HH
#define CEXMC_TRACK_POINT_INFO_HH

#include <G4ThreeVector.hh>
#include <G4ParticleDefinition.hh>
#include <G4Types.hh>
#include <G4UnitsTable.hh>
#include "CexmcCommon.hh"


struct  CexmcTrackPointInfo
{
    CexmcTrackPointInfo() : trackId( -1 )
    {}

    G4ThreeVector           positionLocal;

    G4ThreeVector           positionWorld;

    G4ThreeVector           directionLocal;

    G4ThreeVector           directionWorld;

    G4double                momentumAmp;

    G4ParticleDefinition *  particle;

    G4int                   trackId;

    CexmcTrackType          trackType;

    // following type cast operator is only needed by G4THitsMap template
    // (in PrintAll()), it has no actual use here
    operator G4double()
    {
        return 0;
    }
};


inline G4bool  IsTPValid( const CexmcTrackPointInfo &  tp )
{
    return tp.trackId != -1;
}


inline std::ostream &  operator<<( std::ostream &  out,
                                const CexmcTrackPointInfo &  trackPointInfo )
{
    if ( ! IsTPValid( trackPointInfo ) )
        return out << "tp is not valid";

    const char *  trackTypeInfo = "???";

    switch ( trackPointInfo.trackType )
    {
    case CexmcIncidentParticleTrack :
        trackTypeInfo = "ip";
        break;
    case CexmcOutputParticleTrack :
        trackTypeInfo = "op";
        break;
    case CexmcNucleusParticleTrack :
        trackTypeInfo = "np";
        break;
    case CexmcOutputParticleDecayProductTrack :
        trackTypeInfo = "opdp";
        break;
    default :
        break;
    }

    std::ostream::fmtflags  savedFlags( out.flags() );

    out.precision( 5 );
    out << trackPointInfo.particle->GetParticleName() << " [" <<
           trackPointInfo.trackId << "," << trackTypeInfo << "] " <<
           G4BestUnit( trackPointInfo.momentumAmp, "Energy" ) << " :  " <<
           G4BestUnit( trackPointInfo.positionLocal, "Length" ) << " [" <<
           trackPointInfo.directionLocal.x() << ", " <<
           trackPointInfo.directionLocal.y() << ", " <<
           trackPointInfo.directionLocal.z() << "]";
#ifdef CEXMC_DEBUG_TP
    out << std::endl << "                            < in world: " <<
           G4BestUnit( trackPointInfo.positionWorld, "Length" ) << " [" <<
           trackPointInfo.directionWorld.x() << ", " <<
           trackPointInfo.directionWorld.y() << ", " <<
           trackPointInfo.directionWorld.z() << "] >";
#endif

    out.flags( savedFlags );

    return out;
}


#endif

