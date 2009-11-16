/*
 * ============================================================================
 *
 *       Filename:  CexmcOriginalParticleEntryPointFilter.cc
 *
 *    Description:  original particle entry point
 *
 *        Version:  1.0
 *        Created:  16.11.2009 22:29:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4String.hh>
#include <G4Step.hh>
#include <G4Track.hh>
#include <G4VProcess.hh>
#include "CexmcOriginalParticleEntryPointFilter.hh"
#include "CexmcCommon.hh"


CexmcOriginalParticleEntryPointFilter::CexmcOriginalParticleEntryPointFilter(
                                                    const G4String &  name ) :
    G4VSDFilter( name )
{
}


G4bool  CexmcOriginalParticleEntryPointFilter::Accept(
                                                    const G4Step *  step ) const
{
    G4Track *  track( step->GetTrack() );
    G4int      trackId( track->GetTrackID() );
    G4String   particleName( track->GetDefinition()->GetParticleName() );

    if ( trackId == 1 )
        return true;

    return false;
}

