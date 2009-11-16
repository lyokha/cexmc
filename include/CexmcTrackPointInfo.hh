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


struct  CexmcTrackPointInfo
{
    G4ThreeVector  position;

    G4ThreeVector  direction;

    G4double       momentumAmp;

    G4String       particleName;

    // following type cast operator is only needed by G4THitsMap template
    // (in PrintAll()), it has no actual use
    operator G4double()
    {
        return 0;
    }
};


#endif

