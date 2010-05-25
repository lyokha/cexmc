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

#include <G4VParticleChange.hh>
#include <G4WrapperProcess.hh>
#include "CexmcPhysicsManager.hh"
#include "CexmcIncidentParticleTrackInfo.hh"
#include "CexmcException.hh"


template  < typename  Particle >
class  CexmcStudiedProcess : public G4WrapperProcess
{
    public:
        explicit  CexmcStudiedProcess( CexmcPhysicsManager *  physicsManager );

    public:
        G4double  PostStepGetPhysicalInteractionLength( const G4Track &  track,
                    G4double  previousStepSize, G4ForceCondition *  condition );

        G4VParticleChange *  PostStepDoIt( const G4Track &  track,
                                           const G4Step &  step );

        G4bool    IsApplicable( const G4ParticleDefinition &  particle );

    private:
        CexmcPhysicsManager *  physicsManager;
};


template  < typename  Particle >
CexmcStudiedProcess< Particle >::CexmcStudiedProcess(
                                    CexmcPhysicsManager *  physicsManager ) :
    G4WrapperProcess( CexmcStudiedProcessFirstName, fUserDefined ),
    physicsManager( physicsManager )
{
}


template  < typename  Particle >
G4double  CexmcStudiedProcess< Particle >::
                PostStepGetPhysicalInteractionLength( const G4Track &  track,
                                      G4double , G4ForceCondition *  condition )
{
    *condition = NotForced;

    if ( ! physicsManager->IsStudiedProcessAllowed() )
        return DBL_MAX;

    CexmcIncidentParticleTrackInfo *  trackInfo(
                    dynamic_cast< CexmcIncidentParticleTrackInfo * >(
                                                track.GetUserInformation() ) );

    if ( ! trackInfo )
        return DBL_MAX;

    if ( ! trackInfo->IsStudiedProcessActivated() )
        return DBL_MAX;

    return trackInfo->GetFinalTrackLengthInTarget() -
            trackInfo->GetCurrentTrackLengthInTarget();
}


template  < typename  Particle >
G4VParticleChange *  CexmcStudiedProcess< Particle >::PostStepDoIt(
                                const G4Track &  track, const G4Step &  step )
{
    G4VParticleChange *  particleChange(
                                    pRegProcess->PostStepDoIt( track, step ) );

    if ( particleChange && particleChange->GetTrackStatus() == fStopAndKill )
        physicsManager->IncrementNumberOfTriggeredStudiedInteractions();

    return particleChange;
}


template  < typename  Particle >
G4bool  CexmcStudiedProcess< Particle >::IsApplicable(
                                        const G4ParticleDefinition &  particle )
{
    return particle.GetPDGEncoding() ==
            Particle::Definition()->GetPDGEncoding();
}


#endif

