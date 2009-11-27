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
#include <G4VTouchable.hh>
#include <G4NavigationHistory.hh>
#include <G4AffineTransform.hh>
#include <G4UnitsTable.hh>
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
    const G4VTouchable *    touchable( postStepPoint->GetTouchable() );
    G4VPhysicalVolume *     volume( touchable->GetVolume() );
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
                const G4AffineTransform &  transform(
                                            postStepPoint->GetTouchable()->
                                            GetHistory()->GetTopTransform() );
                G4ThreeVector  position( transform.TransformPoint(
                                    postStepPoint->GetPosition() ) );
                G4ThreeVector  direction( transform.TransformAxis(
                                    postStepPoint->GetMomentumDirection() ) );
                distanceInTarget = targetSolid->DistanceToOut( position,
                                                               direction );
                G4cout << "POSITION = " << G4BestUnit(
                                        position, "Length" ) << G4endl;
                G4cout << "DISTANCE = " << G4BestUnit(
                                        distanceInTarget, "Length" ) << G4endl;
                if ( distanceInTarget > 0. )
                    activateProcess = true;
                break;
            }
        }
    }

    physicsManager->ActivateStudiedProcess( activateProcess, distanceInTarget );
}

