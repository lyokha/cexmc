/*
 * ============================================================================
 *
 *       Filename:  CexmcReconstructorMessenger.cc
 *
 *    Description:  reconstructor messenger
 *
 *        Version:  1.0
 *        Created:  02.12.2009 15:38:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4UIcmdWithAnInteger.hh>
#include "CexmcReconstructorMessenger.hh"
#include "CexmcReconstructor.hh"
#include "CexmcMessenger.hh"


CexmcReconstructorMessenger::CexmcReconstructorMessenger(
                                        CexmcReconstructor *  reconstructor ) :
    reconstructor( reconstructor ),
    setCalorimeterEntryPointDefinitionAlgorithm( NULL )
{
    setCalorimeterEntryPointDefinitionAlgorithm = new G4UIcmdWithAnInteger(
        ( CexmcMessenger::reconstructorDirName + "entryPointDefAlgo" ).c_str(),
        this );
    setCalorimeterEntryPointDefinitionAlgorithm->SetGuidance(
        "\n    Algorithm to reconstruct entry point of output particle decay "
        "products\n    in calorimeter"
        " (none of the following algorithms reconstruct directions)\n"
        "    0 - entry points defined in the center of the calorimeters,\n"
        "    1 - entry points defined by linear weights of energy deposit in "
        "crystals,\n"
        "    2 - entry points defined by square root weights ofenergy deposit "
        "in crystals" );
    setCalorimeterEntryPointDefinitionAlgorithm->SetDefaultValue( 2 );
    setCalorimeterEntryPointDefinitionAlgorithm->SetParameterName(
                                                "EntryPointDefAlgo", false );
    setCalorimeterEntryPointDefinitionAlgorithm->SetRange(
                        "EntryPointDefAlgo >= 0 && EntryPointDefAlgo <= 2" );
    setCalorimeterEntryPointDefinitionAlgorithm->AvailableForStates(
                                                G4State_PreInit, G4State_Idle );

    setCrystalSelectionAlgorithm = new G4UIcmdWithAnInteger(
        ( CexmcMessenger::reconstructorDirName + "crystalSelectionAlgo" ).
                c_str(), this );
    setCrystalSelectionAlgorithm->SetGuidance(
        "\n    Choose which crystals will be selected in entry point "
        "reconstruction algorithm\n"
        "    0 - all,\n"
        "    1 - crystal with maximum energy deposit and adjacent crystals" );
    setCrystalSelectionAlgorithm->SetDefaultValue( 0 );
    setCrystalSelectionAlgorithm->SetParameterName( "CrystalSelAlgo", false );
    setCrystalSelectionAlgorithm->SetRange(
                        "CrystalSelAlgo >= 0 && CrystalSelAlgo <= 1" );
    setCrystalSelectionAlgorithm->AvailableForStates( G4State_PreInit,
                                                      G4State_Idle );
}


CexmcReconstructorMessenger::~CexmcReconstructorMessenger()
{
    delete setCalorimeterEntryPointDefinitionAlgorithm;
    delete setCrystalSelectionAlgorithm;
}


void  CexmcReconstructorMessenger::SetNewValue( G4UIcommand *  cmd,
                                                G4String  value )
{
    do
    {
        if ( cmd == setCalorimeterEntryPointDefinitionAlgorithm )
        {
            reconstructor->SetCalorimeterEntryPointDefinitionAlgorithm(
                                G4UIcmdWithAnInteger::GetNewIntValue( value ) );
            break;
        }
        if ( cmd == setCrystalSelectionAlgorithm )
        {
            reconstructor->SetCrystalSelectionAlgorithm(
                                G4UIcmdWithAnInteger::GetNewIntValue( value ) );
            break;
        }
    } while ( false );
}
