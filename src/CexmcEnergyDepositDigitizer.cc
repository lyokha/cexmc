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

#include <iostream>
#include <iomanip>
#include <G4DigiManager.hh>
#include <G4String.hh>
#include <Randomize.hh>
#include "CexmcEnergyDepositDigitizer.hh"
#include "CexmcEnergyDepositDigitizerMessenger.hh"
#include "CexmcSimpleEnergyDeposit.hh"
#include "CexmcEnergyDepositInLeftRightSet.hh"
#include "CexmcEnergyDepositInCalorimeter.hh"
#include "CexmcCalorimeterGeometry.hh"
#include "CexmcCommon.hh"


CexmcEnergyDepositDigitizer::CexmcEnergyDepositDigitizer(
                                                    const G4String &  name ) :
    G4VDigitizerModule( name ), monitorED( 0 ),
    vetoCounterEDLeft( 0 ), vetoCounterEDRight( 0 ),
    calorimeterEDLeft( 0 ), calorimeterEDRight( 0 ),
    calorimeterEDLeftMaxX( 0 ), calorimeterEDLeftMaxY( 0 ),
    calorimeterEDRightMaxX( 0 ), calorimeterEDRightMaxY( 0 ),
    monitorHasTriggered( false ), hasTriggered( false ),
    monitorEDThreshold( 0 ),
    vetoCounterEDLeftThreshold( 0 ), vetoCounterEDRightThreshold( 0 ),
    calorimeterEDLeftThreshold( 0 ), calorimeterEDRightThreshold( 0 ),
    calorimeterTriggerAlgorithm( CexmcAllCrystalsMakeEDTriggerThreshold ),
    outerCrystalsVetoAlgorithm( CexmcNoOuterCrystalsVeto ),
    outerCrystalsVetoFraction( 0 ), monitorEDThresholdRef( 0 ),
    vetoCounterEDLeftThresholdRef( 0 ), vetoCounterEDRightThresholdRef( 0 ),
    calorimeterEDLeftThresholdRef( 0 ), calorimeterEDRightThresholdRef( 0 ),
    calorimeterTriggerAlgorithmRef( CexmcAllCrystalsMakeEDTriggerThreshold ),
    outerCrystalsVetoAlgorithmRef( CexmcNoOuterCrystalsVeto ),
    outerCrystalsVetoFractionRef( 0 ), nCrystalsInColumn( 1 ),
    nCrystalsInRow( 1 ), applyFiniteCrystalResolution( false ),
    messenger( NULL )
{
    G4double  crystalWidth;
    G4double  crystalHeight;
    G4double  crystalLength;

    CexmcCalorimeterGeometry::GetGeometryData( nCrystalsInColumn,
                nCrystalsInRow, crystalWidth, crystalHeight, crystalLength );

    if ( nCrystalsInColumn > 0 )
    {
        calorimeterEDLeftCollection.resize( nCrystalsInColumn );
        calorimeterEDRightCollection.resize( nCrystalsInColumn );
    }

    if ( nCrystalsInRow > 0 )
    {
        for ( CexmcEnergyDepositCalorimeterCollection::iterator
                k( calorimeterEDLeftCollection.begin() );
                    k != calorimeterEDLeftCollection.end(); ++k )
        {
            k->resize( nCrystalsInRow );
        }
        for ( CexmcEnergyDepositCalorimeterCollection::iterator
                k( calorimeterEDRightCollection.begin() );
                    k != calorimeterEDRightCollection.end(); ++k )
        {
            k->resize( nCrystalsInRow );
        }
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
    calorimeterEDLeftMaxX = 0;
    calorimeterEDLeftMaxY = 0;
    calorimeterEDRightMaxX = 0;
    calorimeterEDRightMaxY = 0;
    monitorHasTriggered = false;
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
    G4double  outerCrystalsEDLeft( 0 );
    G4double  outerCrystalsEDRight( 0 );
    G4double  innerCrystalsEDLeft( 0 );
    G4double  innerCrystalsEDRight( 0 );

    CexmcRunManager *  runManager( static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );

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
            G4double   value( *k->second );
            if ( applyFiniteCrystalResolution && value > 0. &&
                                                 ! runManager->ProjectIsRead() )
            {
                for ( CexmcEnergyRangeWithDoubleValueList::const_iterator
                          l( crystalResolutionData.begin() );
                          l != crystalResolutionData.end(); ++l )
                {
                    if ( value < l->bottom || value >= l->top )
                        continue;
                    value = G4RandGauss::shoot( value,
                                        value * l->value * CexmcFwhmToStddev );
                    if ( value < 0. )
                        value = 0.;
                    break;
                }
            }
            switch ( side )
            {
            case CexmcLeft :
                if ( value > maxEDCrystalLeft )
                {
                    calorimeterEDLeftMaxX = column;
                    calorimeterEDLeftMaxY = row;
                    maxEDCrystalLeft = value;
                }
                if ( IsOuterCrystal( column, row ) )
                {
                    outerCrystalsEDLeft += value;
                }
                else
                {
                    innerCrystalsEDLeft += value;
                }
                calorimeterEDLeft += value;
                calorimeterEDLeftCollection[ row ][ column ] = value;
                break;
            case CexmcRight :
                if ( value > maxEDCrystalRight )
                {
                    calorimeterEDRightMaxX = column;
                    calorimeterEDRightMaxY = row;
                    maxEDCrystalRight = value;
                }
                if ( IsOuterCrystal( column, row ) )
                {
                    outerCrystalsEDRight += value;
                }
                else
                {
                    innerCrystalsEDRight += value;
                }
                calorimeterEDRight += value;
                calorimeterEDRightCollection[ row ][ column ] = value;
                break;
            default :
                break;
            }
        }
    }

    G4double  calorimeterEDLeftEffective( calorimeterEDLeft );
    G4double  calorimeterEDRightEffective( calorimeterEDRight );

    if ( calorimeterTriggerAlgorithm ==
         CexmcInnerCrystalsMakeEDTriggerThreshold )
    {
        calorimeterEDLeftEffective = innerCrystalsEDLeft;
        calorimeterEDRightEffective = innerCrystalsEDRight;
    }

    monitorHasTriggered = monitorED >= monitorEDThreshold;

    hasTriggered = monitorHasTriggered &&
                   vetoCounterEDLeft < vetoCounterEDLeftThreshold &&
                   vetoCounterEDRight < vetoCounterEDRightThreshold &&
                   calorimeterEDLeftEffective >= calorimeterEDLeftThreshold &&
                   calorimeterEDRightEffective >= calorimeterEDRightThreshold;

    /* event won't trigger if outer crystals veto triggered */
    if ( hasTriggered )
    {
        switch ( outerCrystalsVetoAlgorithm )
        {
        case CexmcNoOuterCrystalsVeto :
            break;
        case CexmcMaximumEDInASingleOuterCrystalVeto :
            hasTriggered =
                    ! IsOuterCrystal( calorimeterEDLeftMaxX,
                                      calorimeterEDLeftMaxY ) &&
                    ! IsOuterCrystal( calorimeterEDRightMaxX,
                                      calorimeterEDRightMaxY );
            break;
        case CexmcFractionOfEDInOuterCrystalsVeto :
            hasTriggered =
                    ( ( outerCrystalsEDLeft / calorimeterEDLeft ) <
                                              outerCrystalsVetoFraction ) &&
                    ( ( outerCrystalsEDRight / calorimeterEDRight ) <
                                              outerCrystalsVetoFraction );
            break;
        default :
            break;
        }
    }
}


std::ostream &  operator<<( std::ostream &  out,
                const CexmcEnergyDepositCalorimeterCollection &  edCollection )
{
    std::streamsize  prec( out.precision() );

    out.precision( 4 );

    out << std::endl;
    for ( CexmcEnergyDepositCalorimeterCollection::const_reverse_iterator
            k( edCollection.rbegin() ); k != edCollection.rend(); ++k )
    {
        for ( CexmcEnergyDepositCrystalRowCollection::const_reverse_iterator
                l( k->rbegin() ); l != k->rend(); ++l )
            out << std::setw( 10 ) << *l;
        out << std::endl;
    }

    out.precision( prec );

    return out;
}

