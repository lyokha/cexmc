/*
 * ============================================================================
 *
 *       Filename:  CexmcAngularRange.cc
 *
 *    Description:  angular range object
 *
 *        Version:  1.0
 *        Created:  28.12.2009 22:35:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <algorithm>
#include <iostream>
#include "CexmcAngularRange.hh"


void  GetNormalizedAngularRange( const CexmcAngularRangeList &  src,
                                 CexmcAngularRangeList &  dst )
{
    dst = src;
    std::sort( dst.begin(), dst.end() );

    for ( CexmcAngularRangeList::iterator  k( dst.begin() + 1 );
                                                            k != dst.end(); )
    {
        if ( k->top == ( k - 1 )->top || k->bottom >= ( k - 1 )->bottom )
        {
            dst.erase( k );
            continue;
        }
        if ( k->top >= ( k - 1 )->bottom )
        {
            ( k - 1 )->bottom = k->bottom;
            dst.erase( k );
            continue;
        }
        ++k;
    }
}


std::ostream &  operator<<( std::ostream &  out,
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

