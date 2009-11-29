/*
 * ============================================================================
 *
 *       Filename:  CexmcTrackPointsDigitizer.cc
 *
 *    Description:  track points collector
 *
 *        Version:  1.0
 *        Created:  24.11.2009 16:34:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4DigiManager.hh>
#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4Box.hh>
#include <G4String.hh>
#include "CexmcTrackPointsDigitizer.hh"
#include "CexmcTrackPoints.hh"
#include "CexmcTrackPointsInLeftRightSet.hh"
#include "CexmcTrackPointsInCalorimeter.hh"
#include "CexmcCommon.hh"


CexmcTrackPointsDigitizer::CexmcTrackPointsDigitizer( const G4String &  name ) :
    G4VDigitizerModule( name ), hasTriggered( false ), nCrystalsInColumn( 1 ),
    nCrystalsInRow( 1 ), crystalWidth( 0 ), crystalHeight( 0 )
{
    const G4LogicalVolumeStore *  lvs( G4LogicalVolumeStore::GetInstance() );
    EAxis                         axis;
    G4int                         nReplicas( 0 );
    G4double                      width;
    G4double                      offset;
    G4bool                        consuming;

    G4LogicalVolume *             lVolume( lvs->GetVolume( "vCalorimeter" ) );
    if ( lVolume )
    {
        G4VPhysicalVolume *  pVolume( lVolume->GetDaughter( 0 ) );
        if ( pVolume && pVolume->IsReplicated() )
        {
            pVolume->GetReplicationData( axis, nReplicas, width, offset,
                                         consuming );
        }
        nCrystalsInColumn = nReplicas;
    }

    lVolume = lvs->GetVolume( "vCrystalRow" );
    if ( lVolume )
    {
        nReplicas = 0;
        G4VPhysicalVolume *  pVolume( lVolume->GetDaughter( 0 ) );
        if ( pVolume && pVolume->IsReplicated() )
        {
            pVolume->GetReplicationData( axis, nReplicas, width, offset,
                                         consuming );
        }
        nCrystalsInRow = nReplicas;
    }

    lVolume = lvs->GetVolume( "vCrystal" );
    if ( lVolume )
    {
        G4Box *  crystalBox( static_cast< G4Box * >( lVolume->GetSolid() ) );
        crystalWidth = crystalBox->GetXHalfLength() * 2;
        crystalHeight = crystalBox->GetYHalfLength() * 2;
    }
}


void  CexmcTrackPointsDigitizer::InitializeData( void )
{
    monitorTP.trackId = -1;
    targetTPIncidentParticle.trackId = -1;
    targetTPOutputParticle.trackId = -1;
    targetTPNucleusParticle.trackId = -1;
    targetTPOutputParticleDecayProductParticle[ 0 ].trackId = -1;
    targetTPOutputParticleDecayProductParticle[ 1 ].trackId = -1;
    vetoCounterTPLeft.trackId = -1;
    vetoCounterTPRight.trackId = -1;
    calorimeterTPLeft.trackId = -1;
    calorimeterTPRight.trackId = -1;
    hasTriggered = false;
}


void  CexmcTrackPointsDigitizer::Digitize( void )
{
    InitializeData();

    G4DigiManager *  digiManager( G4DigiManager::GetDMpointer() );
    G4int            hcId( digiManager->GetHitsCollectionID(
                                                    "vMonitor/Monitor/TP" ) );
    const CexmcTrackPointsCollection *
             hitsCollection( static_cast< const CexmcTrackPointsCollection* >(
                                    digiManager->GetHitsCollection( hcId ) ) );

    if ( hitsCollection )
    {
        for ( std::map< G4int, CexmcTrackPointInfo* >::iterator
                  k( hitsCollection->GetMap()->begin() );
                      k != hitsCollection->GetMap()->end(); ++k )
        {
            monitorTP = *k->second;
            break;
        }
    }

    hcId = digiManager->GetHitsCollectionID( "vTarget/Target/TP" );
    hitsCollection = static_cast< const CexmcTrackPointsCollection* >(
                                    digiManager->GetHitsCollection( hcId ) );

    if ( hitsCollection )
    {
        for ( std::map< G4int, CexmcTrackPointInfo* >::iterator
                  k( hitsCollection->GetMap()->begin() );
                      k != hitsCollection->GetMap()->end(); ++k )
        {
            do
            {
                if ( k->second->trackType == CexmcIncidentParticleTrack )
                {
                    targetTPIncidentParticle = *k->second;
                    break;
                }
                if ( k->second->trackType == CexmcOutputParticleTrack )
                {
                    targetTPOutputParticle = *k->second;
                    hasTriggered = true;
                    break;
                }
                if ( k->second->trackType == CexmcNucleusParticleTrack )
                {
                    targetTPNucleusParticle = *k->second;
                    break;
                }
                if ( k->second->trackType ==
                                        CexmcOutputParticleDecayProductTrack )
                {
                    G4int  index(
                            targetTPOutputParticleDecayProductParticle[ 0 ].
                                                          trackId > 0 ? 1 : 0 );
                    targetTPOutputParticleDecayProductParticle[ index ] =
                            *k->second;
                    break;
                }
            } while ( false );
        }
    }

    hcId = digiManager->GetHitsCollectionID( "vVetoCounter/VetoCounter/TP" );
    hitsCollection = static_cast< const CexmcTrackPointsCollection* >(
                                    digiManager->GetHitsCollection( hcId ) );

    if ( hitsCollection )
    {
        for ( std::map< G4int, CexmcTrackPointInfo* >::iterator
                  k( hitsCollection->GetMap()->begin() );
                      k != hitsCollection->GetMap()->end(); ++k )
        {
            G4int  index( k->first );
            CexmcSide  side( CexmcTrackPointsInLeftRightSet::GetSide(
                                                                   index ) );
            switch ( side )
            {
            case CexmcLeft :
                vetoCounterTPLeft = *k->second;
                break;
            case CexmcRight :
                vetoCounterTPRight = *k->second;
                break;
            default :
                break;
            }
            break;
        }
    }

    hcId = digiManager->GetHitsCollectionID( "vCrystal/Calorimeter/TP" );
    hitsCollection = static_cast< const CexmcTrackPointsCollection* >(
                                    digiManager->GetHitsCollection( hcId ) );

    if ( hitsCollection )
    {
        for ( std::map< G4int, CexmcTrackPointInfo* >::iterator
                  k( hitsCollection->GetMap()->begin() );
                      k != hitsCollection->GetMap()->end(); ++k )
        {
            G4int  index( k->first );
            CexmcSide  side( CexmcTrackPointsInLeftRightSet::GetSide(
                                                                   index ) );
            G4int      row( CexmcTrackPointsInCalorimeter::GetRow( index ) );
            G4int      column( CexmcTrackPointsInCalorimeter::GetColumn(
                                                                   index ) );
            G4double   xInCalorimeterOffset(
                    ( G4double( column ) - G4double( nCrystalsInRow ) / 2 ) *
                                        crystalWidth  + crystalWidth / 2 );
            G4double   yInCalorimeterOffset(
                    ( G4double( row ) - G4double( nCrystalsInColumn ) / 2 ) *
                                        crystalHeight + crystalHeight / 2 );
            switch ( side )
            {
            case CexmcLeft :
                calorimeterTPLeft = *k->second;
                calorimeterTPLeft.positionLocal.setX( xInCalorimeterOffset +
                                        calorimeterTPLeft.positionLocal.x() );
                calorimeterTPLeft.positionLocal.setY( yInCalorimeterOffset +
                                        calorimeterTPLeft.positionLocal.y() );
                break;
            case CexmcRight :
                calorimeterTPRight = *k->second;
                calorimeterTPRight.positionLocal.setX( xInCalorimeterOffset +
                                        calorimeterTPRight.positionLocal.x() );
                calorimeterTPRight.positionLocal.setY( yInCalorimeterOffset +
                                        calorimeterTPRight.positionLocal.y() );
                break;
            default :
                break;
            }
            break;
        }
    }
}

