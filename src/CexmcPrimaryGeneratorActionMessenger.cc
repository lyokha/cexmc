/*
 * ============================================================================
 *
 *       Filename:  CexmcPrimaryGeneratorActionMessenger.cc
 *
 *    Description:  user assigned gun parameters
 *
 *        Version:  1.0
 *        Created:  02.11.2009 13:32:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include "CexmcPrimaryGeneratorAction.hh"
#include "CexmcPrimaryGeneratorActionMessenger.hh"
#include "CexmcMessenger.hh"


CexmcPrimaryGeneratorActionMessenger::CexmcPrimaryGeneratorActionMessenger(
        CexmcPrimaryGeneratorAction *  primaryGeneratorAction ) :
    primaryGeneratorAction( primaryGeneratorAction ),
    fwhmPosX( NULL ), fwhmPosY( NULL ), fwhmAngleX( NULL ), fwhmAngleY( NULL ),
    fwhmMomentumAmp( NULL )
{
    fwhmPosX = new G4UIcmdWithADoubleAndUnit(
                ( CexmcMessenger::gunDirName + "fwhmPosX" ).c_str(), this );
    fwhmPosX->SetGuidance( "Set positional fwhm of the beam along X axis" );
    fwhmPosX->SetParameterName( "FwhmPosX", false );
    fwhmPosX->SetDefaultUnit( "cm" );
    fwhmPosX->SetUnitCandidates( "mm cm m" );
    fwhmPosX->AvailableForStates( G4State_PreInit, G4State_Idle );

    fwhmPosY = new G4UIcmdWithADoubleAndUnit(
                ( CexmcMessenger::gunDirName + "fwhmPosY" ).c_str(), this );
    fwhmPosY->SetGuidance( "Set positional fwhm of the beam along Y axis" );
    fwhmPosY->SetParameterName( "FwhmPosY", false );
    fwhmPosY->SetDefaultUnit( "cm" );
    fwhmPosY->SetUnitCandidates( "mm cm m" );
    fwhmPosY->AvailableForStates( G4State_PreInit, G4State_Idle );

    fwhmAngleX = new G4UIcmdWithADoubleAndUnit(
                ( CexmcMessenger::gunDirName + "fwhmAngleX" ).c_str(), this );
    fwhmAngleX->SetGuidance( "Set angular fwhm of the beam along X axis" );
    fwhmAngleX->SetParameterName( "FwhmAngleX", false );
    fwhmAngleX->SetDefaultUnit( "deg" );
    fwhmAngleX->SetUnitCandidates( "deg rad" );
    fwhmAngleX->AvailableForStates( G4State_PreInit, G4State_Idle );

    fwhmAngleY = new G4UIcmdWithADoubleAndUnit(
                ( CexmcMessenger::gunDirName + "fwhmAngleY" ).c_str(), this );
    fwhmAngleY->SetGuidance( "Set angular fwhm of the beam along Y axis" );
    fwhmAngleY->SetParameterName( "FwhmAngleY", false );
    fwhmAngleY->SetDefaultUnit( "deg" );
    fwhmAngleY->SetUnitCandidates( "deg rad" );
    fwhmAngleY->AvailableForStates( G4State_PreInit, G4State_Idle );

    fwhmMomentumAmp = new G4UIcmdWithADouble(
            ( CexmcMessenger::gunDirName + "fwhmMomentumAmp" ).c_str(), this );
    fwhmMomentumAmp->SetGuidance( "Set fwhm of the beam momentum as fraction "
                                  "of its value" );
    fwhmMomentumAmp->SetParameterName( "FwhmMomentumAmp", false );
    fwhmMomentumAmp->AvailableForStates( G4State_PreInit, G4State_Idle );
}


CexmcPrimaryGeneratorActionMessenger::~CexmcPrimaryGeneratorActionMessenger()
{
    delete fwhmPosX;
    delete fwhmPosY;
    delete fwhmAngleX;
    delete fwhmAngleY;
    delete fwhmMomentumAmp;
}


void  CexmcPrimaryGeneratorActionMessenger::SetNewValue( G4UIcommand *  cmd,
                                                         G4String  value )
{
    do
    {
        if ( cmd == fwhmPosX )
        {
            primaryGeneratorAction->SetFwhmPosX(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
            break;
        }
        if ( cmd == fwhmPosY )
        {
            primaryGeneratorAction->SetFwhmPosY(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
            break;
        }
        if ( cmd == fwhmAngleX )
        {
            primaryGeneratorAction->SetFwhmAngleX(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
            break;
        }
        if ( cmd == fwhmAngleY )
        {
            primaryGeneratorAction->SetFwhmAngleY(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
            break;
        }
        if ( cmd == fwhmMomentumAmp )
        {
            primaryGeneratorAction->SetFwhmMomentumAmp(
                        G4UIcmdWithADouble::GetNewDoubleValue( value ) );
            break;
        }
    } while ( false );
}

