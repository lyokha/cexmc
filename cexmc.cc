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

#include <set>
#include <boost/algorithm/string.hpp>
#include <boost/archive/archive_exception.hpp>
#include <G4UImanager.hh>
#include <G4UIsession.hh>
#include <G4UIterminal.hh>
#include <G4UItcsh.hh>
#include <G4UIQt.hh>
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
    CexmcCmdLineData() : isInteractive( false ), startQtSession( false ),
                         preinitMacro( "" ), initMacro( "" ), rProject( "" ),
                         wProject( "" ), overrideExistingProject( false )
    {}

    G4bool    isInteractive;
    G4bool    startQtSession;
    G4String  preinitMacro;
    G4String  initMacro;
    G4String  rProject;
    G4String  wProject;
    G4bool    overrideExistingProject;
    CexmcOutputDataTypeSet  outputData;
};


void  printUsage( void )
{
    G4cout << "Usage: cexmc [-i] [-p preinit_macro] [-m init_macro] "
                           "[[-y] -w project] [-r project [-o list]]" << G4endl;
    G4cout << "or     cexmc [--help | -h]" << G4endl;
    G4cout << "           -i - run in interactive mode" << G4endl;
    G4cout << "           -g - start graphical interface (Qt), implies "
                              "interactive mode " << G4endl;
    G4cout << "           -p - use specified preinit macro file " << G4endl;
    G4cout << "           -m - use specified init macro file " << G4endl;
    G4cout << "           -w - save data in specified project files" << G4endl;
    G4cout << "           -r - read data from specified project files" <<
              G4endl;
    G4cout << "           -o - comma-separated list of data to output, "
                              "possible values: run, geom, events" << G4endl;
    G4cout << "           -y - force project override" << G4endl;
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
            if ( G4String( argv[ i ], 2 ) == "-g" )
            {
                cmdLineData.isInteractive = true;
                cmdLineData.startQtSession = true;
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
            if ( G4String( argv[ i ], 2 ) == "-y" )
            {
                cmdLineData.overrideExistingProject = true;
                break;
            }
            if ( G4String( argv[ i ], 2 ) == "-o" )
            {
                std::string  outputData( argv[ i ] + 2 );
                if ( outputData == "" )
                {
                    if ( ++i >= argc )
                        throw CexmcException( CexmcCmdLineParseException );
                    outputData = argv[ i ];
                }
                std::set< std::string >  tokens;
                boost::split( tokens, outputData, boost::is_any_of( "," ) );
                for ( std::set< std::string >::iterator  k( tokens.begin() );
                                                        k != tokens.end(); ++k )
                {
                    do
                    {
                        if ( *k == "run" )
                        {
                            cmdLineData.outputData.insert( CexmcOutputRun );
                            break;
                        }
                        if ( *k == "geom" )
                        {
                            cmdLineData.outputData.insert(
                                                          CexmcOutputGeometry );
                            break;
                        }
                        if ( *k == "events" )
                        {
                            cmdLineData.outputData.insert( CexmcOutputEvents );
                            break;
                        }
                        throw CexmcException( CexmcCmdLineParseException );
                    } while ( false );
                }
                break;
            }

            throw CexmcException( CexmcCmdLineParseException );

        } while ( false );
    }

    return true;
}


int  main( int  argc, char **  argv )
{
    G4UIsession *  session( NULL );

    CexmcCmdLineData  cmdLineData;
    G4bool            outputDataOnly( false );

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
        if ( cmdLineData.rProject == "" && ! cmdLineData.outputData.empty() )
            throw CexmcException( CexmcCmdLineParseException );
        if ( cmdLineData.wProject != "" && ! cmdLineData.outputData.empty() )
            throw CexmcException( CexmcCmdLineParseException );
        outputDataOnly = ! cmdLineData.outputData.empty();
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

    CexmcRunManager *  runManager( NULL );
    G4VisManager *     visManager( NULL );

    CexmcMessenger::Instance();
    CexmcHistoManager::Instance();

    try
    {
        runManager = new CexmcRunManager( cmdLineData.wProject,
                                          cmdLineData.rProject,
                                          cmdLineData.overrideExistingProject );

        if ( outputDataOnly )
        {
            /* we will need an arbitrary physics list to get access to particle
             * table if events output was ordered */
            CexmcOutputDataTypeSet::const_iterator  found(
                            cmdLineData.outputData.find( CexmcOutputEvents ) );
            if ( found != cmdLineData.outputData.end() )
                runManager->SetUserInitialization(
                                CexmcChargeExchangePMFactory::
                                            Create( CexmcPionZeroProduction ) );
            runManager->PrintReadData( cmdLineData.outputData );
            delete runManager;
            return 0;
        }

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

        CexmcHistoManager::Instance()->Initialize();

        if ( cmdLineData.isInteractive )
        {
            visManager = new G4VisExecutive;
            visManager->Initialize();
        }

        if ( runManager->ProjectIsRead() )
        {
            runManager->ReadProject();
            runManager->PrintReadRunData();
        }

        if ( cmdLineData.initMacro != "" )
            uiManager->ApplyCommand( "/control/execute " +
                                     cmdLineData.initMacro );

        CexmcProductionModel *  productionModel(
                                        physicsManager->GetProductionModel() );

        if ( ! productionModel )
            throw CexmcException( CexmcWeirdException );


        if ( cmdLineData.isInteractive )
        {
            productionModel->PrintInitialData();
        }

        if ( cmdLineData.isInteractive )
        {
            if ( cmdLineData.startQtSession )
            {
                session = new G4UIQt( argc, argv );
                const G4String &  guiMacroName( runManager->GetGuiMacroName() );
                if ( guiMacroName != "" )
                    uiManager->ApplyCommand( "/control/execute " +
                                             guiMacroName );
                runManager->EnableLiveHistograms();
            }
            else
            {
                session = new G4UIterminal( new G4UItcsh );
            }
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

