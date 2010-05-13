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

#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include "CexmcReconstructorMessenger.hh"
#include "CexmcReconstructor.hh"
#include "CexmcMessenger.hh"
#include "CexmcCommon.hh"


CexmcReconstructorMessenger::CexmcReconstructorMessenger(
                                        CexmcReconstructor *  reconstructor ) :
    reconstructor( reconstructor ),
    setCalorimeterEntryPointDefinitionAlgorithm( NULL ),
    setCrystalSelectionAlgorithm( NULL ), useInnerMaxCrystal( NULL ),
    setCalorimeterEntryPointDepth( NULL )
{
    setCalorimeterEntryPointDefinitionAlgorithm = new G4UIcmdWithAString(
        ( CexmcMessenger::reconstructorDirName + "entryPointDefinitionAlgo" ).
                c_str(), this );
    setCalorimeterEntryPointDefinitionAlgorithm->SetGuidance(
        "\n    Algorithm to reconstruct entry point of output particle"
        "\n    decay products in calorimeter"
        "\n    (none of the following algorithms reconstruct directions)\n"
        "    center - entry points defined in the center of the\n"
        "             calorimeters,\n"
        "    simple - entry points defined in the center of the crystal\n"
        "             that has maximum energy deposit value,\n"
        "    linear - entry points defined by linear weights of energy\n"
        "             deposit in crystals,\n"
        "    sqrt - entry points defined by square root weights of\n"
        "           energy deposit in crystals" );
    setCalorimeterEntryPointDefinitionAlgorithm->SetDefaultValue( "sqrt" );
    setCalorimeterEntryPointDefinitionAlgorithm->SetParameterName(
                                            "EntryPointDefinitionAlgo", false );
    setCalorimeterEntryPointDefinitionAlgorithm->SetCandidates(
                                            "center simple linear sqrt" );
    setCalorimeterEntryPointDefinitionAlgorithm->AvailableForStates(
                                            G4State_PreInit, G4State_Idle );

    setCalorimeterEntryPointDepthDefinitionAlgorithm = new G4UIcmdWithAString(
        ( CexmcMessenger::reconstructorDirName +
                              "entryPointDepthDefinitionAlgo" ).c_str(), this );
    setCalorimeterEntryPointDepthDefinitionAlgorithm->SetGuidance(
        "\n    Algorithm to reconstruct entry point depth of output\n"
        "    particle decay products in calorimeter\n"
        "    (value is defined by 'entryPointDepth' parameter)\n"
        "    plain - depth is a constant\n"
        "    sphere - depth depends on X and Y of calorimeter entry\n"
        "             points and locates on surface of a sphere\n"
        "             with origin in the center of the target;\n"
        "             radius of the sphere is sum of distance to\n"
        "             the calorimeter and 'entryPointDepth' value" );
    setCalorimeterEntryPointDepthDefinitionAlgorithm->SetDefaultValue(
                                                                    "plain" );
    setCalorimeterEntryPointDepthDefinitionAlgorithm->SetParameterName(
                                    "EntryPointDepthDefinitionAlgo", false );
    setCalorimeterEntryPointDepthDefinitionAlgorithm->SetCandidates(
                                    "plain sphere" );
    setCalorimeterEntryPointDepthDefinitionAlgorithm->AvailableForStates(
                                                G4State_PreInit, G4State_Idle );

    setCrystalSelectionAlgorithm = new G4UIcmdWithAString(
        ( CexmcMessenger::reconstructorDirName + "crystalSelectionAlgo" ).
                c_str(), this );
    setCrystalSelectionAlgorithm->SetGuidance(
        "\n    Choose which crystals will be selected in weighted entry point\n"
        "    reconstruction algorithms\n"
        "    all - all,\n"
        "    adjacent - crystal with maximum energy deposit and\n"
        "               adjacent crystals" );
    setCrystalSelectionAlgorithm->SetDefaultValue( "all" );
    setCrystalSelectionAlgorithm->SetParameterName( "CrystalSelAlgo", false );
    setCrystalSelectionAlgorithm->SetCandidates( "all adjacent" );
    setCrystalSelectionAlgorithm->AvailableForStates( G4State_PreInit,
                                                      G4State_Idle );

    useInnerMaxCrystal = new G4UIcmdWithABool(
        ( CexmcMessenger::reconstructorDirName + "useInnerMaxCrystal" ).
                c_str(), this );
    useInnerMaxCrystal->SetGuidance(
        "\n    Defines that inner max crystal will be chosen for adjacent\n"
        "    crystal selection algorithm and simple entry point definition\n"
        "    algorithm. If not set then max crystal will be found from all\n"
        "    crystals in calorimeter" );
    useInnerMaxCrystal->SetDefaultValue( false );
    useInnerMaxCrystal->SetParameterName( "UseInnerMaxCrystal", false );
    useInnerMaxCrystal->AvailableForStates( G4State_PreInit, G4State_Idle );

    setCalorimeterEntryPointDepth = new G4UIcmdWithADoubleAndUnit(
        ( CexmcMessenger::reconstructorDirName + "entryPointDepth" ).c_str(),
        this );
    setCalorimeterEntryPointDepth->SetGuidance(
        "\n    Depth of entry point used in reconstruction of angle\n"
        "    between output particle decay products" );
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
    delete setCalorimeterEntryPointDepthDefinitionAlgorithm;
    delete setCrystalSelectionAlgorithm;
    delete useInnerMaxCrystal;
    delete setCalorimeterEntryPointDepth;
}


void  CexmcReconstructorMessenger::SetNewValue( G4UIcommand *  cmd,
                                                G4String  value )
{
    do
    {
        if ( cmd == setCalorimeterEntryPointDefinitionAlgorithm )
        {
            CexmcCalorimeterEntryPointDefinitionAlgorithm
                            epDefinitionAlgorithm( CexmcEntryPointInTheCenter );
            do
            {
                if ( value  == "simple" )
                {
                    epDefinitionAlgorithm =
                            CexmcEntryPointInTheCenterOfCrystalWithMaxED;
                    break;
                }
                if ( value  == "linear" )
                {
                    epDefinitionAlgorithm = CexmcEntryPointByLinearEDWeights;
                    break;
                }
                if ( value  == "sqrt" )
                {
                    epDefinitionAlgorithm = CexmcEntryPointBySqrtEDWeights;
                    break;
                }
            } while ( false );
            reconstructor->SetCalorimeterEntryPointDefinitionAlgorithm(
                                                        epDefinitionAlgorithm );
            break;
        }
        if ( cmd == setCalorimeterEntryPointDepthDefinitionAlgorithm )
        {
            CexmcCalorimeterEntryPointDepthDefinitionAlgorithm
                        epDepthDefinitionAlgorithm( CexmcEntryPointDepthPlain );
            do
            {
                if ( value  == "sphere" )
                {
                    epDepthDefinitionAlgorithm = CexmcEntryPointDepthSphere;
                    break;
                }
            } while ( false );
            reconstructor->SetCalorimeterEntryPointDepthDefinitionAlgorithm(
                                                epDepthDefinitionAlgorithm );
            break;
        }
        if ( cmd == setCrystalSelectionAlgorithm )
        {
            CexmcCrystalSelectionAlgorithm
                                        csAlgorithm( CexmcSelectAllCrystals );
            do
            {
                if ( value  == "adjacent" )
                {
                    csAlgorithm = CexmcSelectAdjacentCrystals;
                    break;
                }
            } while ( false );
            reconstructor->SetCrystalSelectionAlgorithm( csAlgorithm );
            break;
        }
        if ( cmd == useInnerMaxCrystal )
        {
            reconstructor->UseInnerMaxCrystal(
                        G4UIcmdWithABool::GetNewBoolValue( value ) );
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

