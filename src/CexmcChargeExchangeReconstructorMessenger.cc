/*
 * ============================================================================
 *
 *       Filename:  CexmcChargeExchangeReconstructorMessenger.cc
 *
 *    Description:  charge exchange reconstructor messenger
 *
 *        Version:  1.0
 *        Created:  14.12.2009 17:53:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include "CexmcChargeExchangeReconstructorMessenger.hh"
#include "CexmcChargeExchangeReconstructor.hh"
#include "CexmcMessenger.hh"


CexmcChargeExchangeReconstructorMessenger::
        CexmcChargeExchangeReconstructorMessenger(
                        CexmcChargeExchangeReconstructor *  reconstructor ) :
            reconstructor( reconstructor ), useTableMass( NULL ),
            useMassCut( NULL ), mCutOPCenter( NULL ), mCutNOPCenter( NULL ),
            mCutOPWidth( NULL ), mCutNOPWidth( NULL ), mCutAngle( NULL ),
            useAbsorbedEnergyCut( NULL ), aeCutCLCenter( NULL ),
            aeCutCRCenter( NULL ), aeCutCLWidth( NULL ), aeCutCRWidth( NULL ),
            aeCutAngle( NULL )
{
    useTableMass = new G4UIcmdWithABool(
        ( CexmcMessenger::reconstructorDirName + "useTableMass" ).c_str(),
        this );
    useTableMass->SetGuidance( "\n    If true then reconstructor will use "
        "table mass of output particle\n    when building output particle "
        "energy,\n    otherwise reconstructed mass will be used" );
    useTableMass->SetParameterName( "UseTableMass", false );
    useTableMass->SetDefaultValue( false );
    useTableMass->AvailableForStates( G4State_PreInit, G4State_Idle );

    useMassCut = new G4UIcmdWithABool(
        ( CexmcMessenger::reconstructorDirName + "useMassCut" ).c_str(), this );
    useMassCut->SetGuidance( "\n    Use elliptical cut for masses of output "
                             "particle\n    and nucleus output particle" );
    useMassCut->SetParameterName( "UseMassCut", false );
    useMassCut->SetDefaultValue( false );
    useMassCut->AvailableForStates( G4State_PreInit, G4State_Idle );

    mCutOPCenter = new G4UIcmdWithADoubleAndUnit(
        ( CexmcMessenger::reconstructorDirName + "mCutOPCenter" ).c_str(),
        this );
    mCutOPCenter->SetGuidance( "Center of the ellipse in output particle mass "
                               "coordinate" );
    mCutOPCenter->SetParameterName( "MCutOPCenter", false );
    mCutOPCenter->SetDefaultValue( reconstructor->GetProductionModelData().
                                   outputParticle->GetPDGMass() );
    mCutOPCenter->SetDefaultUnit( "MeV" );
    mCutOPCenter->SetUnitCandidates( "eV keV MeV GeV" );
    mCutOPCenter->AvailableForStates( G4State_PreInit, G4State_Idle );

    mCutNOPCenter = new G4UIcmdWithADoubleAndUnit(
        ( CexmcMessenger::reconstructorDirName + "mCutNOPCenter" ).c_str(),
        this );
    mCutNOPCenter->SetGuidance( "Center of the ellipse in nucleus output "
                                "particle mass coordinate" );
    mCutNOPCenter->SetParameterName( "MCutNOPCenter", false );
    mCutNOPCenter->SetDefaultValue( reconstructor->GetProductionModelData().
                                    nucleusOutputParticle->GetPDGMass() );
    mCutNOPCenter->SetDefaultUnit( "MeV" );
    mCutNOPCenter->SetUnitCandidates( "eV keV MeV GeV" );
    mCutNOPCenter->AvailableForStates( G4State_PreInit, G4State_Idle );

    mCutOPWidth = new G4UIcmdWithADoubleAndUnit(
        ( CexmcMessenger::reconstructorDirName + "mCutOPWidth" ).c_str(),
        this );
    mCutOPWidth->SetGuidance( "Width of the ellipse in output particle mass "
                               "coordinate" );
    mCutOPWidth->SetParameterName( "MCutOPWidth", false );
    mCutOPWidth->SetDefaultValue( reconstructor->GetProductionModelData().
                                  outputParticle->GetPDGMass() * 0.1 );
    mCutOPWidth->SetDefaultUnit( "MeV" );
    mCutOPWidth->SetUnitCandidates( "eV keV MeV GeV" );
    mCutOPWidth->AvailableForStates( G4State_PreInit, G4State_Idle );

    mCutNOPWidth = new G4UIcmdWithADoubleAndUnit(
        ( CexmcMessenger::reconstructorDirName + "mCutNOPWidth" ).c_str(),
        this );
    mCutNOPWidth->SetGuidance( "Width of the ellipse in nucleus output "
                               "particle mass coordinate" );
    mCutNOPWidth->SetParameterName( "MCutNOPWidth", false );
    mCutNOPWidth->SetDefaultValue( reconstructor->GetProductionModelData().
                                   nucleusOutputParticle->GetPDGMass() * 0.1 );
    mCutNOPWidth->SetDefaultUnit( "MeV" );
    mCutNOPWidth->SetUnitCandidates( "eV keV MeV GeV" );
    mCutNOPWidth->AvailableForStates( G4State_PreInit, G4State_Idle );

    mCutAngle = new G4UIcmdWithADoubleAndUnit(
        ( CexmcMessenger::reconstructorDirName + "mCutAngle" ).c_str(),
        this );
    mCutAngle->SetGuidance( "Angle of the ellipse" );
    mCutAngle->SetParameterName( "MCutAngle", false );
    mCutAngle->SetDefaultValue( 0 );
    mCutAngle->SetDefaultUnit( "deg" );
    mCutAngle->SetUnitCandidates( "deg rad" );
    mCutAngle->AvailableForStates( G4State_PreInit, G4State_Idle );

    useAbsorbedEnergyCut = new G4UIcmdWithABool(
        ( CexmcMessenger::reconstructorDirName + "useAbsorbedEnergyCut" ).
            c_str(), this );
    useAbsorbedEnergyCut->SetGuidance( "\n    Use elliptical cut for absorbed "
                                       "energies in calorimeters" );
    useAbsorbedEnergyCut->SetParameterName( "UseAbsorbedEnergyCut", false );
    useAbsorbedEnergyCut->SetDefaultValue( false );
    useAbsorbedEnergyCut->AvailableForStates( G4State_PreInit, G4State_Idle );

    aeCutCLCenter = new G4UIcmdWithADoubleAndUnit(
        ( CexmcMessenger::reconstructorDirName + "aeCutCLCenter" ).c_str(),
        this );
    aeCutCLCenter->SetGuidance( "Center of the ellipse in left calorimeter "
                                "absorbed energy coordinate" );
    aeCutCLCenter->SetParameterName( "AECutCLCenter", false );
    aeCutCLCenter->SetDefaultValue( 0 );
    aeCutCLCenter->SetDefaultUnit( "MeV" );
    aeCutCLCenter->SetUnitCandidates( "eV keV MeV GeV" );
    aeCutCLCenter->AvailableForStates( G4State_PreInit, G4State_Idle );

    aeCutCRCenter = new G4UIcmdWithADoubleAndUnit(
        ( CexmcMessenger::reconstructorDirName + "aeCutCRCenter" ).c_str(),
        this );
    aeCutCRCenter->SetGuidance( "Center of the ellipse in right calorimeter "
                                "absorbed energy coordinate" );
    aeCutCRCenter->SetParameterName( "AECutCRCenter", false );
    aeCutCRCenter->SetDefaultValue( 0 );
    aeCutCRCenter->SetDefaultUnit( "MeV" );
    aeCutCRCenter->SetUnitCandidates( "eV keV MeV GeV" );
    aeCutCRCenter->AvailableForStates( G4State_PreInit, G4State_Idle );

    aeCutCLWidth = new G4UIcmdWithADoubleAndUnit(
        ( CexmcMessenger::reconstructorDirName + "aeCutCLWidth" ).c_str(),
        this );
    aeCutCLWidth->SetGuidance( "Width of the ellipse in left calorimeter "
                               "absorbed energy coordinate" );
    aeCutCLWidth->SetParameterName( "AECutCLWidth", false );
    aeCutCLWidth->SetDefaultValue( 0 );
    aeCutCLWidth->SetDefaultUnit( "MeV" );
    aeCutCLWidth->SetUnitCandidates( "eV keV MeV GeV" );
    aeCutCLWidth->AvailableForStates( G4State_PreInit, G4State_Idle );

    aeCutCRWidth = new G4UIcmdWithADoubleAndUnit(
        ( CexmcMessenger::reconstructorDirName + "aeCutCRWidth" ).c_str(),
        this );
    aeCutCRWidth->SetGuidance( "Width of the ellipse in right calorimeter "
                               "absorbed energy coordinate" );
    aeCutCRWidth->SetParameterName( "AECutCRWidth", false );
    aeCutCRWidth->SetDefaultValue( 0 );
    aeCutCRWidth->SetDefaultUnit( "MeV" );
    aeCutCRWidth->SetUnitCandidates( "eV keV MeV GeV" );
    aeCutCRWidth->AvailableForStates( G4State_PreInit, G4State_Idle );

    aeCutAngle = new G4UIcmdWithADoubleAndUnit(
        ( CexmcMessenger::reconstructorDirName + "aeCutAngle" ).c_str(),
        this );
    aeCutAngle->SetGuidance( "Angle of the ellipse" );
    aeCutAngle->SetParameterName( "AECutAngle", false );
    aeCutAngle->SetDefaultValue( 0 );
    aeCutAngle->SetDefaultUnit( "deg" );
    aeCutAngle->SetUnitCandidates( "deg rad" );
    aeCutAngle->AvailableForStates( G4State_PreInit, G4State_Idle );
}


CexmcChargeExchangeReconstructorMessenger::
                                    ~CexmcChargeExchangeReconstructorMessenger()
{
    delete useTableMass;
    delete useMassCut;
    delete mCutOPCenter;
    delete mCutNOPCenter;
    delete mCutOPWidth;
    delete mCutNOPWidth;
    delete mCutAngle;
    delete useAbsorbedEnergyCut;
    delete aeCutCLCenter;
    delete aeCutCRCenter;
    delete aeCutCLWidth;
    delete aeCutCRWidth;
    delete aeCutAngle;
}


void  CexmcChargeExchangeReconstructorMessenger::SetNewValue(
                                        G4UIcommand *  cmd, G4String  value )
{
    do
    {
        if ( cmd == useTableMass )
        {
            reconstructor->UseTableMass(
                        G4UIcmdWithABool::GetNewBoolValue( value ) );
        }
        if ( cmd == useMassCut )
        {
            reconstructor->UseMassCut(
                        G4UIcmdWithABool::GetNewBoolValue( value ) );
        }
        if ( cmd == mCutOPCenter )
        {
            reconstructor->SetMassCutOPCenter(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
        }
        if ( cmd == mCutNOPCenter )
        {
            reconstructor->SetMassCutNOPCenter(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
        }
        if ( cmd == mCutOPWidth )
        {
            reconstructor->SetMassCutOPWidth(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
        }
        if ( cmd == mCutNOPWidth )
        {
            reconstructor->SetMassCutNOPWidth(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
        }
        if ( cmd == mCutAngle )
        {
            reconstructor->SetMassCutEllipseAngle(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
        }
        if ( cmd == useAbsorbedEnergyCut )
        {
            reconstructor->UseAbsorbedEnergyCut(
                        G4UIcmdWithABool::GetNewBoolValue( value ) );
        }
        if ( cmd == aeCutCLCenter )
        {
            reconstructor->SetAbsorbedEnergyCutCLCenter(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
        }
        if ( cmd == aeCutCRCenter )
        {
            reconstructor->SetAbsorbedEnergyCutCRCenter(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
        }
        if ( cmd == aeCutCLWidth )
        {
            reconstructor->SetAbsorbedEnergyCutCLWidth(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
        }
        if ( cmd == aeCutCRWidth )
        {
            reconstructor->SetAbsorbedEnergyCutCRWidth(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
        }
        if ( cmd == aeCutAngle )
        {
            reconstructor->SetAbsorbedEnergyCutEllipseAngle(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
        }
    } while ( false );
}

