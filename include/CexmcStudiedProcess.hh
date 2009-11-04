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

#include <G4WrapperProcess.hh>
#include <Randomize.hh>
#include "CexmcCommon.hh"


template  < typename  Particle >
class  CexmcStudiedProcess : public G4WrapperProcess
{
    public:
        CexmcStudiedProcess();

    public:
        G4double  PostStepGetPhysicalInteractionLength( const G4Track &  track,
                    G4double  previousStepSize, G4ForceCondition *  condition );

        G4bool    IsApplicable( const G4ParticleDefinition &  particle );

    public:
        void      SetStepSize( G4double  stepSize_ );

    private:
        G4double  stepSize;
};


template  < typename  Particle >
CexmcStudiedProcess< Particle >::CexmcStudiedProcess() :
        G4WrapperProcess( CexmcStudiedProcessFirstName, fUserDefined ),
        stepSize( DBL_MAX )
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

    *condition = NotForced;
    G4double  curStepSize( G4UniformRand() * stepSize );
    G4cout << "STEP SIZE = " << stepSize << "; CUR STEP SIZE = " <<
              curStepSize << G4endl;
    return curStepSize;
}


template  < typename  Particle >
G4bool  CexmcStudiedProcess< Particle >::IsApplicable(
                                        const G4ParticleDefinition &  particle )
{
    return &particle == Particle::Definition();
}


template  < typename  Particle >
void  CexmcStudiedProcess< Particle >::SetStepSize( G4double  stepSize_ )
{
    stepSize = stepSize_;
}


#endif

