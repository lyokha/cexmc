/*
 * =============================================================================
 *
 *       Filename:  cexmc.cc
 *
 *    Description:  main
 *
 *        Version:  1.0
 *        Created:  10.10.2009 23:24:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#include <boost/archive/archive_exception.hpp>
#include <G4UImanager.hh>
#include <G4UIsession.hh>
#include <G4UIterminal.hh>
#include <G4UItcsh.hh>
#include <G4VisExecutive.hh>
#include "CexmcRunManager.hh"
#include "CexmcHistoManager.hh"
#include "CexmcSetup.hh"
#include "CexmcPhysicsList.hh"
#include "CexmcPhysicsManager.hh"
#include "CexmcPrimaryGeneratorAction.hh"
#include "CexmcTrackingAction.hh"
#include "CexmcSteppingAction.hh"
#include "CexmcEventAction.hh"
#include "CexmcRunAction.hh"
#include "CexmcMessenger.hh"
#include "CexmcException.hh"
#include "CexmcBasicPhysicsSettings.hh"
#include "CexmcCommon.hh"


struct  CexmcCmdLineData
{
    CexmcCmdLineData() : isInteractive( false ), preinitMacro( "" ),
                         initMacro( "" ), rProject( "" ), wProject( "" )
    {}

    G4bool    isInteractive;
    G4String  preinitMacro;
    G4String  initMacro;
    G4String  rProject;
    G4String  wProject;
};


void  printUsage( void )
{
    G4cout << "Usage: cexmc [-i] [-p preinit_macro] [-m init_macro] "
                           "[-w project] [-r project]" << G4endl;
    G4cout << "or     cexmc [--help | -h]" << G4endl;
    G4cout << "           -i - run in interactive mode" << G4endl;
    G4cout << "           -p - use specified preinit macro file "
                     "(default is 'preinit.mac')" << G4endl;
    G4cout << "           -m - use specified init macro file "
                     "(default is 'init.mac')" << G4endl;
    G4cout << "           -w - save data in specified project files " << G4endl;
    G4cout << "           -r - read data from specified project files " <<
              G4endl;
    G4cout << "  --help | -h - print this message and exit " << G4endl;
}


G4bool  parseArgs( int  argc, char ** argv, CexmcCmdLineData &  cmdLineData )
{
    for ( G4int  i( 1 ); i < argc; ++i )
    {
        do
        {
            if ( G4String( argv[ i ] ) == "--help" )
            {
                return false;
            }
            if ( G4String( argv[ i ] ) == "-h" )
            {
                return false;
            }
            if ( G4String( argv[ i ], 2 ) == "-i" )
            {
                cmdLineData.isInteractive = true;
                break;
            }
            if ( G4String( argv[ i ], 2 ) == "-p" )
            {
                cmdLineData.preinitMacro = argv[ i ] + 2;
                if ( cmdLineData.preinitMacro == "" )
                {
                    if ( ++i >= argc )
                        throw CexmcException( CexmcCmdLineParseException );
                    cmdLineData.preinitMacro = argv[ i ];
                }
                break;
            }
            if ( G4String( argv[ i ], 2 ) == "-m" )
            {
                cmdLineData.initMacro = argv[ i ] + 2;
                if ( cmdLineData.initMacro == "" )
                {
                    if ( ++i >= argc )
                        throw CexmcException( CexmcCmdLineParseException );
                    cmdLineData.initMacro = argv[ i ];
                }
                break;
            }
            if ( G4String( argv[ i ], 2 ) == "-w" )
            {
                cmdLineData.wProject = argv[ i ] + 2;
                if ( cmdLineData.wProject == "" )
                {
                    if ( ++i >= argc )
                        throw CexmcException( CexmcCmdLineParseException );
                    cmdLineData.wProject = argv[ i ];
                }
                break;
            }
            if ( G4String( argv[ i ], 2 ) == "-r" )
            {
                cmdLineData.rProject = argv[ i ] + 2;
                if ( cmdLineData.rProject == "" )
                {
                    if ( ++i >= argc )
                        throw CexmcException( CexmcCmdLineParseException );
                    cmdLineData.rProject = argv[ i ];
                }
                break;
            }
        } while ( false );
    }

    return true;
}


int  main( int  argc, char **  argv )
{
    G4UIsession *  session( NULL );

    CexmcCmdLineData  cmdLineData;

    try
    {
        if ( ! parseArgs( argc, argv, cmdLineData ) )
        {
            printUsage();
            return 0;
        }
        if ( cmdLineData.rProject != "" &&
             cmdLineData.rProject == cmdLineData.wProject )
            throw CexmcException( CexmcCmdLineParseException );
    }
    catch ( CexmcException &  e )
    {
        G4cout << e.what() << G4endl;
        return 1;
    }
    catch ( ... )
    {
        G4cout << "Unknown exception caught when parsing args" << G4endl;
        return 1;
    }

    if ( cmdLineData.isInteractive )
    {
        session = new G4UIterminal( new G4UItcsh );
    }

    CexmcRunManager *  runManager( NULL );
    G4VisManager *     visManager( NULL );

    CexmcMessenger::Instance();
    CexmcHistoManager::Instance();

    try
    {
        runManager = new CexmcRunManager( cmdLineData.wProject,
                                          cmdLineData.rProject );

        G4UImanager *  uiManager( G4UImanager::GetUIpointer() );

        if ( cmdLineData.preinitMacro != "" )
            uiManager->ApplyCommand( "/control/execute " +
                                     cmdLineData.preinitMacro );

        CexmcSetup *   setup( new CexmcSetup( runManager->GetGdmlFileName() ) );

        runManager->SetUserInitialization( setup );

        CexmcProductionModelType  productionModelType(
                                        runManager->GetProductionModelType() );

        if ( productionModelType == CexmcUnknownProductionModel )
            throw CexmcException( CexmcPreinitException );

        G4VUserPhysicsList *  physicsList( CexmcChargeExchangePMFactory::
                                                Create( productionModelType ) );

        runManager->SetUserInitialization( physicsList );

        runManager->Initialize();

        runManager->SetUserAction( new CexmcPrimaryGeneratorAction );

        CexmcPhysicsManager *  physicsManager(
                        dynamic_cast< CexmcPhysicsManager * >( physicsList ) );

        runManager->SetPhysicsManager( physicsManager );

        runManager->SetUserAction( new CexmcEventAction( physicsManager ) );

        runManager->SetUserAction( new CexmcRunAction( physicsManager ) );

        runManager->SetUserAction( new CexmcTrackingAction );

        runManager->SetUserAction( new CexmcSteppingAction( physicsManager ) );

        CexmcHistoManager::Initialize();

        visManager = new G4VisExecutive;
        visManager->Initialize();

        if ( runManager->ProjectIsRead() )
        {
            runManager->ReadProject();
            runManager->PrintReadData();
        }

        if ( cmdLineData.initMacro != "" )
            uiManager->ApplyCommand( "/control/execute " +
                                     cmdLineData.initMacro );

        CexmcProductionModel *  productionModel(
                                        physicsManager->GetProductionModel() );

        if ( ! productionModel )
            throw CexmcException( CexmcWeirdException );

        productionModel->PrintInitialData();

        if ( session )
        {
            session->SessionStart();
        }

        if ( runManager->ProjectIsSaved() )
        {
            runManager->SaveProject();
        }
    }
    catch ( CexmcException &  e )
    {
        G4cout << e.what() << G4endl;
    }
    catch ( boost::archive::archive_exception &  e )
    {
        G4cout << e.what() << G4endl;
    }
    catch ( ... )
    {
        G4cout << "Unknown exception caught" << G4endl;
    }

    CexmcHistoManager::Destroy();
    CexmcMessenger::Destroy();

    delete visManager;
    delete runManager;
    delete session;

    return 0;
}

