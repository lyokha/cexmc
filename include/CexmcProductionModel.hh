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
#include "CexmcCommon.hh"

class  CexmcProductionModelMessenger;


struct  CexmcAngularRange
{
    CexmcAngularRange( G4double  top, G4double  bottom, G4int  index ) :
        top( top ), bottom( bottom ), index( index )
    {}

    G4double  top;

    G4double  bottom;

    G4int     index;
};


typedef std::vector< CexmcAngularRange >  CexmcAngularRangeList;


class  CexmcProductionModel
{
    public:
        explicit CexmcProductionModel( G4bool  fermiMotionIsOn = true );

        virtual ~CexmcProductionModel();

    public:
        void  ApplyFermiMotion( G4bool  on );

        void  SetAngularRange( G4double  top, G4double  bottom,
                               G4int  nmbOfDivs );

        void  AddAngularRange( G4double  top, G4double  bottom,
                               G4int  nmbOfDivs );

        void  PrintInitialData( void ) const;

    protected:
        G4bool                 fermiMotionIsOn;

        CexmcAngularRangeList  angularRanges;

    private:
        CexmcProductionModelMessenger *  messenger;
};


inline void  CexmcProductionModel::ApplyFermiMotion( G4bool  on )
{
    fermiMotionIsOn = on;
}


inline void  CexmcProductionModel::SetAngularRange( G4double  top,
                                        G4double  bottom, G4int  nmbOfDivs )
{
    if ( top <= bottom || top > 1.0 || top < -1.0 ||
         bottom > 1.0 || bottom < -1.0 || nmbOfDivs < 1 )
        return;

    angularRanges.clear();
    for ( int  i( 0 ); i < nmbOfDivs; ++i )
    {
        G4double  binWidth( ( top - bottom ) / nmbOfDivs );
        G4double  curTop( top - binWidth * i );
        G4double  curBottom( curTop - binWidth );
        angularRanges.push_back( CexmcAngularRange( curTop, curBottom, i ) );
    }
}


inline void  CexmcProductionModel::AddAngularRange( G4double  top,
                                        G4double  bottom, G4int  nmbOfDivs )
{
    if ( top <= bottom || top > 1.0 || top < -1.0 ||
         bottom > 1.0 || bottom < -1.0 || nmbOfDivs < 1 )
        return;

    G4int  curIndex( angularRanges.size() );
    for ( int  i( 0 ); i < nmbOfDivs; ++i )
    {
        G4double  binWidth( ( top - bottom ) / nmbOfDivs );
        G4double  curTop( top - binWidth * i );
        G4double  curBottom( curTop - binWidth );
        angularRanges.push_back( CexmcAngularRange( curTop, curBottom,
                                                    curIndex + i ) );
    }
}


inline std::ostream &  operator<<( std::ostream &  out,
                                const CexmcAngularRangeList &  angularRanges )
{
    std::ostream::fmtflags  savedFlags( out.flags() );

    out.precision( 4 );
    out << std::endl << std::fixed;
    for ( CexmcAngularRangeList::const_iterator  k( angularRanges.begin() );
                                                k != angularRanges.end(); ++k )
    {
        out << "                 " << k->index  + 1 << " [" << k->top << ", " <<
               k->bottom << ")";
        out << std::endl;
    }

    out.flags( savedFlags );

    return out;
}


inline void  CexmcProductionModel::PrintInitialData( void ) const
{
    const char *  fermiMotionMsg(
                                "Fermi Motion in the target is not applied." );
    if ( fermiMotionIsOn )
        fermiMotionMsg = "Fermi Motion in the target is applied.";

    G4cout << CEXMC_LINE_START << fermiMotionMsg << G4endl;
    G4cout << CEXMC_LINE_START << "Angular ranges:" << angularRanges << G4endl;
}


#endif

