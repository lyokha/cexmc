/*
 * ============================================================================
 *
 *       Filename:  CexmcTrackPointsInCalorimeter.cc
 *
 *    Description:  track points in calorimeter
 *
 *        Version:  1.0
 *        Created:  22.11.2009 21:59:43
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
#include <G4NavigationHistory.hh>
#include <G4UnitsTable.hh>
#include "CexmcTrackPointsInCalorimeter.hh"


G4int  CexmcTrackPointsInCalorimeter::copyDepth0BitsOffset( 8 );
G4int  CexmcTrackPointsInCalorimeter::copyDepth1BitsOffset( 16 );


CexmcTrackPointsInCalorimeter::CexmcTrackPointsInCalorimeter(
                            const G4String &  name ) :
    CexmcTrackPointsInLeftRightSet( name )
{
}


G4int  CexmcTrackPointsInCalorimeter::GetIndex( G4Step *  step )
{
    G4int                        ret( step->GetTrack()->GetTrackID() );
    G4StepPoint *                preStep( step->GetPreStepPoint() );
    const G4VTouchable *         touchable( preStep->GetTouchable() );
    const G4NavigationHistory *  navHistory( touchable->GetHistory() );
    G4int                        navDepth( navHistory->GetDepth() );
    G4String                     volumeName( navHistory->GetVolume(
                                                    navDepth - 2 )->GetName() );
    if ( volumeName.contains( "Right" ) )
        ret |= 1 << leftRightBitsOffset;

    ret |= touchable->GetReplicaNumber( 0 ) << copyDepth0BitsOffset;
    ret |= touchable->GetReplicaNumber( 1 ) << copyDepth1BitsOffset;

    return ret;
}


void  CexmcTrackPointsInCalorimeter::PrintAll( void )
{
    G4int   nmbOfEntries( eventMap->entries() );

    if ( nmbOfEntries == 0 )
        return;

    G4cout << " --- MultiFunctionalDet " << detector->GetName() << G4endl;
    G4cout << "     PrimitiveScorer " << GetName() << G4endl;
    G4cout << "     Number of entries " << nmbOfEntries << G4endl;

    for( std::map< G4int, CexmcTrackPointInfo* >::iterator
                                     itr( eventMap->GetMap()->begin() );
         itr != eventMap->GetMap()->end(); ++itr )
    {
        G4bool  isRightDetector( itr->first >> leftRightBitsOffset );
        G4int   index( itr->first &
                            ( ( 1 << ( leftRightBitsOffset - 1 ) ) |
                              ( ( 1 << ( leftRightBitsOffset - 1 ) ) - 1 ) ) );
        G4int   copyDepth1( index >> copyDepth1BitsOffset );
        index &= ( 1 << ( copyDepth1BitsOffset - 1 ) ) |
                   ( ( 1 << ( copyDepth1BitsOffset - 1 ) ) - 1 );
        G4int   copyDepth0( index >> copyDepth0BitsOffset );
        G4int   trackId( index &
                         ( ( 1 << ( copyDepth0BitsOffset - 1 ) ) |
                           ( ( 1 << ( copyDepth0BitsOffset - 1 ) ) - 1 ) ) );
        const G4String  detectorSide( isRightDetector ? "right" : "left" );
        G4cout << "       " << detectorSide << " detector, row " <<
                copyDepth1 << ", column " << copyDepth0 << G4endl;
        G4cout << "         , track id " << trackId << G4endl;
        G4cout << "         , position: " <<
                G4BestUnit( itr->second->positionLocal, "Length" ) << G4endl;
        G4cout << "         , direction: " <<
                itr->second->directionLocal << G4endl;
        G4cout << "         , momentum: " <<
                G4BestUnit( itr->second->momentumAmp, "Energy" ) << G4endl;
        G4cout << "         , particle: " << itr->second->particleName <<
                G4endl;
    }
}

