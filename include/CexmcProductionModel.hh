/*
 * =============================================================================
 *
 *       Filename:  CexmcProductionModel.hh
 *
 *    Description:  interface for production model
 *
 *        Version:  1.0
 *        Created:  03.11.2009 16:50:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_PRODUCTION_MODEL_HH
#define CEXMC_PRODUCTION_MODEL_HH

#include <vector>
#include <G4Types.hh>
#include <G4ios.hh>
#include "CexmcAngularRange.hh"
#include "CexmcProductionModelData.hh"
#include "CexmcRunManager.hh"
#include "CexmcException.hh"
#include "CexmcCommon.hh"

class  CexmcProductionModelMessenger;


class  CexmcProductionModel
{
    public:
        explicit CexmcProductionModel( G4bool  fermiMotionIsOn = true );

        virtual ~CexmcProductionModel();

    public:
        void  ApplyFermiMotion( G4bool  on, G4bool  fromMessenger = true );

        void  SetAngularRange( G4double  top, G4double  bottom,
                               G4int  nmbOfDivs );

        void  SetAngularRanges( const CexmcAngularRangeList &  angularRanges_ );

        void  AddAngularRange( G4double  top, G4double  bottom,
                               G4int  nmbOfDivs );

        void  SetProductionModelData(
                    const CexmcProductionModelData &  productionModelData_ );

        void  PrintInitialData( void ) const;

        const CexmcAngularRangeList &  GetAngularRanges( void ) const;

        const CexmcAngularRangeList &  GetTriggeredAngularRanges( void ) const;

        const CexmcProductionModelData &  GetProductionModelData( void ) const;

        G4bool  IsFermiMotionOn( void ) const;

        void    SetTriggeredAngularRanges( G4double  opCosThetaSCM );

    protected:
        G4bool                    fermiMotionIsOn;

        CexmcAngularRangeList     angularRanges;

        CexmcAngularRangeList     angularRangesRef;

        CexmcAngularRangeList     triggeredAngularRanges;

        CexmcProductionModelData  productionModelData;

    private:
        CexmcProductionModelMessenger *  messenger;
};


inline void  CexmcProductionModel::ApplyFermiMotion( G4bool  on,
                                                     G4bool  fromMessenger )
{
    if ( fromMessenger )
    {
        CexmcRunManager *  runManager( static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );
        if ( runManager->ProjectIsRead() )
            throw CexmcException( CexmcCmdIsNotAllowed );
    }

    fermiMotionIsOn = on;
}


inline void  CexmcProductionModel::SetAngularRange( G4double  top,
                                        G4double  bottom, G4int  nmbOfDivs )
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
}


inline void  CexmcProductionModel::SetAngularRanges(
                                const CexmcAngularRangeList &  angularRanges_ )
{
    angularRangesRef = angularRanges_;
    angularRanges = angularRangesRef;
}


inline void  CexmcProductionModel::AddAngularRange( G4double  top,
                                        G4double  bottom, G4int  nmbOfDivs )
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
        angularRanges.push_back( CexmcAngularRange( curTop, curBottom,
                                                    curIndex + i ) );
    }
}


inline void  CexmcProductionModel::SetProductionModelData(
                        const CexmcProductionModelData &  productionModelData_ )
{
    productionModelData = productionModelData_;
}


inline void  CexmcProductionModel::PrintInitialData( void ) const
{
    const char *  fermiMotionMsg(
                                "Fermi Motion in the target is not applied." );
    if ( fermiMotionIsOn )
        fermiMotionMsg = "Fermi Motion in the target is applied.";

    G4cout << CEXMC_LINE_START << fermiMotionMsg << G4endl;
    G4cout << CEXMC_LINE_START << "Angular ranges:" << angularRanges;
}


inline const CexmcAngularRangeList &
                CexmcProductionModel::GetAngularRanges( void ) const
{
    return angularRanges;
}


inline const CexmcAngularRangeList &
                CexmcProductionModel::GetTriggeredAngularRanges( void ) const
{
    return triggeredAngularRanges;
}


inline const CexmcProductionModelData &
                CexmcProductionModel::GetProductionModelData( void ) const
{
    return productionModelData;
}


inline G4bool  CexmcProductionModel::IsFermiMotionOn( void ) const
{
    return fermiMotionIsOn;
}


#endif

