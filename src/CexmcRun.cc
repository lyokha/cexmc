/*
 * ============================================================================
 *
 *       Filename:  CexmcRun.cc
 *
 *    Description:  run data (acceptances etc.)
 *
 *        Version:  1.0
 *        Created:  19.12.2009 23:59:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include "CexmcRun.hh"


CexmcRun::CexmcRun() : nmbOfSavedEvents( 0 )
{
}


void  CexmcRun::IncrementNmbOfHitsSampled( G4int  index )
{
    CexmcNmbOfHitsInRanges::iterator  found(
                                        nmbOfHitsSampled.find( index ) );
    if ( found == nmbOfHitsSampled.end() )
        nmbOfHitsSampled.insert( std::pair< int, int >( index, 1 ) );
    else
        ++found->second;
}


void  CexmcRun::IncrementNmbOfHitsTriggeredReal( G4int  index )
{
    CexmcNmbOfHitsInRanges::iterator  found(
                                        nmbOfHitsTriggeredReal.find( index ) );
    if ( found == nmbOfHitsTriggeredReal.end() )
        nmbOfHitsTriggeredReal.insert( std::pair< int, int >( index, 1 ) );
    else
        ++found->second;
}


void  CexmcRun::IncrementNmbOfHitsTriggeredRec( G4int  index )
{
    CexmcNmbOfHitsInRanges::iterator  found(
                                        nmbOfHitsTriggeredRec.find( index ) );
    if ( found == nmbOfHitsTriggeredRec.end() )
        nmbOfHitsTriggeredRec.insert( std::pair< int, int >( index, 1 ) );
    else
        ++found->second;
}


void  CexmcRun::IncrementNmbOfOrphanHits( G4int  index )
{
    CexmcNmbOfHitsInRanges::iterator  found(
                                        nmbOfOrphanHits.find( index ) );
    if ( found == nmbOfOrphanHits.end() )
        nmbOfOrphanHits.insert( std::pair< int, int >( index, 1 ) );
    else
        ++found->second;
}


void  CexmcRun::IncrementNmbOfSavedEvents( void )
{
    ++nmbOfSavedEvents;
}
