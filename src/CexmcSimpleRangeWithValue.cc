/*
 * =============================================================================
 *
 *       Filename:  CexmcSimpleRangeWithValue.cc
 *
 *    Description:  auxiliary functions for simple range instances
 *
 *        Version:  1.0
 *        Created:  17.02.2010 22:46:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#include "CexmcSimpleRangeWithValue.hh"
#include <G4UnitsTable.hh>


std::ostream &  operator<<( std::ostream &  out,
                        const CexmcEnergyRangeWithDoubleValue &  range )
{
    std::ostream::fmtflags  savedFlags( out.flags() );
    std::streamsize         prec( out.precision() );

    out.precision( 6 );
    out.flags( std::ios::fixed );

    out << " [" << G4BestUnit( range.bottom, "Energy" ) << ", " <<
                   G4BestUnit( range.top, "Energy" ) << ")  " << range.value;

    out.precision( prec );
    out.flags( savedFlags );

    return out;
}


std::ostream &  operator<<( std::ostream &  out,
                        const CexmcEnergyRangeWithDoubleValueList &  ranges )
{
    out << std::endl;
    for ( CexmcEnergyRangeWithDoubleValueList::const_iterator
                                  k( ranges.begin() ); k != ranges.end(); ++k )
    {
        out << "                 " << *k << std::endl;
    }

    return out;
}

