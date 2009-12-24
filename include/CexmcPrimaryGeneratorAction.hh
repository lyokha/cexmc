/*
 * =============================================================================
 *
 *       Filename:  CexmcPrimaryGeneratorAction.hh
 *
 *    Description:  primary particle position, direction, energy etc.
 *
 *        Version:  1.0
 *        Created:  11.10.2009 14:54:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_PRIMARY_GENERATOR_ACTION_HH
#define CEXMC_PRIMARY_GENERATOR_ACTION_HH

#include <G4VUserPrimaryGeneratorAction.hh>

class  G4Event;
class  CexmcParticleGun;
class  CexmcPrimaryGeneratorActionMessenger;


class  CexmcPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        CexmcPrimaryGeneratorAction();

        ~CexmcPrimaryGeneratorAction();

    public:
        void  GeneratePrimaries( G4Event *  event );

    public:
        void  SetFwhmPosX( G4double  value );

        void  SetFwhmPosY( G4double  value );

        void  SetFwhmAngleX( G4double  value );

        void  SetFwhmAngleY( G4double  value );

        void  SetFwhmMomentumAmp( G4double  value );

    public:
        CexmcParticleGun *  GetParticleGun( void );

    private:
        CexmcParticleGun *  particleGun;

        G4double            fwhmPosX;

        G4double            fwhmPosY;

        G4double            fwhmAngleX;

        G4double            fwhmAngleY;

        G4double            fwhmMomentumAmp;

    private:
        CexmcPrimaryGeneratorActionMessenger *  messenger;
};


inline void  CexmcPrimaryGeneratorAction::SetFwhmPosX( G4double  value )
{
    fwhmPosX = value;
}


inline void  CexmcPrimaryGeneratorAction::SetFwhmPosY( G4double  value )
{
    fwhmPosY = value;
}


inline void  CexmcPrimaryGeneratorAction::SetFwhmAngleX( G4double  value )
{
    fwhmAngleX = value;
}


inline void  CexmcPrimaryGeneratorAction::SetFwhmAngleY( G4double  value )
{
    fwhmAngleY = value;
}


inline void  CexmcPrimaryGeneratorAction::SetFwhmMomentumAmp( G4double  value )
{
    fwhmMomentumAmp = value;
}


#endif

