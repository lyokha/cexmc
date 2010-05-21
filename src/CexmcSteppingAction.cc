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
#include <G4Track.hh>
#include <G4VSolid.hh>
#include <G4Tubs.hh>
#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4VTouchable.hh>
#include <G4NavigationHistory.hh>
#include <G4AffineTransform.hh>
#include <G4UnitsTable.hh>
#include "CexmcSteppingAction.hh"
#include "CexmcStudiedProcessBase.hh"
#include "CexmcPhysicsManager.hh"
#include "CexmcIncidentParticleTrackInfo.hh"
#include "CexmcCommon.hh"


CexmcSteppingAction::CexmcSteppingAction(
                                    CexmcPhysicsManager *  physicsManager ) :
    physicsManager( physicsManager ), proposedMaxILInitialized( false ),
    proposedMaxIL( DBL_MAX )
{
}


void  CexmcSteppingAction::UserSteppingAction( const G4Step *  step )
{
    G4Track *                         track( step->GetTrack() );
    CexmcIncidentParticleTrackInfo *  trackInfo(
                    dynamic_cast< CexmcIncidentParticleTrackInfo * >(
                                                track->GetUserInformation() ) );
    if ( ! trackInfo )
        return;

    G4StepPoint *         postStepPoint( step->GetPostStepPoint() );
    G4StepStatus          stepStatus( postStepPoint->GetStepStatus() );
    const G4VTouchable *  touchable( postStepPoint->GetTouchable() );
    G4VPhysicalVolume *   volume( touchable->GetVolume() );

    if ( volume && volume->GetName() == "Target" )
    {
        G4VSolid *  targetSolid( volume->GetLogicalVolume()->GetSolid() );

        if ( ! proposedMaxILInitialized )
        {
            G4double  targetRadius( DBL_MAX );
            G4Tubs *  targetTube( dynamic_cast< G4Tubs * >( targetSolid ) );

            if ( targetTube )
            {
                targetRadius = targetTube->GetOuterRadius();
            }
            proposedMaxIL = physicsManager->GetProposedMaxIL(
                                                        targetRadius * 2 );
            proposedMaxILInitialized = true;
        }

        if ( ! trackInfo->IsStudiedProcessActivated() )
        {
            trackInfo->ResetCurrentTrackLengthInTarget();
            const G4AffineTransform &  transform(
                        postStepPoint->GetTouchable()->GetHistory()->
                        GetTopTransform() );
            G4ThreeVector  position( transform.TransformPoint(
                                        postStepPoint->GetPosition() ) );
            G4ThreeVector  direction( transform.TransformAxis(
                                    postStepPoint->GetMomentumDirection() ) );
            G4double       distanceInTarget( targetSolid->DistanceToOut(
                                                position, direction ) );
            trackInfo->SetFinalTrackLengthInTarget( G4UniformRand() *
                                std::max( distanceInTarget, proposedMaxIL ) );
            trackInfo->ActivateStudiedProcess();
        }

        if ( stepStatus == fPostStepDoItProc )
        {
            const G4VProcess *  process(
                                    postStepPoint->GetProcessDefinedStep() );
            if ( process &&
                 process->GetProcessName() == CexmcStudiedProcessFullName )
            {
                CexmcStudiedProcessBase *  theProcess(
                        static_cast< CexmcStudiedProcessBase * >(
                                const_cast< G4VProcess * >( process ) ) );
                theProcess->IncrementNumberOfTriggeredEvents();
            }
        }

        if ( stepStatus != fGeomBoundary )
            trackInfo->AddTrackLengthInTarget( step->GetStepLength() );
    }

    G4StepPoint *  preStepPoint( step->GetPreStepPoint() );
    touchable = preStepPoint->GetTouchable();
    volume = touchable->GetVolume();

    if ( volume && volume->GetName() == "Target" )
    {
        if ( stepStatus == fGeomBoundary )
            trackInfo->ActivateStudiedProcess( false );
    }
}

