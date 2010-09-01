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
#include "CexmcTrackingAction.hh"
#include "CexmcTrackInfo.hh"
#include "CexmcIncidentParticleTrackInfo.hh"
#include "CexmcPhysicsManager.hh"
#include "CexmcRunManager.hh"
#include "CexmcBasicPhysicsSettings.hh"
#include "CexmcCommon.hh"


CexmcTrackingAction::CexmcTrackingAction(
                                    CexmcPhysicsManager *  physicsManager ) :
    physicsManager( physicsManager ),
    outputParticleTrackId( CexmcInvalidTrackId )
{
}


void  CexmcTrackingAction::PreUserTrackingAction( const G4Track *  track )
{
    CexmcTrackInfo *  trackInfo( static_cast< CexmcTrackInfo * >(
                                                track->GetUserInformation() ) );

    if ( trackInfo )
        return;

    G4Track *  theTrack( const_cast< G4Track * >( track ) );

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
                theTrack->SetUserInformation( trackInfo );
                SetupIncidentParticleTrackInfo( track );
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
            theTrack->SetUserInformation( trackInfo );
            SetupIncidentParticleTrackInfo( track );
            break;
        }
    } while ( false );

    if ( ! trackInfo )
        return;

    if ( ! track->GetUserInformation() )
        theTrack->SetUserInformation( trackInfo );
}


void  CexmcTrackingAction::SetupIncidentParticleTrackInfo(
                                                    const G4Track *  track )
{
    CexmcIncidentParticleTrackInfo *  trackInfo(
                    static_cast< CexmcIncidentParticleTrackInfo * >(
                                                track->GetUserInformation() ) );

    if ( ! trackInfo )
        return;

    G4VPhysicalVolume *  volume( track->GetVolume() );

    if ( volume && volume->GetName() == "Target" )
    {
        physicsManager->ResampleTrackLengthInTarget( track );
        trackInfo->ActivateStudiedProcess();
    }
}

