/*
 * =============================================================================
 *
 *       Filename:  CexmcPrimitiveScorer.cc
 *
 *    Description:  primitive scorer with a messenger
 *
 *        Version:  1.0
 *        Created:  24.01.2011 18:34:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#include <G4ios.hh>
#include "CexmcPrimitiveScorer.hh"
#include "CexmcSensitiveDetectorMessenger.hh"


CexmcPrimitiveScorer::CexmcPrimitiveScorer( const G4String &  name ) :
    G4VPrimitiveScorer( name )
{
}


CexmcPrimitiveScorer::~CexmcPrimitiveScorer()
{
    delete messenger;
}


void  CexmcPrimitiveScorer::InitializeMessenger( void )
{
    messenger = new CexmcSensitiveDetectorMessenger( this );
}


void  CexmcPrimitiveScorer::PrintHeader( G4int  nmbOfEntries ) const
{
    G4cout << " --- MultiFunctionalDet " << detector->GetName() << G4endl;
    G4cout << "     PrimitiveScorer " << primitiveName << G4endl;
    G4cout << "     Number of entries " << nmbOfEntries << G4endl;
}

