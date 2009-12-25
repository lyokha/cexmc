/*
 * ============================================================================
 *
 *       Filename:  CexmcRunManagerMessenger.cc
 *
 *    Description:  init parameters (production model, gdml file etc.)
 *
 *        Version:  1.0
 *        Created:  03.11.2009 20:45:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4UIcmdWithAString.hh>
#include "CexmcRunManager.hh"
#include "CexmcRunManagerMessenger.hh"
#include "CexmcMessenger.hh"


CexmcRunManagerMessenger::CexmcRunManagerMessenger(
                                CexmcRunManager *  runManager ) :
    runManager( runManager ), setProductionModel( NULL ), setGdmlFile( NULL ),
    setEventCountPolicy( NULL )
{
    setProductionModel = new G4UIcmdWithAString(
        ( CexmcMessenger::physicsDirName + "productionModel" ).c_str(), this );
    setProductionModel->SetGuidance( "Set production model (e.g. pi0 or eta)" );
    setProductionModel->SetParameterName( "ProductionModel", false );
    setProductionModel->SetCandidates( "pi0 eta" );
    setProductionModel->AvailableForStates( G4State_PreInit );

    setGdmlFile = new G4UIcmdWithAString(
        ( CexmcMessenger::geometryDirName + "gdmlFile" ).c_str(), this );
    setGdmlFile->SetGuidance( "GDML file to read geometry from" );
    setGdmlFile->SetParameterName( "GdmlFile", false );
    setGdmlFile->AvailableForStates( G4State_PreInit );

    setEventCountPolicy = new G4UIcmdWithAString(
        ( CexmcMessenger::runDirName + "eventCountPolicy" ).c_str(), this );
    setEventCountPolicy->SetGuidance( "How number of events is interpreted.\n"
            "  all - all events are accounted,\n"
            "  interaction - events with studied interaction triggered,\n"
            "  trigger - only events with trigger" );
    setEventCountPolicy->SetParameterName( "EventCountPolicy", false );
    setEventCountPolicy->SetDefaultValue( "all" );
    setEventCountPolicy->SetCandidates( "all interaction trigger" );
    setEventCountPolicy->AvailableForStates( G4State_PreInit, G4State_Idle );
}


CexmcRunManagerMessenger::~CexmcRunManagerMessenger()
{
    delete setProductionModel;
    delete setGdmlFile;
    delete setEventCountPolicy;
}


void  CexmcRunManagerMessenger::SetNewValue( G4UIcommand *  cmd,
                                             G4String  value )
{
    do
    {
        if ( cmd == setProductionModel )
        {
            CexmcProductionModelType  productionModelType(
                                                CexmcUnknownProductionModel );
            do
            {
                if ( value == "pi0" )
                {
                    productionModelType = CexmcPionZeroProduction;
                    break;
                }
                if ( value == "eta" )
                {
                    productionModelType = CexmcEtaProduction;
                    break;
                }
            } while ( false );
            runManager->SetProductionModelType( productionModelType );
            break;
        }
        if ( cmd == setGdmlFile )
        {
            runManager->SetGdmlFileName( value );
            break;
        }
        if ( cmd == setEventCountPolicy )
        {
            CexmcEventCountPolicy  eventCountPolicy( CexmcCountAllEvents );
            do
            {
                if ( value == "interaction" )
                {
                    eventCountPolicy = CexmcCountEventsWithInteraction;
                    break;
                }
                if ( value == "trigger" )
                {
                    eventCountPolicy = CexmcCountEventsWithTrigger;
                    break;
                }
            } while ( false );
            runManager->SetEventCountPolicy( eventCountPolicy );
            break;
        }
    } while ( false );
}

