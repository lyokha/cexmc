/*
 * ============================================================================
 *
 *       Filename:  CexmcTrackPoints.cc
 *
 *    Description:  track points collection
 *
 *        Version:  1.0
 *        Created:  16.11.2009 13:03:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4UnitsTable.hh>
#include <G4HCofThisEvent.hh>
#include "CexmcTrackPoints.hh"
#include "CexmcSensitiveDetectorMessenger.hh"


CexmcTrackPoints::CexmcTrackPoints( const G4String &  name ) :
    G4VPrimitiveScorer( name ), messenger( NULL ), hcId( -1 )
{
    messenger = new CexmcSensitiveDetectorMessenger( this, name );
}


CexmcTrackPoints::~CexmcTrackPoints()
{
    delete messenger;
}


G4int  CexmcTrackPoints::GetIndex( G4Step *  step )
{
    return step->GetTrack()->GetTrackID();
}


G4bool  CexmcTrackPoints::ProcessHits( G4Step *  step,
                                               G4TouchableHistory * )
{
    G4int                   index( GetIndex( step ) );

    if ( ( *eventMap )[ index ] )
        return false;

    G4Track *               track( step->GetTrack() );
    G4ParticleDefinition *  particle( track->GetDefinition() );

    G4StepPoint *  preStepPoint( step->GetPreStepPoint() );
    CexmcTrackPointInfo  trackPointInfo;
    trackPointInfo.position = preStepPoint->GetPosition();
    trackPointInfo.direction = preStepPoint->GetMomentumDirection();
    trackPointInfo.momentumAmp = preStepPoint->GetMomentum().mag();
    trackPointInfo.particleName = particle->GetParticleName();

    eventMap->set( index, trackPointInfo );

    return true; 
}


void  CexmcTrackPoints::Initialize( G4HCofThisEvent *  hcOfEvent )
{
    eventMap = new G4THitsMap< CexmcTrackPointInfo >(
                          GetMultiFunctionalDetector()->GetName(), GetName() );
    if( hcId < 0 )
    {
        hcId = GetCollectionID( 0 );
    }
    hcOfEvent->AddHitsCollection( hcId, eventMap );
}


void  CexmcTrackPoints::EndOfEvent( G4HCofThisEvent * )
{
    if ( GetVerboseLevel() > 0 )
        PrintAll();
}


void  CexmcTrackPoints::clear( void )
{
    eventMap->clear();
}


void  CexmcTrackPoints::DrawAll( void )
{
}


void  CexmcTrackPoints::PrintAll( void )
{
    G4int   nmbOfEntries( eventMap->entries() );

    if ( nmbOfEntries == 0 )
        return;

    G4cout << " --- MultiFunctionalDet " << detector->GetName() << G4endl;
    G4cout << "     PrimitiveScorer " << GetName() << G4endl;
    G4cout << "     Number of entries " << nmbOfEntries << G4endl;

    for( std::map< G4int, CexmcTrackPointInfo* >::iterator
                                     itr( eventMap->GetMap()->begin() );
         itr != eventMap->GetMap()->end(); ++itr )
    {
        G4cout << "       track id " << itr->first << G4endl;
        G4cout << "         , position: " <<
                G4BestUnit( itr->second->position, "Length" ) << G4endl;
        G4cout << "         , direction: " << itr->second->direction << G4endl;
        G4cout << "         , momentum: " <<
                G4BestUnit( itr->second->momentumAmp, "Energy" ) << G4endl;
        G4cout << "         , particle: " << itr->second->particleName <<
                G4endl;
    }
}

