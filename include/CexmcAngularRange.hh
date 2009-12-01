/*
 * =============================================================================
 *
 *       Filename:  CexmcAngularRange.hh
 *
 *    Description:  angular range object
 *
 *        Version:  1.0
 *        Created:  01.12.2009 16:29:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_ANGULAR_RANGE_HH
#define CEXMC_ANGULAR_RANGE_HH

#include <vector>
#include <iostream>
#include <G4Types.hh>


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


#endif

