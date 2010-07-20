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
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithABool.hh>
#include "CexmcRunManager.hh"
#include "CexmcRunManagerMessenger.hh"
#include "CexmcMessenger.hh"


CexmcRunManagerMessenger::CexmcRunManagerMessenger(
                                CexmcRunManager *  runManager ) :
    runManager( runManager ), setProductionModel( NULL ), setGdmlFile( NULL ),
    setGuiMacro( NULL ), setEventCountPolicy( NULL ), replayEvents( NULL ),
    seekTo( NULL ), skipInteractionsWithoutEDT( NULL )
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

    setGuiMacro = new G4UIcmdWithAString(
        ( CexmcMessenger::runDirName + "guiMacro" ).c_str(), this );
    setGuiMacro->SetGuidance( "Set Gui macro file" );
    setGuiMacro->SetParameterName( "GuiMacro", false );
    setGuiMacro->AvailableForStates( G4State_PreInit, G4State_Idle );

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

    replayEvents = new G4UIcmdWithAnInteger(
        ( CexmcMessenger::runDirName + "replay" ).c_str(), this );
    replayEvents->SetGuidance( "Replay specified number of events "
           "\n  (available only if a project is read)."
           "\n  If number of events is 0 (or not specified) then all"
           "\n  run will be replayed" );
    replayEvents->SetParameterName( "ReplayEvents", true );
    replayEvents->SetDefaultValue( 0 );
    replayEvents->SetRange( "ReplayEvents >= 0" );
    replayEvents->AvailableForStates( G4State_PreInit, G4State_Idle );

    seekTo = new G4UIcmdWithAnInteger(
        ( CexmcMessenger::runDirName + "seekto" ).c_str(), this );
    seekTo->SetGuidance( "Seek to specified event id "
           "(available only if a project is read).\n"
           "  'seekto 0' brings to the start of run, 'seekto 4' - to the first"
           "\n  recorded event with interaction after fourth recorded event "
           "\n  with trigger" );
    seekTo->SetParameterName( "SeekTo", false );
    seekTo->SetDefaultValue( 0 );
    seekTo->SetRange( "SeekTo >= 0" );
    seekTo->AvailableForStates( G4State_PreInit, G4State_Idle );

    skipInteractionsWithoutEDT = new G4UIcmdWithABool(
        ( CexmcMessenger::runDirName + "skipInteractionsWithoutEDT" ).c_str(),
        this );
    skipInteractionsWithoutEDT->SetGuidance( "effective only when a project is "
        "read and then written to\nanother project. Do not write interactions "
        "into .fdb file if\nevent was not triggered" );
    skipInteractionsWithoutEDT->SetParameterName( "skipInteractionsWithoutEDT",
                                                  true );
    skipInteractionsWithoutEDT->SetDefaultValue( true );
    skipInteractionsWithoutEDT->AvailableForStates( G4State_PreInit,
                                                    G4State_Idle );
}


CexmcRunManagerMessenger::~CexmcRunManagerMessenger()
{
    delete setProductionModel;
    delete setGdmlFile;
    delete setGuiMacro;
    delete setEventCountPolicy;
    delete replayEvents;
    delete seekTo;
    delete skipInteractionsWithoutEDT;
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
        if ( cmd == setGuiMacro )
        {
            runManager->SetGuiMacroName( value );
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
        if ( cmd == replayEvents )
        {
            runManager->ReplayEvents(
                                G4UIcmdWithAnInteger::GetNewIntValue( value ) );
            break;
        }
        if ( cmd == seekTo )
        {
            runManager->SeekTo( G4UIcmdWithAnInteger::GetNewIntValue( value ) );
            break;
        }
        if ( cmd == skipInteractionsWithoutEDT )
        {
            runManager->SkipInteractionsWithoutEDTonWrite(
                                G4UIcmdWithABool::GetNewBoolValue( value ) );
            break;
        }
    } while ( false );
}

