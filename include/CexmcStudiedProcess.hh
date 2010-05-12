/*
 * =============================================================================
 *
 *       Filename:  CexmcStudiedProcess.hh
 *
 *    Description:  studied process in the target
 *
 *        Version:  1.0
 *        Created:  26.10.2009 20:41:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_STUDIED_PROCESS_HH
#define CEXMC_STUDIED_PROCESS_HH

#include <Randomize.hh>
#include "CexmcStudiedProcessBase.hh"
#include "CexmcTrackInfo.hh"
#include "CexmcException.hh"


template  < typename  Particle >
class  CexmcStudiedProcess : public CexmcStudiedProcessBase
{
    public:
        CexmcStudiedProcess();

    public:
        G4double  PostStepGetPhysicalInteractionLength( const G4Track &  track,
                    G4double  previousStepSize, G4ForceCondition *  condition );

        G4bool    IsApplicable( const G4ParticleDefinition &  particle );

    public:
        void      SetMaxStepSize( G4double  value );

    private:
        G4double  maxStepSize;

        G4double  stepSize;
};


template  < typename  Particle >
CexmcStudiedProcess< Particle >::CexmcStudiedProcess() :
        maxStepSize( DBL_MAX ), stepSize( DBL_MAX )
{
}


template  < typename  Particle >
G4double  CexmcStudiedProcess< Particle >::
                PostStepGetPhysicalInteractionLength( const G4Track &  track,
                    G4double  previousStepSize, G4ForceCondition *  condition )
{
    /* make G4HadronicProcess happy with its private(!) ModelingState */
    G4WrapperProcess::PostStepGetPhysicalInteractionLength( track,
                                                previousStepSize, condition );

    CexmcTrackInfo *  trackInfo( static_cast< CexmcTrackInfo * >(
                                                track.GetUserInformation() ) );

    if ( ! trackInfo )
        throw CexmcException( CexmcWeirdException );

    if ( ! trackWatched )
    {
        stepSize = G4UniformRand() * maxStepSize +
                                        trackInfo->GetTrackLengthInTarget();
        trackWatched = true;
    }

    *condition = NotForced;

    return stepSize - trackInfo->GetTrackLengthInTarget();
}


template  < typename  Particle >
G4bool  CexmcStudiedProcess< Particle >::IsApplicable(
                                        const G4ParticleDefinition &  particle )
{
    return &particle == Particle::Definition();
}


template  < typename  Particle >
void  CexmcStudiedProcess< Particle >::SetMaxStepSize( G4double  value )
{
    maxStepSize = value;
}


#endif

