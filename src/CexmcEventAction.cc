/*
 * ============================================================================
 *
 *       Filename:  CexmcEventAction.cc
 *
 *    Description:  event action
 *
 *        Version:  1.0
 *        Created:  27.10.2009 22:48:08
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4ProcessVector.hh>
#include "CexmcEventAction.hh"
#include "CexmcPhysicsManager.hh"
#include "CexmcCommon.hh"


CexmcEventAction::CexmcEventAction( CexmcPhysicsManager *  physicsManager ) :
    physicsManager( physicsManager )
{
}


void  CexmcEventAction::BeginOfEventAction( const G4Event * )
{
    const G4ParticleDefinition *  particle(
                                    physicsManager->GetIncidentParticleType() );
    G4ProcessManager *            processManager(
                                    particle->GetProcessManager() );
    G4ProcessVector *             processVector(
                                    processManager->GetProcessList() );
    G4int                         processVectorSize(
                                    processManager->GetProcessListLength() );

    for ( int  i( 0 ); i < processVectorSize; ++i )
    {
        if ( ( *processVector )[ i ]->GetProcessName() ==
                                                CexmcStudiedProcessFullName )
        {
            physicsManager->ActivateStudiedProcess( false );
            break;
        }
    }
}

