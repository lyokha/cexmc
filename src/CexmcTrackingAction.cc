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
#include "CexmcRunManager.hh"
#include "CexmcBasicPhysicsSettings.hh"
#include "CexmcCommon.hh"


CexmcTrackingAction::CexmcTrackingAction() : outputParticleTrackId( -1 )
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
        if ( track->GetParentID() == 0 )
        {
            /* good place to reset outputParticleTrackId */
            outputParticleTrackId = -1;
            trackInfo = new CexmcTrackInfo( CexmcIncidentParticleTrack );
            break;
        }
        if ( track->GetCreatorProcess()->GetProcessName() ==
             CexmcStudiedProcessFullName )
        {
            CexmcRunManager *         runManager(
                                        static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );
            CexmcProductionModelType  productionModelType(
                                        runManager->GetProductionModelType() );
            G4ParticleDefinition *    outputParticle(
                        CexmcPMFactoryInstance::GetOutputParticle(
                                                        productionModelType ) );
            G4ParticleDefinition *    nucleusOutputParticle(
                        CexmcPMFactoryInstance::GetNucleusOutputParticle(
                                                        productionModelType ) );
            do
            {
                if ( track->GetDefinition()->GetParticleName() ==
                     outputParticle->GetParticleName() )
                {
                    outputParticleTrackId = track->GetTrackID();
                    trackInfo = new CexmcTrackInfo( CexmcOutputParticleTrack );
                    break;
                }
                if ( track->GetDefinition()->GetParticleName() ==
                     nucleusOutputParticle->GetParticleName() )
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
    } while ( false );

    if ( ! trackInfo )
        return;

    G4Track *  theTrack( const_cast< G4Track * >( track ) );
    theTrack->SetUserInformation( trackInfo );
}

