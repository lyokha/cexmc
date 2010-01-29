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

#include <limits>
#include "CexmcRunAction.hh"
#include "CexmcPhysicsManager.hh"
#include "CexmcProductionModel.hh"
#include "CexmcAngularRange.hh"
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
                    const CexmcNmbOfHitsInRanges &  nmbOfHitsSampledFull,
                    const CexmcNmbOfHitsInRanges &  nmbOfHitsTriggeredRealRange,
                    const CexmcNmbOfHitsInRanges &  nmbOfHitsTriggeredRecRange,
                    const CexmcNmbOfHitsInRanges &  nmbOfOrphanHits,
                    const CexmcAngularRangeList &  angularRanges,
                    G4int  nmbOfFalseHitsTriggeredEDT,
                    G4int  nmbOfFalseHitsTriggeredRec )
{
    std::ostream::fmtflags  savedFlags( G4cout.flags() );
    std::streamsize  prec( G4cout.precision() );

    G4cout.precision( 8 );
    G4cout << std::fixed;

    for ( CexmcAngularRangeList::const_iterator  k( angularRanges.begin() );
                                                k != angularRanges.end(); ++k )
    {
        G4cout << "       " << *k;
        G4int     total( 0 );
        G4int     totalFull( 0 );
        G4int     triggered( 0 );
        G4double  acc( std::numeric_limits< G4double >::quiet_NaN() );

        CexmcNmbOfHitsInRanges::const_iterator  found(
                                        nmbOfHitsSampled.find( k->index ) );
        if ( found != nmbOfHitsSampled.end() )
        {
            total = found->second;
            acc = 0;
        }

        found = nmbOfHitsSampledFull.find( k->index );
        if ( found != nmbOfHitsSampledFull.end() )
        {
            totalFull = found->second;
        }

        G4double  accSave( acc );
        found = nmbOfHitsTriggeredRealRange.find( k->index );
        if ( found != nmbOfHitsTriggeredRealRange.end() )
        {
            triggered = found->second;
            if ( total > 0 )
                acc = G4double( triggered ) / total;
        }

        G4cout << "  | " << acc << " ( " << triggered << " / " << total << " )";

        triggered = 0;
        acc = accSave;
        found = nmbOfHitsTriggeredRecRange.find( k->index );
        if ( found != nmbOfHitsTriggeredRecRange.end() )
        {
            triggered = found->second;
            if ( total > 0 )
                acc = G4double( triggered ) / total;
        }

        G4cout << "  | " << acc << " ( " << triggered << " / " << total <<
                  " / " << totalFull << " )" << G4endl;
    }

    CexmcAngularRangeList  angularGaps;
    GetAngularGaps( angularRanges, angularGaps );

    if ( ! angularGaps.empty() )
    {
        G4cout << "    orphans detected: " << G4endl;
        for ( CexmcAngularRangeList::const_iterator  k( angularGaps.begin() );
                                                k != angularGaps.end(); ++k )
        {
            G4cout << "       " << *k;
            G4int     total( 0 );

            CexmcNmbOfHitsInRanges::const_iterator  found(
                                            nmbOfOrphanHits.find( k->index ) );
            if ( found != nmbOfHitsSampled.end() )
            {
                total = found->second;
            }
            G4cout << " " << total << G4endl;
        }
    }

    G4cout << "       ---" << G4endl;
    G4cout << "       False hits (edt, rec):  " << nmbOfFalseHitsTriggeredEDT <<
              " | " << nmbOfFalseHitsTriggeredRec << G4endl;

    G4cout.precision( prec );
    G4cout.flags( savedFlags );
}


void  CexmcRunAction::EndOfRunAction( const G4Run *  run )
{
    const CexmcRun *  theRun( static_cast< const CexmcRun * >( run ) );

    const CexmcNmbOfHitsInRanges &  nmbOfHitsSampled(
                                        theRun->GetNmbOfHitsSampled() );
    const CexmcNmbOfHitsInRanges &  nmbOfHitsSampledFull(
                                        theRun->GetNmbOfHitsSampledFull() );
    const CexmcNmbOfHitsInRanges &  nmbOfHitsTriggeredRealRange(
                                    theRun->GetNmbOfHitsTriggeredRealRange() );
    const CexmcNmbOfHitsInRanges &  nmbOfHitsTriggeredRecRange(
                                    theRun->GetNmbOfHitsTriggeredRecRange() );
    const CexmcNmbOfHitsInRanges &  nmbOfOrphanHits(
                                        theRun->GetNmbOfOrphanHits() );

    CexmcProductionModel *          productionModel(
                                        physicsManager->GetProductionModel() );
    if ( ! productionModel )
            throw CexmcException( CexmcWeirdException );

    const CexmcAngularRangeList &  angularRanges(
                                        productionModel->GetAngularRanges() );

    G4cout << " --- Setup acceptances (real, rec):" << G4endl;
    PrintResults( nmbOfHitsSampled, nmbOfHitsSampledFull,
                  nmbOfHitsTriggeredRealRange, nmbOfHitsTriggeredRecRange,
                  nmbOfOrphanHits, angularRanges,
                  theRun->GetNmbOfFalseHitsTriggeredEDT(),
                  theRun->GetNmbOfFalseHitsTriggeredRec() );
}

