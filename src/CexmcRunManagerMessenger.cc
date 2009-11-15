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
    runManager( runManager ), setProductionModel( NULL ), setGdmlFile( NULL )
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
}


CexmcRunManagerMessenger::~CexmcRunManagerMessenger()
{
    delete setProductionModel;
    delete setGdmlFile;
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
    } while ( false );
}

