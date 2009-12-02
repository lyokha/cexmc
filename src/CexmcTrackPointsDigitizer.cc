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
#include <G4String.hh>
#include "CexmcTrackPointsDigitizer.hh"
#include "CexmcTrackPoints.hh"
#include "CexmcTrackPointsInLeftRightSet.hh"
#include "CexmcTrackPointsInCalorimeter.hh"
#include "CexmcCalorimeterGeometry.hh"
#include "CexmcCommon.hh"


CexmcTrackPointsDigitizer::CexmcTrackPointsDigitizer( const G4String &  name ) :
    G4VDigitizerModule( name ), hasTriggered( false ), nCrystalsInColumn( 1 ),
    nCrystalsInRow( 1 ), crystalWidth( 0 ), crystalHeight( 0 )
{
    G4double  crystalLength;
    CexmcCalorimeterGeometry::GetGeometryData( nCrystalsInColumn,
                nCrystalsInRow, crystalWidth, crystalHeight, crystalLength );
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
            if ( k->second->trackType != CexmcOutputParticleDecayProductTrack )
                continue;

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
            if ( k->second->trackType != CexmcOutputParticleDecayProductTrack )
                continue;

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

