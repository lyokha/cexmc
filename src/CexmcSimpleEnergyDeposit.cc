/*
 * ============================================================================
 *
 *       Filename:  CexmcSimpleEnergyDeposit.cc
 *
 *    Description:  simple energy deposit scorer
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

#include <G4UnitsTable.hh>
#include <G4HCofThisEvent.hh>
#include "CexmcSimpleEnergyDeposit.hh"


CexmcSimpleEnergyDeposit::CexmcSimpleEnergyDeposit( const G4String &  name ) :
    CexmcPrimitiveScorer( name ), hcId( -1 )
{
}


G4int  CexmcSimpleEnergyDeposit::GetIndex( G4Step * )
{
    return 0;
}


G4bool  CexmcSimpleEnergyDeposit::ProcessHits( G4Step *  step,
                                               G4TouchableHistory * )
{
    G4double  energyDeposit( step->GetTotalEnergyDeposit() );

    if ( energyDeposit == 0. )
        return false;

    eventMap->add( GetIndex( step ), energyDeposit ); 

    return true; 
}


void  CexmcSimpleEnergyDeposit::Initialize( G4HCofThisEvent *  hcOfEvent )
{
    eventMap = new CexmcEnergyDepositCollection( detector->GetName(),
                                                 primitiveName );

    if ( hcId < 0 )
        hcId = GetCollectionID( 0 );

    hcOfEvent->AddHitsCollection( hcId, eventMap );
}


void  CexmcSimpleEnergyDeposit::EndOfEvent( G4HCofThisEvent * )
{
    if ( GetVerboseLevel() > 0 )
        PrintAll();
}


void  CexmcSimpleEnergyDeposit::clear( void )
{
    eventMap->clear();
}


void  CexmcSimpleEnergyDeposit::DrawAll( void )
{
}


void  CexmcSimpleEnergyDeposit::PrintAll( void )
{
    G4int   nmbOfEntries( eventMap->entries() );

    if ( nmbOfEntries == 0 )
        return;

    PrintHeader( nmbOfEntries );

    /* index is always 0 */
    for ( CexmcEnergyDepositCollectionData::iterator
                         itr( eventMap->GetMap()->begin() );
                                     itr != eventMap->GetMap()->end(); ++itr )
    {
        G4cout << "       energy deposit: " <<
                G4BestUnit( *( itr->second ), "Energy" ) << G4endl;
    }
}

