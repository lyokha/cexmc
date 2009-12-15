/*
 * ============================================================================
 *
 *       Filename:  CexmcParticleGunMessenger.cc
 *
 *    Description:  original position and momentum of the incident particle
 *
 *        Version:  1.0
 *        Created:  15.12.2009 14:02:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWith3Vector.hh>
#include <G4UIcmdWith3VectorAndUnit.hh>
#include "CexmcParticleGun.hh"
#include "CexmcParticleGunMessenger.hh"
#include "CexmcMessenger.hh"


CexmcParticleGunMessenger::CexmcParticleGunMessenger(
                                            CexmcParticleGun *  particleGun ) :
    particleGun( particleGun ), setOrigPosition( NULL ),
    setOrigDirection( NULL ), setOrigMomentumAmp( NULL )
{
    setOrigPosition = new G4UIcmdWith3VectorAndUnit( 
        ( CexmcMessenger::gunDirName + "position" ).c_str(), this );
    setOrigPosition->SetGuidance( "Original position of the beam" );
    setOrigPosition->SetParameterName( "PositionX", "PositionY", "PositionZ",
                                       false );
    setOrigPosition->SetDefaultUnit( "cm" );
    setOrigPosition->SetUnitCandidates( "mm cm m" );
    setOrigPosition->AvailableForStates( G4State_PreInit, G4State_Idle );

    setOrigDirection = new G4UIcmdWith3Vector(
        ( CexmcMessenger::gunDirName + "direction" ).c_str(), this );
    setOrigDirection->SetGuidance( "Original direction of the beam" );
    setOrigDirection->SetParameterName( "DirectionX", "DirectionY",
                                        "DirectionZ", false );
    setOrigDirection->SetRange(
        "DirectionX >= -1.0 && DirectionX <= 1.0 && "
        "DirectionY >= -1.0 && DirectionY <= 1.0 && "
        "DirectionZ >= -1.0 && DirectionZ <= 1.0" );
    setOrigDirection->AvailableForStates( G4State_PreInit, G4State_Idle );

    setOrigMomentumAmp = new G4UIcmdWithADoubleAndUnit(
        ( CexmcMessenger::gunDirName + "momentumAmp" ).c_str(), this );
    setOrigMomentumAmp->SetGuidance( "Original momentum value" );
    setOrigMomentumAmp->SetParameterName( "MomentumAmp", false );
    setOrigMomentumAmp->SetRange( "MomentumAmp > 0" );
    setOrigMomentumAmp->SetDefaultUnit( "MeV" );
    setOrigMomentumAmp->SetUnitCandidates( "eV keV MeV GeV" );
    setOrigMomentumAmp->AvailableForStates( G4State_PreInit, G4State_Idle );
}


CexmcParticleGunMessenger::~CexmcParticleGunMessenger()
{
    delete setOrigPosition;
    delete setOrigDirection;
    delete setOrigMomentumAmp;
}


void CexmcParticleGunMessenger::SetNewValue( G4UIcommand *  cmd,
                                             G4String  value )
{
    do
    {
        if ( cmd == setOrigPosition )
        {
            particleGun->SetOrigPosition(
                    G4UIcmdWith3VectorAndUnit::GetNew3VectorValue( value ) );
            break;
        }
        if ( cmd == setOrigDirection )
        {
            particleGun->SetOrigDirection(
                    G4UIcmdWith3Vector::GetNew3VectorValue( value ) );
            break;
        }
        if ( cmd == setOrigMomentumAmp )
        {
            particleGun->SetOrigMomentumAmp(
                    G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
            break;
        }
    } while ( false );
}

