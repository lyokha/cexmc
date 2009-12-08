/*
 * =============================================================================
 *
 *       Filename:  CexmcPrimaryGeneratorAction.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11.10.2009 14:43:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#include <G4Event.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>
#include <globals.hh>
#include <Randomize.hh>
#include "CexmcPrimaryGeneratorAction.hh"
#include "CexmcPrimaryGeneratorActionMessenger.hh"
#include "CexmcPrimaryVertexInfo.hh"
#include "CexmcCommon.hh"


CexmcPrimaryGeneratorAction::CexmcPrimaryGeneratorAction() :
    wasActivated( false ), particleGun( NULL ),
    origPos( 0, 0, 0 ), origAngle( 0, 0, 0 ), origMomentumAmp( 0 ),
    fwhmPosX( 0 ), fwhmPosY( 0 ), fwhmAngleX( 0 ), fwhmAngleY( 0 ),
    messenger( NULL )
{
    particleGun = new G4ParticleGun( 1 );
    messenger = new CexmcPrimaryGeneratorActionMessenger( this );
}


CexmcPrimaryGeneratorAction::~CexmcPrimaryGeneratorAction()
{
    delete particleGun;
    delete messenger;
}


void  CexmcPrimaryGeneratorAction::GeneratePrimaries( G4Event *  event )
{
    if ( wasActivated )
    {
        particleGun->SetParticlePosition( origPos );
        particleGun->SetParticleMomentumDirection( origAngle );
        particleGun->SetParticleMomentum( origMomentumAmp );
    }
    else
    {
        origPos = particleGun->GetParticlePosition();
        origAngle = particleGun->GetParticleMomentumDirection();
        origMomentumAmp = particleGun->GetParticleMomentum();
        wasActivated = true;
    }

    G4ThreeVector  pos( particleGun->GetParticlePosition() );
    G4double       randPosX( G4RandGauss::shoot( pos.x(),
                                            fwhmPosX * CexmcFwhmToStddev ) );
    G4double       randPosY( G4RandGauss::shoot( pos.y(),
                                            fwhmPosY * CexmcFwhmToStddev ) );
    G4ThreeVector  newPos( randPosX, randPosY, pos.z() );

    G4ThreeVector  angle( particleGun->GetParticleMomentumDirection() );
    G4double       randAngleX( G4RandGauss::shoot( angle.x(),
                                            fwhmAngleX * CexmcFwhmToStddev ) );
    G4double       randAngleY( G4RandGauss::shoot( angle.y(),
                                            fwhmAngleY * CexmcFwhmToStddev ) );
    G4ThreeVector  newAngle( randAngleX, randAngleY, angle.z() );

    G4double       momentumAmp( particleGun->GetParticleMomentum() );
    G4double       newMomentumAmp( G4RandGauss::shoot( momentumAmp,
                                            fwhmMomentumAmp * momentumAmp *
                                            CexmcFwhmToStddev ) );

    particleGun->SetParticlePosition( newPos );
    particleGun->SetParticleMomentumDirection( newAngle );
    particleGun->SetParticleMomentum( newMomentumAmp );

    particleGun->GeneratePrimaryVertex( event );

    G4PrimaryVertex *  primaryVertex( event->GetPrimaryVertex() );
    if ( primaryVertex )
    {
        CexmcPrimaryVertexInfo *  primaryVertexInfo( new CexmcPrimaryVertexInfo(
                                        origPos, origAngle, origMomentumAmp ) );
        primaryVertex->SetUserInformation( primaryVertexInfo );
    }
}

