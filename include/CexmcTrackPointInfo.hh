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
#include <G4String.hh>
#include <G4Types.hh>
#include <G4UnitsTable.hh>
#include "CexmcCommon.hh"


struct  CexmcTrackPointInfo
{
    CexmcTrackPointInfo() : trackId( -1 )
    {}

    G4ThreeVector   position;

    G4ThreeVector   direction;

    G4double        momentumAmp;

    G4String        particleName;

    G4int           trackId;

    CexmcTrackType  trackType;

    // following type cast operator is only needed by G4THitsMap template
    // (in PrintAll()), it has no actual use
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
    case CexmcIncidentParticleTrack:
        trackTypeInfo = "ip";
        break;
    case CexmcOutputParticleTrack:
        trackTypeInfo = "op";
        break;
    case CexmcNucleusParticleTrack:
        trackTypeInfo = "np";
        break;
    case CexmcOutputParticleDecayProductTrack:
        trackTypeInfo = "opdp";
        break;
    default:
        break;
    }

    out.precision( 5 );
    out << trackPointInfo.particleName << " [" <<
           trackPointInfo.trackId << "," << trackTypeInfo << "] " <<
           G4BestUnit( trackPointInfo.momentumAmp, "Energy" ) << " :  " <<
           G4BestUnit( trackPointInfo.position, "Length" ) << " [" <<
           trackPointInfo.direction.x() << ", " <<
           trackPointInfo.direction.y() << ", " <<
           trackPointInfo.direction.z() << "]";

    return out;
}


#endif

