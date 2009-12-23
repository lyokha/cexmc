/*
 * ============================================================================
 *
 *       Filename:  CexmcRunManager.cc
 *
 *    Description:  run manager
 *
 *        Version:  1.0
 *        Created:  03.11.2009 20:27:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <stdlib.h>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <G4ScoringManager.hh>
#include <G4UImanager.hh>
#include <G4Timer.hh>
#include "CexmcRunManager.hh"
#include "CexmcRunManagerMessenger.hh"
#include "CexmcRunSObject.hh"
#include "CexmcPhysicsManager.hh"
#include "CexmcProductionModel.hh"
#include "CexmcEventInfo.hh"
#include "CexmcException.hh"


CexmcRunManager::CexmcRunManager( const G4String &  projectId,
                                  const G4String &  rProject ) :
    productionModelType( CexmcUnknownProductionModel ),
    gdmlFileName( "default.gdml" ), projectsDir( "." ), projectId( projectId ),
    rProject( rProject ), eventCountPolicy( CexmcCountAllEvents ),
    messenger( NULL )
{
    /* this exception must be caught before creating the object! */
    if ( rProject != "" && rProject == projectId )
        throw CexmcException( CexmcWeirdException );

    const char *  projectsDirEnv( getenv( "CEXMC_PROJECTS_DIR" ) );

    if ( projectsDirEnv )
        projectsDir = projectsDirEnv;

    messenger = new CexmcRunManagerMessenger( this );

    if ( ProjectIsRead() )
        ReadProject();
}


CexmcRunManager::~CexmcRunManager()
{
    if ( ProjectIsRead() )
    {
        G4String  cmd( G4String( "bzip2 " ) + projectsDir + "/" + rProject +
                       ".gdml" );
        if ( system( cmd ) != 0 )
            G4cerr << "Failed to zip geometry data" << G4endl;
    }

    delete messenger;
}


void  CexmcRunManager::ReadProject( void )
{
    if ( ! ProjectIsRead() )
        return;

    /* read run data */
    CexmcRunSObject  sObject;
    std::ifstream    runDataFile( ( projectsDir + "/" + rProject + ".rdb" ).
                                  c_str() );
    if ( ! runDataFile )
        throw CexmcException( CexmcReadProjectIncompleteException );

    {
        boost::archive::binary_iarchive  archive( runDataFile );
        archive >> sObject;
    }

    productionModelType = sObject.productionModelType;
    G4cout << "READ ARs: " << sObject.angularRanges << G4cout;

    /* read gdml file */
    G4String  cmd( G4String( "cp " ) + projectsDir + "/" + rProject +
                   ".gdml.bz2 " + projectsDir + "/" + projectId +
                   ".gdml.bz2" );
    if ( ProjectIsSaved() && system( cmd ) != 0 )
        throw CexmcException( CexmcReadProjectIncompleteException );

    cmd = G4String( "bunzip2 " ) + projectsDir + "/" + rProject + ".gdml.bz2";
    if ( system( cmd ) != 0 )
        throw CexmcException( CexmcFileCompressException );

    gdmlFileName = projectsDir + "/" + rProject + ".gdml";
}


void  CexmcRunManager::SaveProject( void )
{
    if ( ! ProjectIsSaved() )
        return;

    /* save run data */
    if ( ! physicsManager )
        throw CexmcException( CexmcWeirdException );

    CexmcRunSObject  sObject( productionModelType,
                    physicsManager->GetProductionModel()->GetAngularRanges() );
    std::ofstream    runDataFile( ( projectsDir + "/" + projectId + ".rdb" ).
                                  c_str() );

    {
        boost::archive::binary_oarchive  archive( runDataFile );
        archive << sObject;
    }

    /* save gdml file */
    G4String  cmd( G4String( "bzip2 -c " ) + gdmlFileName + " > " +
                   projectsDir + "/" + projectId + ".gdml.bz2" );
    if ( system( cmd ) != 0 )
        throw CexmcException( CexmcFileCompressException );
}


/* mostly adopted from G4RunManager::DoEventLoop() */
void  CexmcRunManager::DoEventLoop( G4int  nEvent, const char *  macroFile,
                                    G4int  nSelect )
{
    if ( verboseLevel > 0 ) 
        timer->Start();

    G4String  msg;
    if ( macroFile != 0 )
    {
        if ( nSelect < 0 )
            nSelect = nEvent;
        msg = "/control/execute ";
        msg += macroFile;
    }
    else
    {
        nSelect = -1;
    }

    G4int  iEvent( 0 );
    G4int  iEventEffective( 0 );

    for( iEvent = 0; iEventEffective < nEvent; ++iEvent )
    {
        currentEvent = GenerateEvent( iEvent );
        eventManager->ProcessOneEvent( currentEvent );
        CexmcEventInfo *  eventInfo( static_cast< CexmcEventInfo * >(
                                        currentEvent->GetUserInformation() ) );
        switch ( eventCountPolicy )
        {
        case CexmcCountAllEvents :
            ++iEventEffective;
            break;
        case CexmcCountEventsWithInteraction :
            if ( eventInfo->TpTriggerIsOk() )
                ++iEventEffective;
            break;
        case CexmcCountEventsWithTrigger :
            if ( eventInfo->EdTriggerIsOk() )
                ++iEventEffective;
            break;
        default :
            ++iEventEffective;
            break;
        }
        AnalyzeEvent( currentEvent );
        UpdateScoring();
        if( iEvent < nSelect )
            G4UImanager::GetUIpointer()->ApplyCommand( msg );
        StackPreviousEvent( currentEvent );
        currentEvent = 0;
        if( runAborted )
            break;
    }

    if( verboseLevel > 0 )
    {
        timer->Stop();
        G4cout << "Run terminated." << G4endl;
        G4cout << "Run Summary" << G4endl;
        if( runAborted )
        {
            G4cout << "  Run Aborted after " << iEvent <<
                      " events processed." << G4endl;
        }
        else
        {
            G4cout << "  Number of events processed : " << iEvent <<
                      ", effectively: " << iEventEffective << G4endl;
        }
        G4cout << "  "  << *timer << G4endl;
    }
}

