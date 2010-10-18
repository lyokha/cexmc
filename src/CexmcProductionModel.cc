/*
 * ============================================================================
 *
 *       Filename:  CexmcProductionModel.cc
 *
 *    Description:  interface to production model
 *
 *        Version:  1.0
 *        Created:  03.11.2009 16:56:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include "CexmcProductionModel.hh"
#include "CexmcProductionModelMessenger.hh"


CexmcProductionModel::CexmcProductionModel( const G4String &  name,
                                            G4bool  fermiMotionIsOn ) :
    name( name ), fermiMotionIsOn( fermiMotionIsOn ), incidentParticle( NULL ),
    nucleusParticle( NULL ), outputParticle( NULL ),
    nucleusOutputParticle( NULL ), messenger( NULL )
{
    angularRanges.push_back( CexmcAngularRange( 1.0, -1.0, 0 ) );
    messenger = new CexmcProductionModelMessenger( this );
}


CexmcProductionModel::~CexmcProductionModel()
{
    delete messenger;
}


void  CexmcProductionModel::SetAngularRange( G4double  top, G4double  bottom,
                                             G4int  nmbOfDivs )
{
    if ( top <= bottom || top > 1.0 || top < -1.0 ||
         bottom > 1.0 || bottom < -1.0 || nmbOfDivs < 1 )
        return;

    CexmcRunManager *  runManager( static_cast< CexmcRunManager * >(
                                           G4RunManager::GetRunManager() ) );
    if ( runManager->ProjectIsRead() )
    {
        G4bool                 isGoodCandidate( false );
        CexmcAngularRangeList  normalizedARanges;
        GetNormalizedAngularRange( angularRangesRef, normalizedARanges );
        for ( CexmcAngularRangeList::iterator  k( normalizedARanges.begin() );
                                            k != normalizedARanges.end(); ++k )
        {
            if ( top <= k->top && bottom >= k->bottom )
            {
                isGoodCandidate = true;
                break;
            }
        }
        if ( ! isGoodCandidate )
            throw CexmcException( CexmcBadAngularRange );
    }

    angularRanges.clear();
    G4double  curBottom( top );
    for ( int  i( 0 ); i < nmbOfDivs; ++i )
    {
        G4double  binWidth( ( top - bottom ) / nmbOfDivs );
        G4double  curTop( curBottom );
        curBottom -=  binWidth;
        angularRanges.push_back( CexmcAngularRange( curTop, curBottom, i ) );
    }
#ifdef CEXMC_USE_ROOT
    CexmcHistoManager::Instance()->SetupARHistos( angularRanges );
#endif
}


void  CexmcProductionModel::AddAngularRange( G4double  top, G4double  bottom,
                                             G4int  nmbOfDivs )
{
    if ( top <= bottom || top > 1.0 || top < -1.0 ||
         bottom > 1.0 || bottom < -1.0 || nmbOfDivs < 1 )
        return;

    CexmcRunManager *  runManager( static_cast< CexmcRunManager * >(
                                           G4RunManager::GetRunManager() ) );
    if ( runManager->ProjectIsRead() )
    {
        G4bool                 isGoodCandidate( false );
        CexmcAngularRangeList  normalizedARanges;
        GetNormalizedAngularRange( angularRangesRef, normalizedARanges );
        for ( CexmcAngularRangeList::iterator  k( normalizedARanges.begin() );
                                            k != normalizedARanges.end(); ++k )
        {
            if ( top <= k->top && bottom >= k->bottom )
            {
                isGoodCandidate = true;
                break;
            }
        }
        if ( ! isGoodCandidate )
            throw CexmcException( CexmcBadAngularRange );
    }

    G4int  curIndex( angularRanges.size() );
    G4double  curBottom( top );
    for ( int  i( 0 ); i < nmbOfDivs; ++i )
    {
        G4double  binWidth( ( top - bottom ) / nmbOfDivs );
        G4double  curTop( curBottom );
        curBottom -= binWidth;
        CexmcAngularRange  aRange( curTop, curBottom, curIndex + i );
        angularRanges.push_back( aRange );
#ifdef CEXMC_USE_ROOT
        CexmcHistoManager::Instance()->AddARHistos( aRange );
#endif
    }
}


void  CexmcProductionModel::SetTriggeredAngularRanges( G4double  opCosThetaSCM )
{
    triggeredAngularRanges.clear();

    for ( CexmcAngularRangeList::iterator  k( angularRanges.begin() );
                                            k != angularRanges.end(); ++k )
    {
        if ( opCosThetaSCM <= k->top && opCosThetaSCM > k->bottom )
            triggeredAngularRanges.push_back( CexmcAngularRange(
                                            k->top, k->bottom, k->index ) );
    }
}


void  CexmcProductionModel::FermiMotionStatusChangeHook( void )
{
}

