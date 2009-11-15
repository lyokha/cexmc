/*
 * ============================================================================
 *
 *       Filename:  CexmcEnergyDepositInCalorimeter.cc
 *
 *    Description:  energy deposit scorer in calorimeters
 *
 *        Version:  1.0
 *        Created:  14.11.2009 12:48:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4Step.hh>
#include <G4StepPoint.hh>
#include <G4VTouchable.hh>
#include <G4UnitsTable.hh>
#include "CexmcEnergyDepositInCalorimeter.hh"


G4int  CexmcEnergyDepositInCalorimeter::copyDepth1BitsOffset( 8 );


CexmcEnergyDepositInCalorimeter::CexmcEnergyDepositInCalorimeter(
                            const G4String &  name ) :
    CexmcEnergyDepositInLeftRightSet( name )
{
}


G4int  CexmcEnergyDepositInCalorimeter::GetIndex( G4Step *  step )
{
    G4int  ret( CexmcEnergyDepositInLeftRightSet::GetIndex( step ) );

    G4StepPoint *                preStep( step->GetPreStepPoint() );
    const G4VTouchable *         touchable( preStep->GetTouchable() );
    ret |= touchable->GetReplicaNumber( 0 );
    ret |= touchable->GetReplicaNumber( 1 ) << copyDepth1BitsOffset;

    return ret;
}


void  CexmcEnergyDepositInCalorimeter::PrintAll( void )
{
    G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
    G4cout << " PrimitiveScorer " << GetName() << G4endl;
    G4cout << " Number of entries " << eventMap->entries() << G4endl;

    for( std::map< G4int, G4double* >::iterator
                                     itr( eventMap->GetMap()->begin() );
         itr != eventMap->GetMap()->end(); ++itr )
    {
        G4bool  isRightDetector( itr->first >> leftRightBitsOffset );
        G4int   index( itr->first &
                            ( ( 1 << ( leftRightBitsOffset - 1 ) ) |
                              ( ( 1 << ( leftRightBitsOffset - 1 ) ) - 1 ) ) );
        G4int   copyDepth1( index >> copyDepth1BitsOffset );
        G4int   copyDepth0( index &
                            ( ( 1 << ( copyDepth1BitsOffset - 1 ) ) |
                              ( ( 1 << ( copyDepth1BitsOffset - 1 ) ) - 1 ) ) );
        const G4String  detectorSide( isRightDetector ? "right" : "left" );
        G4cout << "  " << detectorSide << " detector, row " << copyDepth1 <<
                ", column " << copyDepth0 << ", energy deposit " <<
                G4BestUnit( *( itr->second ), "Energy" ) << G4endl;
    }
}

