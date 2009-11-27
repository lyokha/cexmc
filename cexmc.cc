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

#include <G4UImanager.hh>
#include <G4UIsession.hh>
#include <G4UIterminal.hh>
#include <G4UItcsh.hh>
#include <G4VisExecutive.hh>
#include <QGSP_BERT.hh>
#include <G4PionMinus.hh>
#include "CexmcRunManager.hh"
#include "CexmcHistoManager.hh"
#include "CexmcSetup.hh"
#include "CexmcPhysicsList.hh"
#include "CexmcPhysicsManager.hh"
#include "CexmcPrimaryGeneratorAction.hh"
#include "CexmcTrackingAction.hh"
#include "CexmcSteppingAction.hh"
#include "CexmcEventAction.hh"
#include "CexmcStudiedPhysicsChargeExchange.hh"
#include "CexmcProductionModelFactory.hh"
#include "CexmcMessenger.hh"
#include "CexmcException.hh"
#include "CexmcCommon.hh"


typedef QGSP_BERT    CexmcBasePhysics;
typedef G4PionMinus  CexmcStudiedPhysicsStarterParticle;
typedef CexmcProductionModelFactory< CexmcBasePhysics,
                                     CexmcStudiedPhysicsStarterParticle,
                                     CexmcStudiedPhysicsChargeExchange >
                     CexmcChargeExchangePMFactory;


namespace
{
    G4String  preinitMacroFile( "preinit.mac" );
    G4String  initMacroFile( "init.mac" );
}


void printUsage( void )
{
    G4cout << "Usage: cexmc [init_macro_file] [preinit_macro_file]" << G4endl;
}


int  main( int  argc, char **  argv )
{
    G4UIsession *  session( NULL );

    if ( argc == 1 )
    {
        session = new G4UIterminal( new G4UItcsh );
    }
    else
    {
        initMacroFile = argv[ 1 ];
        if ( argc > 2 )
            preinitMacroFile = argv[ 2 ];
    }

    CexmcRunManager *  runManager( NULL );
    G4VisManager *     visManager( NULL );

    CexmcMessenger::Instance();
    CexmcHistoManager::Instance();

    try
    {
        runManager = new CexmcRunManager;

        G4UImanager *  uiManager( G4UImanager::GetUIpointer() );

        uiManager->ApplyCommand( "/control/execute " + preinitMacroFile );

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

        runManager->SetUserAction( new CexmcEventAction( physicsManager ) );

        runManager->SetUserAction( new CexmcTrackingAction(
                        CexmcChargeExchangePMFactory::GetOutputParticle(
                                                    productionModelType),
                        CexmcChargeExchangePMFactory::GetNucleusParticle(
                                                    productionModelType) ) );

        runManager->SetUserAction( new CexmcSteppingAction( physicsManager ) );

        CexmcHistoManager::Initialize();

        visManager = new G4VisExecutive;
        visManager->Initialize();

        uiManager->ApplyCommand( "/control/execute " + initMacroFile );

        if ( session )
        {
            session->SessionStart();
        }

        CexmcHistoManager::Destroy();
        CexmcMessenger::Destroy();
    }
    catch ( CexmcException &  e )
    {
        G4cout << e.what() << G4endl;
    }
    catch ( ... )
    {
        G4cout << "Unknown exception caught" << G4endl;
    }

    delete visManager;
    delete runManager;
    delete session;

    return 0;
}

