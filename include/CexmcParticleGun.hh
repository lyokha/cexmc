/*
 * =============================================================================
 *
 *       Filename:  CexmcParticleGun.hh
 *
 *    Description:  particle gun
 *
 *        Version:  1.0
 *        Created:  15.12.2009 00:41:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_PARTICLE_GUN_HH
#define CEXMC_PARTICLE_GUN_HH

#include <G4ParticleGun.hh>
#include <G4ThreeVector.hh>
#include "CexmcRunManager.hh"
#include "CexmcException.hh"

class  CexmcParticleGunMessenger;


class  CexmcParticleGun : public G4ParticleGun
{
    public:
        explicit CexmcParticleGun( G4int  nmbOfParticles );

        ~CexmcParticleGun();

    public:
        void  PrepareForNewEvent( void );

    public:
        const G4ThreeVector &  GetOrigPosition( void ) const;

        const G4ThreeVector &  GetOrigDirection( void ) const;

        G4double               GetOrigMomentumAmp( void ) const;

        void  SetOrigPosition( const G4ThreeVector &  position,
                               G4bool  fromMessenger = true );

        void  SetOrigDirection( const G4ThreeVector &  direction,
                                G4bool  fromMessenger = true );

        void  SetOrigMomentumAmp( G4double  momentumAmp,
                                  G4bool  fromMessenger = true );

        void  SetIncidentParticle( G4ParticleDefinition *  particleDefinition,
                                   G4bool  fromMessenger = true );

    private:
        G4ThreeVector  origPos;

        G4ThreeVector  origDir;

        G4double       origMomentumAmp;

    private:
        CexmcParticleGunMessenger *  messenger;
};


inline void  CexmcParticleGun::PrepareForNewEvent( void )
{
    /* this will prevent G4ParticleGun spam about kinetic energy redefinition */
    particle_energy = 0.0;
    particle_momentum = 0.0;
}


inline const G4ThreeVector &  CexmcParticleGun::GetOrigPosition( void ) const
{
    return origPos;
}


inline const G4ThreeVector &  CexmcParticleGun::GetOrigDirection( void ) const
{
    return origDir;
}


inline G4double  CexmcParticleGun::GetOrigMomentumAmp( void ) const
{
    return origMomentumAmp;
}


inline void  CexmcParticleGun::SetOrigPosition(
                    const G4ThreeVector &  position, G4bool  fromMessenger )
{
    if ( fromMessenger )
    {
        CexmcRunManager *  runManager( static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );
        if ( runManager->ProjectIsRead() )
            throw CexmcException( CexmcCmdIsNotAllowed );
    }

    origPos = position;
}


inline void  CexmcParticleGun::SetOrigDirection(
                    const G4ThreeVector &  direction, G4bool  fromMessenger )
{
    if ( fromMessenger )
    {
        CexmcRunManager *  runManager( static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );
        if ( runManager->ProjectIsRead() )
            throw CexmcException( CexmcCmdIsNotAllowed );
    }

    origDir = direction;
}


inline void  CexmcParticleGun::SetOrigMomentumAmp( G4double  momentumAmp,
                                                   G4bool  fromMessenger )
{
    if ( fromMessenger )
    {
        CexmcRunManager *  runManager( static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );
        if ( runManager->ProjectIsRead() )
            throw CexmcException( CexmcCmdIsNotAllowed );
    }

    origMomentumAmp = momentumAmp;
}


inline void  CexmcParticleGun::SetIncidentParticle(
            G4ParticleDefinition *  particleDefinition, G4bool  fromMessenger )
{
    if ( fromMessenger )
    {
        CexmcRunManager *  runManager( static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );
        if ( runManager->ProjectIsRead() )
            throw CexmcException( CexmcCmdIsNotAllowed );
    }

    SetParticleDefinition( particleDefinition );
}


#endif

