/*
 * ============================================================================
 *
 *       Filename:  CexmcPrimaryVertexInfo.cc
 *
 *    Description:  original position, direction and momentum
 *
 *        Version:  1.0
 *        Created:  07.12.2009 23:58:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include "CexmcPrimaryVertexInfo.hh"


CexmcPrimaryVertexInfo::CexmcPrimaryVertexInfo( const G4ThreeVector &  position,
                    const G4ThreeVector &  direction, G4double  momentumAmp ) :
    position( position ), direction( direction ), momentumAmp( momentumAmp )
{
}


void  CexmcPrimaryVertexInfo::Print( void ) const
{
}

