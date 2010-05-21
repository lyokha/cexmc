/*
 * ============================================================================
 *
 *       Filename:  CexmcTrackingAction.cc
 *
 *    Description:  tracking action
 *
 *        Version:  1.0
 *        Created:  22.11.2009 18:22:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4ParticleDefinition.hh>
#include <G4VProcess.hh>
#include <G4Track.hh>
#include <G4VSolid.hh>
#include <G4Tubs.hh>
#include "CexmcTrackingAction.hh"
#include "CexmcTrackInfo.hh"
#include "CexmcIncidentParticleTrackInfo.hh"
#include "CexmcRunManager.hh"
#include "CexmcBasicPhysicsSettings.hh"
#include "CexmcCommon.hh"


CexmcTrackingAction::CexmcTrackingAction(
                                    CexmcPhysicsManager *  physicsManager ) :
    physicsManager( physicsManager ), outputParticleTrackId( -1 ),
    proposedMaxILInitialized( false ), proposedMaxIL( DBL_MAX )
{
}


void  CexmcTrackingAction::PreUserTrackingAction( const G4Track *  track )
{
    CexmcTrackInfo *  trackInfo( static_cast< CexmcTrackInfo * >(
                                                track->GetUserInformation() ) );

    if ( trackInfo )
        return;

    do
    {
        CexmcRunManager *         runManager(
                                    static_cast< CexmcRunManager * >(
                                        G4RunManager::GetRunManager() ) );
        CexmcProductionModelType  productionModelType(
                                    runManager->GetProductionModelType() );
        G4ParticleDefinition *    incidentParticle(
                        CexmcPMFactoryInstance::GetIncidentParticle(
                                                    productionModelType ) );

        if ( track->GetParentID() == 0 )
        {
            if ( track->GetDefinition()->GetPDGEncoding() ==
                 incidentParticle->GetPDGEncoding() )
            {
                trackInfo = new CexmcIncidentParticleTrackInfo(
                                                CexmcIncidentParticleTrack );
                CexmcIncidentParticleTrackInfo *  theTrackInfo(
                            static_cast< CexmcIncidentParticleTrackInfo * >(
                                                                trackInfo ) );
                SetupIncidentParticleTrackInfo( track, theTrackInfo );
            }
            else
            {
                trackInfo = new CexmcTrackInfo( CexmcIncidentParticleTrack );
            }
            break;
        }

        if ( track->GetCreatorProcess()->GetProcessName() ==
             CexmcStudiedProcessFullName )
        {
            G4ParticleDefinition *    outputParticle(
                        CexmcPMFactoryInstance::GetOutputParticle(
                                                        productionModelType ) );
            G4ParticleDefinition *    nucleusOutputParticle(
                        CexmcPMFactoryInstance::GetNucleusOutputParticle(
                                                        productionModelType ) );
            do
            {
                if ( track->GetDefinition()->GetPDGEncoding() ==
                     outputParticle->GetPDGEncoding() )
                {
                    outputParticleTrackId = track->GetTrackID();
                    trackInfo = new CexmcTrackInfo( CexmcOutputParticleTrack );
                    break;
                }
                if ( track->GetDefinition()->GetPDGEncoding() ==
                     nucleusOutputParticle->GetPDGEncoding() )
                {
                    trackInfo = new CexmcTrackInfo( CexmcNucleusParticleTrack );
                    break;
                }
            } while ( false );
            break;
        }
        if ( track->GetParentID() == outputParticleTrackId )
        {
            trackInfo = new CexmcTrackInfo(
                                        CexmcOutputParticleDecayProductTrack );
            break;
        }

        if ( track->GetDefinition()->GetPDGEncoding() ==
             incidentParticle->GetPDGEncoding() )
        {
            if ( physicsManager->
                 OnlyIncidentParticleCanTriggerStudiedProcess() )
                break;
            trackInfo = new CexmcIncidentParticleTrackInfo( CexmcInsipidTrack );
            CexmcIncidentParticleTrackInfo *  theTrackInfo(
                static_cast< CexmcIncidentParticleTrackInfo * >( trackInfo ) );
            SetupIncidentParticleTrackInfo( track, theTrackInfo );
            break;
        }
    } while ( false );

    if ( ! trackInfo )
        return;

    G4Track *  theTrack( const_cast< G4Track * >( track ) );
    theTrack->SetUserInformation( trackInfo );
}


void  CexmcTrackingAction::SetupIncidentParticleTrackInfo(
                                const G4Track *  track,
                                CexmcIncidentParticleTrackInfo *  trackInfo )
{
    G4VPhysicalVolume *  volume( track->GetVolume() );

    if ( volume && volume->GetName() == "Target" )
    {
        G4VSolid *  targetSolid( volume->GetLogicalVolume()->GetSolid() );

        if ( ! proposedMaxILInitialized )
        {
            G4double  targetRadius( DBL_MAX );
            G4Tubs *  targetTube( dynamic_cast< G4Tubs * >( targetSolid ) );

            if ( targetTube )
                targetRadius = targetTube->GetOuterRadius();

            proposedMaxIL = physicsManager->GetProposedMaxIL(
                                                        targetRadius  * 2 );
            proposedMaxILInitialized = true;
        }

        if ( ! trackInfo->IsStudiedProcessActivated() )
        {
            const G4AffineTransform &  transform(
                        track->GetTouchable()->GetHistory()->
                        GetTopTransform() );
            G4ThreeVector  position( transform.TransformPoint(
                                              track->GetPosition() ) );
            G4ThreeVector  direction( transform.TransformAxis(
                                    track->GetMomentumDirection() ) );
            G4double       distanceInTarget( targetSolid->DistanceToOut(
                                                position, direction ) );
            trackInfo->SetFinalTrackLengthInTarget( G4UniformRand() *
                                std::max( distanceInTarget, proposedMaxIL ) );
            trackInfo->ActivateStudiedProcess();
        }
    }
}

