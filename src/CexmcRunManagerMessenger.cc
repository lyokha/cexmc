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
#include <G4UIcmdWithABool.hh>
#include "CexmcRunManager.hh"
#include "CexmcRunManagerMessenger.hh"
#include "CexmcMessenger.hh"


CexmcRunManagerMessenger::CexmcRunManagerMessenger(
                                CexmcRunManager *  runManager ) :
    runManager( runManager ), setProductionModel( NULL ), setGdmlFile( NULL ),
    saveResults( NULL ), setResultsDir( NULL ), setRunId( NULL )
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

    saveResults = new G4UIcmdWithABool(
        ( CexmcMessenger::runDirName + "saveResults" ).c_str(), this );
    saveResults->SetGuidance( "Whether to save results" );
    saveResults->SetParameterName( "SaveResults", false );
    saveResults->SetDefaultValue( true );
    saveResults->AvailableForStates( G4State_PreInit );

    setResultsDir = new G4UIcmdWithAString(
        ( CexmcMessenger::runDirName + "resultsDir" ).c_str(), this );
    setResultsDir->SetGuidance( "Directory where to save results" );
    setResultsDir->SetParameterName( "ResultsDir", false );
    setResultsDir->SetDefaultValue( "." );
    setResultsDir->AvailableForStates( G4State_PreInit );

    setRunId = new G4UIcmdWithAString(
        ( CexmcMessenger::runDirName + "runId" ).c_str(), this );
    setRunId->SetGuidance( "This is the base name for all results files" );
    setRunId->SetParameterName( "RunId", false );
    setRunId->SetDefaultValue( "trash" );
    setRunId->AvailableForStates( G4State_PreInit );
}


CexmcRunManagerMessenger::~CexmcRunManagerMessenger()
{
    delete setProductionModel;
    delete setGdmlFile;
    delete saveResults;
    delete setResultsDir;
    delete setRunId;
}


void  CexmcRunManagerMessenger::SetNewValue( G4UIcommand *  cmd,
                                             G4String  value )
{
    do
    {
        if ( cmd == setProductionModel )
        {
            runManager->SetProductionModelType( value );
            break;
        }
        if ( cmd == setGdmlFile )
        {
            runManager->SetGdmlFileName( value );
            break;
        }
        if ( cmd == saveResults )
        {
            runManager->SaveResults(
                                G4UIcmdWithABool::GetNewBoolValue( value ) );
            break;
        }
        if ( cmd == setResultsDir )
        {
            runManager->SetResultsDir( value );
            break;
        }
        if ( cmd == setRunId )
        {
            runManager->SetRunId( value );
            break;
        }
    } while ( false );
}

