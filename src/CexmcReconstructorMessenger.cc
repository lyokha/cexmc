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

#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include "CexmcReconstructorMessenger.hh"
#include "CexmcReconstructor.hh"
#include "CexmcMessenger.hh"


CexmcReconstructorMessenger::CexmcReconstructorMessenger(
                                        CexmcReconstructor *  reconstructor ) :
    reconstructor( reconstructor ),
    setCalorimeterEntryPointDefinitionAlgorithm( NULL ),
    setCrystalSelectionAlgorithm( NULL ), setCalorimeterEntryPointDepth( NULL )
{
    setCalorimeterEntryPointDefinitionAlgorithm = new G4UIcmdWithAString(
        ( CexmcMessenger::reconstructorDirName + "entryPointDefinitionAlgo" ).
                c_str(), this );
    setCalorimeterEntryPointDefinitionAlgorithm->SetGuidance(
        "\n    Algorithm to reconstruct entry point of output particle decay "
        "products\n    in calorimeter"
        " (none of the following algorithms reconstruct directions)\n"
        "    center - entry points defined in the center of the calorimeters,\n"
        "    simple - entry points defined in the center of the crystal that \n"
        "             has maximum energy deposit value,\n"
        "    linear - entry points defined by linear weights of energy deposit "
                     "in crystals,\n"
        "    sqrt - entry points defined by square root weights of energy "
                     "deposit in crystals" );
    setCalorimeterEntryPointDefinitionAlgorithm->SetDefaultValue( "sqrt" );
    setCalorimeterEntryPointDefinitionAlgorithm->SetParameterName(
                                            "EntryPointDefinitionAlgo", false );
    setCalorimeterEntryPointDefinitionAlgorithm->SetCandidates(
                                            "center simple linear sqrt" );
    setCalorimeterEntryPointDefinitionAlgorithm->AvailableForStates(
                                            G4State_PreInit, G4State_Idle );

    setCrystalSelectionAlgorithm = new G4UIcmdWithAString(
        ( CexmcMessenger::reconstructorDirName + "crystalSelectionAlgo" ).
                c_str(), this );
    setCrystalSelectionAlgorithm->SetGuidance(
        "\n    Choose which crystals will be selected in entry point "
              "reconstruction algorithm\n"
        "    all - all,\n"
        "    adjacent - crystal with maximum energy deposit and adjacent "
            "crystals" );
    setCrystalSelectionAlgorithm->SetDefaultValue( "all" );
    setCrystalSelectionAlgorithm->SetParameterName( "CrystalSelAlgo", false );
    setCrystalSelectionAlgorithm->SetCandidates( "all adjacent" );
    setCrystalSelectionAlgorithm->AvailableForStates( G4State_PreInit,
                                                      G4State_Idle );

    setCalorimeterEntryPointDepth = new G4UIcmdWithADoubleAndUnit(
        ( CexmcMessenger::reconstructorDirName + "entryPointDepth" ).c_str(),
        this );
    setCalorimeterEntryPointDepth->SetGuidance(
        "\n    Depth of entry point used in reconstruction of angle between "
        "output particle decay products" );
    setCalorimeterEntryPointDepth->SetParameterName( "EntryPointDepth", false );
    setCalorimeterEntryPointDepth->SetDefaultValue( 0 );
    setCalorimeterEntryPointDepth->SetDefaultUnit( "cm" );
    setCalorimeterEntryPointDepth->SetUnitCandidates( "mm cm m" );
    setCalorimeterEntryPointDepth->AvailableForStates( G4State_PreInit,
                                                       G4State_Idle );
}


CexmcReconstructorMessenger::~CexmcReconstructorMessenger()
{
    delete setCalorimeterEntryPointDefinitionAlgorithm;
    delete setCrystalSelectionAlgorithm;
    delete setCalorimeterEntryPointDepth;
}


void  CexmcReconstructorMessenger::SetNewValue( G4UIcommand *  cmd,
                                                G4String  value )
{
    do
    {
        if ( cmd == setCalorimeterEntryPointDefinitionAlgorithm )
        {
            reconstructor->SetCalorimeterEntryPointDefinitionAlgorithm( value );
            break;
        }
        if ( cmd == setCrystalSelectionAlgorithm )
        {
            reconstructor->SetCrystalSelectionAlgorithm( value );
            break;
        }
        if ( cmd == setCalorimeterEntryPointDepth )
        {
            reconstructor->SetCalorimeterEntryPointDepth(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
            break;
        }
    } while ( false );
}

