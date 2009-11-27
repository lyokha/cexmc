/*
 * ============================================================================
 *
 *       Filename:  CexmcMessenger.cc
 *
 *    Description:  common messenger stuff (directories etc.)
 *
 *        Version:  1.0
 *        Created:  15.11.2009 12:59:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include "CexmcMessenger.hh"
#include <G4UIdirectory.hh>


CexmcMessenger *  CexmcMessenger::instance( NULL );

G4String  CexmcMessenger::mainDirName( "/cexmc/" );
G4String  CexmcMessenger::geometryDirName( CexmcMessenger::mainDirName +
                                           "geometry/" );
G4String  CexmcMessenger::physicsDirName( CexmcMessenger::mainDirName +
                                          "physics/" );
G4String  CexmcMessenger::gunDirName( CexmcMessenger::mainDirName +
                                      "gun/" );
G4String  CexmcMessenger::detectorDirName( CexmcMessenger::mainDirName +
                                           "detector/" );
G4String  CexmcMessenger::eventDirName( CexmcMessenger::mainDirName +
                                           "event/" );
G4String  CexmcMessenger::runDirName( CexmcMessenger::mainDirName +
                                           "run/" );


CexmcMessenger *  CexmcMessenger::Instance( void )
{
    if ( instance == NULL )
        instance = new CexmcMessenger;

    return instance;
}


void  CexmcMessenger::Destroy( void )
{
    delete instance;
    instance = NULL;
}


CexmcMessenger::CexmcMessenger() : mainDir( NULL ), geometryDir( NULL ),
    physicsDir( NULL ), gunDir( NULL ), detectorDir( NULL ), eventDir( NULL ),
    runDir( NULL )
{
    mainDir = new G4UIdirectory( mainDirName );
    mainDir->SetGuidance( "Cexmc settings." );
    geometryDir = new G4UIdirectory( geometryDirName );
    geometryDir->SetGuidance( "Geometry settings (gdml file etc.)" );
    physicsDir = new G4UIdirectory( physicsDirName );
    physicsDir->SetGuidance( "Physics related settings "
                             "(production model etc.)" );
    gunDir = new G4UIdirectory( gunDirName );
    gunDir->SetGuidance( "Gun settings (different FWHMs etc.)" );
    detectorDir = new G4UIdirectory( detectorDirName );
    detectorDir->SetGuidance( "Sensitive detectors settings" );
    eventDir = new G4UIdirectory( eventDirName );
    eventDir->SetGuidance( "Event settings (verbose level etc.)" );
    runDir = new G4UIdirectory( runDirName );
    runDir->SetGuidance( "Run settings (results directory etc.)" );
}


CexmcMessenger::~CexmcMessenger()
{
    delete mainDir;
    delete geometryDir;
    delete physicsDir;
    delete gunDir;
    delete detectorDir;
    delete eventDir;
    delete runDir;
}

