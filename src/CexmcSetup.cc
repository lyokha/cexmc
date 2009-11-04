/*
 * =============================================================================
 *
 *       Filename:  CexmcSetup.cc
 *
 *    Description:  physical setup
 *
 *        Version:  1.0
 *        Created:  10.10.2009 23:00:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#include <G4GDMLParser.hh>
#include <G4Region.hh>
#include "CexmcSetup.hh"


G4VPhysicalVolume *  CexmcSetup::Construct( void )
{
    if ( world )
        return world;

    G4GDMLParser parser;

    parser.Read( gdmlFile );
    world = parser.GetWorldVolume();

    return world;
}

