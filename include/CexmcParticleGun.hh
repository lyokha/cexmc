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


class  CexmcParticleGun : public G4ParticleGun
{
    public:
        explicit CexmcParticleGun( G4int  nmbOfParticles );

    public:
        void  PrepareForNewEvent( void );
};


inline CexmcParticleGun::CexmcParticleGun( G4int  nmbOfParticles ) :
    G4ParticleGun( nmbOfParticles )
{
}


inline void  CexmcParticleGun::PrepareForNewEvent( void )
{
    /* this will prevent G4ParticleGun spam about kinetic energy redefinition */
    particle_energy = 0.0;
    particle_momentum = 0.0;
}


#endif

