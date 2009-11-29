/*
 * ============================================================================
 *
 *       Filename:  CexmcEnergyDepositDigitizer.cc
 *
 *    Description:  digitizes of energy deposit in a single event
 *
 *        Version:  1.0
 *        Created:  23.11.2009 14:39:41
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
#include <G4String.hh>
#include "CexmcEnergyDepositDigitizer.hh"
#include "CexmcEnergyDepositDigitizerMessenger.hh"
#include "CexmcSimpleEnergyDeposit.hh"
#include "CexmcEnergyDepositInLeftRightSet.hh"
#include "CexmcEnergyDepositInCalorimeter.hh"
#include "CexmcCommon.hh"


CexmcEnergyDepositDigitizer::CexmcEnergyDepositDigitizer(
                                                    const G4String &  name ) :
    G4VDigitizerModule( name ), monitorED( 0 ),
    vetoCounterEDLeft( 0 ), vetoCounterEDRight( 0 ),
    calorimeterEDLeft( 0 ), calorimeterEDRight( 0 ), hasTriggered( false ),
    monitorEDThreshold( 0 ), vetoCounterEDLeftThreshold( 0 ),
    vetoCounterEDRightThreshold( 0 ), calorimeterEDLeftThreshold( 0 ),
    calorimeterEDRightThreshold( 0 ),
    outerCrystalsVetoAlgorithm( CexmcNoOuterCrystalsVeto ),
    outerCrystalsVetoFraction( 0 ), nCrystalsInColumn( 1 ), nCrystalsInRow( 1 ),
    messenger( NULL )
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
        if ( nReplicas > 0 )
        {
            calorimeterEDLeftCollection.resize( nReplicas );
            calorimeterEDRightCollection.resize( nReplicas );
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
        if ( nReplicas > 0 )
        {
            for ( CexmcEnergyDepositCalorimeterCollection::iterator
                    k( calorimeterEDLeftCollection.begin() );
                        k != calorimeterEDLeftCollection.end(); ++k )
            {
                k->resize( nReplicas );
            }
            for ( CexmcEnergyDepositCalorimeterCollection::iterator
                    k( calorimeterEDRightCollection.begin() );
                        k != calorimeterEDRightCollection.end(); ++k )
            {
                k->resize( nReplicas );
            }
        }
        nCrystalsInRow = nReplicas;
    }

    messenger = new CexmcEnergyDepositDigitizerMessenger( this );
}


CexmcEnergyDepositDigitizer::~CexmcEnergyDepositDigitizer()
{
    delete messenger;
}


void  CexmcEnergyDepositDigitizer::InitializeData( void )
{
    monitorED = 0;
    vetoCounterEDLeft = 0;
    vetoCounterEDRight = 0;
    calorimeterEDLeft = 0;
    calorimeterEDRight = 0;
    hasTriggered = false;

    for ( CexmcEnergyDepositCalorimeterCollection::iterator
              k( calorimeterEDLeftCollection.begin() );
                  k != calorimeterEDLeftCollection.end(); ++k )
    {
        for ( CexmcEnergyDepositCrystalRowCollection::iterator
                l( k->begin() ); l != k->end(); ++l )
        {
            *l = 0;
        }
    }
    for ( CexmcEnergyDepositCalorimeterCollection::iterator
              k( calorimeterEDRightCollection.begin() );
                  k != calorimeterEDRightCollection.end(); ++k )
    {
        for ( CexmcEnergyDepositCrystalRowCollection::iterator
                l( k->begin() ); l != k->end(); ++l )
        {
            *l = 0;
        }
    }
}


void  CexmcEnergyDepositDigitizer::Digitize( void )
{
    InitializeData();

    G4DigiManager *  digiManager( G4DigiManager::GetDMpointer() );
    G4int            hcId( digiManager->GetHitsCollectionID(
                                                    "vMonitor/Monitor/ED" ) );
    const CexmcEnergyDepositCollection *
             hitsCollection( static_cast< const CexmcEnergyDepositCollection* >(
                                    digiManager->GetHitsCollection( hcId ) ) );

    if ( hitsCollection )
    {
        /* it always must have index 0 */
        if ( ( *hitsCollection )[ 0 ] )
            monitorED = *( *hitsCollection )[ 0 ];
    }

    hcId = digiManager->GetHitsCollectionID( "vVetoCounter/VetoCounter/ED" );
    hitsCollection = static_cast< const CexmcEnergyDepositCollection* >(
                                    digiManager->GetHitsCollection( hcId ) );
    if ( hitsCollection )
    {
        for ( std::map< G4int, G4double* >::iterator
                  k( hitsCollection->GetMap()->begin() );
                      k != hitsCollection->GetMap()->end(); ++k )
        {
            G4int      index( k->first );
            CexmcSide  side( CexmcEnergyDepositInLeftRightSet::GetSide(
                                                                   index ) );
            switch ( side )
            {
            case CexmcLeft :
                vetoCounterEDLeft = *k->second;
                break;
            case CexmcRight :
                vetoCounterEDRight = *k->second;
                break;
            default :
                break;
            }
        }
    }

    G4double  maxEDCrystalLeft( 0 );
    G4double  maxEDCrystalRight( 0 );
    G4int     maxEDCrystalLeftX( 0 );
    G4int     maxEDCrystalLeftY( 0 );
    G4int     maxEDCrystalRightX( 0 );
    G4int     maxEDCrystalRightY( 0 );
    G4double  outerCrystalsEDLeft( 0 );
    G4double  outerCrystalsEDRight( 0 );

    hcId = digiManager->GetHitsCollectionID( "vCrystal/Calorimeter/ED" );
    hitsCollection = static_cast< const CexmcEnergyDepositCollection* >(
                                    digiManager->GetHitsCollection( hcId ) );
    if ( hitsCollection )
    {
        for ( std::map< G4int, G4double* >::iterator
                  k( hitsCollection->GetMap()->begin() );
                      k != hitsCollection->GetMap()->end(); ++k )
        {
            G4int      index( k->first );
            CexmcSide  side( CexmcEnergyDepositInLeftRightSet::GetSide(
                                                                   index ) );
            G4int      row( CexmcEnergyDepositInCalorimeter::GetRow( index ) );
            G4int      column( CexmcEnergyDepositInCalorimeter::GetColumn(
                                                                   index ) );
            switch ( side )
            {
            case CexmcLeft :
                if ( *k->second > maxEDCrystalLeft )
                {
                    maxEDCrystalLeftX = column;
                    maxEDCrystalLeftY = row;
                }
                if ( IsOuterCrystal( column, row ) )
                {
                    outerCrystalsEDLeft += *k->second;
                }
                calorimeterEDLeft += *k->second;
                calorimeterEDLeftCollection[ row ][ column ] = *k->second;
                break;
            case CexmcRight :
                if ( *k->second > maxEDCrystalRight )
                {
                    maxEDCrystalRightX = column;
                    maxEDCrystalRightY = row;
                }
                if ( IsOuterCrystal( column, row ) )
                {
                    outerCrystalsEDRight += *k->second;
                }
                calorimeterEDRight += *k->second;
                calorimeterEDRightCollection[ row ][ column ] = *k->second;
                break;
            default :
                break;
            }
        }
    }

    hasTriggered = monitorED >= monitorEDThreshold &&
                   vetoCounterEDLeft < vetoCounterEDLeftThreshold &&
                   vetoCounterEDRight < vetoCounterEDRightThreshold &&
                   calorimeterEDLeft >= calorimeterEDLeftThreshold &&
                   calorimeterEDRight >= calorimeterEDRightThreshold;

    /* event won't trigger if outer crystals veto triggered */
    if ( hasTriggered )
    {
        switch ( outerCrystalsVetoAlgorithm )
        {
        case CexmcNoOuterCrystalsVeto :
            break;
        case CexmcMaximumEDInASingleOuterCrystalVeto :
            hasTriggered =
                    ! IsOuterCrystal( maxEDCrystalLeftX, maxEDCrystalLeftY ) &&
                    ! IsOuterCrystal( maxEDCrystalRightX, maxEDCrystalRightY );
            break;
        case CexmcFractionOfEDInOuterCrystalsVeto :
            hasTriggered =
                    ! ( ( outerCrystalsEDLeft / calorimeterEDLeft ) >
                                                outerCrystalsVetoFraction ) &&
                    ! ( ( outerCrystalsEDLeft / calorimeterEDLeft ) >
                                                outerCrystalsVetoFraction );
            break;
        default :
            break;
        }
    }
}

