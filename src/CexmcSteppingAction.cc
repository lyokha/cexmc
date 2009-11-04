/*
 * ============================================================================
 *
 *       Filename:  CexmcSteppingAction.cc
 *
 *    Description:  stepping action
 *
 *        Version:  1.0
 *        Created:  27.10.2009 16:03:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4Step.hh>
#include <G4VSolid.hh>
#include <G4Track.hh>
#include <G4Tubs.hh>
#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4ProcessVector.hh>
#include "CexmcSteppingAction.hh"
#include "CexmcStudiedProcess.hh"
#include "CexmcPhysicsManager.hh"
#include "CexmcCommon.hh"


CexmcSteppingAction::CexmcSteppingAction(
                                    CexmcPhysicsManager *  physicsManager ) :
    physicsManager( physicsManager ), firstTimeInTarget( true )
{
}


void  CexmcSteppingAction::UserSteppingAction( const G4Step *  step )
{
    G4StepPoint *           postStepPoint( step->GetPostStepPoint() );
    G4VPhysicalVolume *     volume( postStepPoint-> GetPhysicalVolume() );
    G4Track *               track( step->GetTrack() );
    G4ParticleDefinition *  particle( track->GetDefinition() );
    G4ProcessManager *      processManager( particle->GetProcessManager() );
    G4ProcessVector *       processVector( processManager->GetProcessList() );
    G4int                   processVectorSize(
                                    processManager->GetProcessListLength() );
    G4bool                  activateProcess( false );
    G4double                distanceInTarget( DBL_MAX );

    if ( particle == physicsManager->GetIncidentParticleType() &&
         volume && volume->GetName() == "Target" )
    {
        G4VSolid *  targetSolid( volume->GetLogicalVolume()->GetSolid() );

        if ( firstTimeInTarget )
        {
            G4double  targetRadius( DBL_MAX );
            G4Tubs *  targetTube( dynamic_cast< G4Tubs * >( targetSolid ) );

            if ( targetTube )
            {
                targetRadius = targetTube->GetOuterRadius();
            }
            physicsManager->CalculateProposedMaxIL( targetRadius );
            firstTimeInTarget = false;
        }

        for ( int  i( 0 ); i < processVectorSize; ++i )
        {
            if ( ( *processVector )[ i ]->GetProcessName() ==
                                                CexmcStudiedProcessFullName )
            {
                distanceInTarget = targetSolid->
                        DistanceToOut( postStepPoint->GetPosition(),
                                       postStepPoint->GetMomentumDirection() );
                G4cout << "DISTANCE = " << distanceInTarget << G4endl;
                if ( distanceInTarget > 0. )
                    activateProcess = true;
                break;
            }
        }
    }

    physicsManager->ActivateStudiedProcess( activateProcess, distanceInTarget );
}

