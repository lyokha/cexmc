/*
 * ============================================================================
 *
 *       Filename:  CexmcRunAction.cc
 *
 *    Description:  run action
 *
 *        Version:  1.0
 *        Created:  20.12.2009 00:18:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include "CexmcRunAction.hh"
#include "CexmcPhysicsManager.hh"
#include "CexmcProductionModel.hh"
#include "CexmcException.hh"


CexmcRunAction::CexmcRunAction( CexmcPhysicsManager *  physicsManager ) :
    physicsManager( physicsManager )
{
}


G4Run *  CexmcRunAction::GenerateRun( void )
{
    return new CexmcRun;
}


void  CexmcRunAction::PrintResults(
                        const CexmcNmbOfHitsInRanges &  nmbOfHitsSampled,
                        const CexmcNmbOfHitsInRanges &  nmbOfHitsTriggeredReal,
                        const CexmcNmbOfHitsInRanges &  nmbOfHitsTriggeredRec,
                        const CexmcAngularRangeList &  angularRanges )
{
    std::ostream::fmtflags  savedFlags( G4cout.flags() );

    G4cout.precision( 4 );
    G4cout << std::fixed;

    for ( CexmcAngularRangeList::const_iterator  k( angularRanges.begin() );
                                                k != angularRanges.end(); ++k )
    {
        G4cout << "       " << k->index + 1 << " [" << k->top << ", " <<
                  k->bottom << ")";
        G4int     total( 0 );
        G4int     triggered( 0 );
        G4double  acc( std::numeric_limits< G4double >::quiet_NaN() );

        CexmcNmbOfHitsInRanges::const_iterator  found(
                                        nmbOfHitsSampled.find( k->index ) );
        if ( found != nmbOfHitsSampled.end() )
        {
            total = found->second;
            acc = 0;
        }

        G4double  accSave( acc );
        found = nmbOfHitsTriggeredReal.find( k->index );
        if ( found != nmbOfHitsTriggeredReal.end() )
        {
            triggered = found->second;
            if ( total > 0 )
                acc = G4double( triggered ) / total;
        }

        G4cout << "  | " << acc << " ( " << triggered << " / " << total << " )";

        triggered = 0;
        acc = accSave;
        found = nmbOfHitsTriggeredRec.find( k->index );
        if ( found != nmbOfHitsTriggeredRec.end() )
        {
            triggered = found->second;
            if ( total > 0 )
                acc = G4double( triggered ) / total;
        }

        G4cout << "  | " << acc << " ( " << triggered << " / " << total <<
                  " )" << G4endl;
    }

    G4cout.flags( savedFlags );
}


void  CexmcRunAction::EndOfRunAction( const G4Run *  run )
{
    const CexmcRun *  theRun( static_cast< const CexmcRun * >( run ) );

    const CexmcNmbOfHitsInRanges &  nmbOfHitsSampled(
                                        theRun->GetNmbOfHitsSampled() );
    const CexmcNmbOfHitsInRanges &  nmbOfHitsTriggeredReal(
                                        theRun->GetNmbOfHitsTriggeredReal() );
    const CexmcNmbOfHitsInRanges &  nmbOfHitsTriggeredRec(
                                        theRun->GetNmbOfHitsTriggeredRec() );

    CexmcProductionModel *          productionModel(
                                        physicsManager->GetProductionModel() );
    if ( ! productionModel )
            throw CexmcException( CexmcWeirdException );

    const CexmcAngularRangeList &  angularRanges(
                                        productionModel->GetAngularRanges() );

    G4cout << " --- Setup acceptances (real, rec):" << G4endl;
    PrintResults( nmbOfHitsSampled, nmbOfHitsTriggeredReal,
                  nmbOfHitsTriggeredRec, angularRanges );
}

