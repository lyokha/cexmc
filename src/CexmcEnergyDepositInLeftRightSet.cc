/*
 * ============================================================================
 *
 *       Filename:  CexmcEnergyDepositInLeftRightSet.cc
 *
 *    Description:  energy deposit scorer in left/right detector sets
 *                  (e.g. veto counters and calorimeters)
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
#include <G4VPhysicalVolume.hh>
#include <G4UnitsTable.hh>
#include "CexmcEnergyDepositInLeftRightSet.hh"
#include "CexmcSetup.hh"


G4int  CexmcEnergyDepositInLeftRightSet::leftRightBitsOffset( 16 );


CexmcEnergyDepositInLeftRightSet::CexmcEnergyDepositInLeftRightSet(
                        const G4String &  name, const CexmcSetup *  setup ) :
    CexmcSimpleEnergyDeposit( name ), setup( setup )
{
}


G4int  CexmcEnergyDepositInLeftRightSet::GetIndex( G4Step *  step )
{
    G4int                ret( 0 );
    G4StepPoint *        preStep( step->GetPreStepPoint() );
    G4VPhysicalVolume *  pVolume( preStep->GetPhysicalVolume() );

    if ( setup->IsRightDetector( pVolume ) )
        ret |= 1 << leftRightBitsOffset;

    return ret;
}


void  CexmcEnergyDepositInLeftRightSet::PrintAll( void )
{
    G4int   nmbOfEntries( eventMap->entries() );

    if ( nmbOfEntries == 0 )
        return;

    G4cout << " --- MultiFunctionalDet " << detector->GetName() << G4endl;
    G4cout << "     PrimitiveScorer " << GetName() << G4endl;
    G4cout << "     Number of entries " << nmbOfEntries << G4endl;

    for( std::map< G4int, G4double* >::iterator
                                     itr( eventMap->GetMap()->begin() );
         itr != eventMap->GetMap()->end(); ++itr )
    {
        G4bool  isRightDetector( itr->first >> leftRightBitsOffset );
        const G4String  detectorSide( isRightDetector ? "right" : "left" );
        G4cout << "       " << detectorSide << " detector" << G4endl;
        G4cout << "         , energy deposit " <<
                G4BestUnit( *( itr->second ), "Energy" ) << G4endl;
    }
}

