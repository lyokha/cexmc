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
            mCutOPWidth( NULL ), mCutNOPWidth( NULL ), mCutAngle( NULL )
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
    } while ( false );
}

