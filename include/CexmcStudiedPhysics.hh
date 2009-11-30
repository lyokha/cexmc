/*
 * =============================================================================
 *
 *       Filename:  CexmcStudiedPhysics.hh
 *
 *    Description:  the studied physics in the target
 *
 *        Version:  1.0
 *        Created:  18.10.2009 16:10:52
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_STUDIED_PHYSICS_HH
#define CEXMC_STUDIED_PHYSICS_HH

#include <G4VPhysicsConstructor.hh>
#include <G4ProcessManager.hh>
#include <G4HadronicProcess.hh>
#include "CexmcStudiedProcess.hh"


template  < typename  Particle, typename  Process >
class  CexmcStudiedPhysics : public G4VPhysicsConstructor
{
    public:
        typedef Process  ProcessType;

    public:
        CexmcStudiedPhysics();

        virtual ~CexmcStudiedPhysics();

    public:
        void  ConstructParticle( void );

        void  ConstructProcess( void );

    protected:
        virtual void  ApplyInteractionModel( G4HadronicProcess *  process );

    private:
        G4bool  wasActivated;
};


template  < typename  Particle, typename  Process >
CexmcStudiedPhysics< Particle, Process >::CexmcStudiedPhysics() :
    G4VPhysicsConstructor( "studiedPhysics" ), wasActivated( false )
{
}


template  < typename  Particle, typename  Process >
CexmcStudiedPhysics< Particle, Process >::~CexmcStudiedPhysics()
{
}


template  < typename  Particle, typename  Process >
void  CexmcStudiedPhysics< Particle, Process >::ConstructParticle( void )
{
    Particle::Definition();
}


template  < typename  Particle, typename  Process >
void  CexmcStudiedPhysics< Particle, Process >::ConstructProcess( void )
{
    if ( wasActivated )
        return;

    wasActivated = true;

    Process *  process( new Process );

    CexmcStudiedProcess< Particle > *  studiedProcess(
                                        new CexmcStudiedProcess< Particle > );

    ApplyInteractionModel( process );

    studiedProcess->RegisterProcess( process );

    theParticleIterator->reset();
    while ( ( *theParticleIterator )() )
    {
        G4ParticleDefinition *  particle( theParticleIterator->value() );
        if ( ! studiedProcess->IsApplicable( *particle ) )
            continue;

        G4ProcessManager *  processManager( particle->GetProcessManager() );
        processManager->AddDiscreteProcess( studiedProcess );
    }
}


template  < typename  Particle, typename  Process >
void  CexmcStudiedPhysics< Particle, Process >::
                    ApplyInteractionModel( G4HadronicProcess * )
{
}


#endif

