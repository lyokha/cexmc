/*
 * =============================================================================
 *
 *       Filename:  CexmcChargeExchangeProductionModel.hh
 *
 *    Description:  charge exchange physics itself
 *
 *        Version:  1.0
 *        Created:  01.11.2009 00:30:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_CHARGE_EXCHANGE_PRODUCTION_MODEL_HH
#define CEXMC_CHARGE_EXCHANGE_PRODUCTION_MODEL_HH

#include <G4HadronicInteraction.hh>
#include <G4HadFinalState.hh>
#include <G4HadProjectile.hh>
#include <G4Nucleus.hh>
#include <G4Proton.hh>
#include <G4Neutron.hh>
#include "CexmcProductionModel.hh"

/* Old Fortran routine GENBOD() is used here to sample kinematics in
 * CM system */


extern "C"
{
    extern int  genbod_( void );
}

extern struct  genbod_in_data
{
    int    np;
    float  tecm;
    float  amass[ 18 ];
    int    kgenev;
}  genin_;

extern struct  genbod_out_data
{
    float  pcm[ 18 ][ 5 ];
    float  wt;
}  genout_;


template  < typename  OutputParticle >
class  CexmcChargeExchangeProductionModel :
                                    virtual public G4HadronicInteraction,
                                            public CexmcProductionModel
{
    public:
        CexmcChargeExchangeProductionModel();

    public:
        G4HadFinalState *  ApplyYourself( const G4HadProjectile &  projectile,
                                          G4Nucleus &  targetNucleus );

    public:
        static G4ParticleDefinition *  GetOutputParticle( void );

        static G4ParticleDefinition *  GetNucleusParticle( void );

    private:
        G4ParticleDefinition *  theProton;

        G4ParticleDefinition *  theNeutron;

        G4ParticleDefinition *  theOutputParticle;
};


template  < typename  OutputParticle >
CexmcChargeExchangeProductionModel< OutputParticle >::
                                        CexmcChargeExchangeProductionModel() :
    G4HadronicInteraction( "Studied Charge Exchange" ),
    theProton( G4Proton::Proton() ), theNeutron( G4Neutron::Neutron() ),
    theOutputParticle( OutputParticle::Definition() )
{
}


template  < typename  OutputParticle >
G4HadFinalState *  CexmcChargeExchangeProductionModel< OutputParticle >::
                            ApplyYourself( const G4HadProjectile &  projectile,
                                           G4Nucleus &  targetNucleus )
{
    theParticleChange.Clear();

    G4double         kinEnergy( projectile.GetKineticEnergy() );

    G4double         protonMass( theProton->GetPDGMass() );
    G4double         neutronMass( theNeutron->GetPDGMass() );
    G4double         outputParticleMass( theOutputParticle->GetPDGMass() );

    G4LorentzVector  lVecProj( projectile.Get4Momentum() );
    G4LorentzVector  lVecTargNucl( 0., 0., 0., protonMass );
    if ( fermiMotionIsOn )
    {
        G4ThreeVector  targetNucleusMomentum(
                                        targetNucleus.GetFermiMomentum() );
        G4double       targetNucleusEnergy(
                            std::sqrt( targetNucleusMomentum.mag2() +
                                                protonMass * protonMass ) );
        lVecTargNucl = G4LorentzVector( targetNucleusMomentum,
                                        targetNucleusEnergy );
    }
    G4LorentzVector  lVec( lVecProj + lVecTargNucl );
    G4ThreeVector    boostVec( lVec.boostVector() );

    lVecProj.boost( -boostVec );
    lVecTargNucl.boost( -boostVec );

    G4double         totalEnergy( lVecProj.e() + lVecTargNucl.e() );  

    // kinematically impossible
    if ( totalEnergy < neutronMass + outputParticleMass )
    {
        G4cout << "KIN IMPOSSIBLE " << totalEnergy << " " <<
                  neutronMass + outputParticleMass << G4endl;
        theParticleChange.SetEnergyChange( kinEnergy );
        theParticleChange.SetMomentumChange(
                                    projectile.Get4Momentum().vect(). unit());
        return &theParticleChange;
    }

    genin_.np = 2;
    genin_.tecm = totalEnergy / GeV;
    genin_.amass[ 0 ] = outputParticleMass / GeV;
    genin_.amass[ 1 ] = neutronMass / GeV;
    genbod_();
    G4LorentzVector  lVecOutPart( genout_.pcm[ 0 ][ 0 ] * GeV,
                                  genout_.pcm[ 0 ][ 1 ] * GeV,
                                  genout_.pcm[ 0 ][ 2 ] * GeV,
                                  genout_.pcm[ 0 ][ 3 ] * GeV );
    G4LorentzVector  lVecNeut( genout_.pcm[ 1 ][ 0 ] * GeV,
                               genout_.pcm[ 1 ][ 1 ] * GeV,
                               genout_.pcm[ 1 ][ 2 ] * GeV, 
                               genout_.pcm[ 1 ][ 3 ] * GeV );
    lVecOutPart.boost( boostVec );
    lVecNeut.boost( boostVec );
    G4cout << "OUT EN: " << genout_.pcm[ 0 ][ 3 ] << G4endl;
    G4cout << "NEUT EN: " << genout_.pcm[ 1 ][ 3 ] << G4endl;

    theParticleChange.SetStatusChange( stopAndKill );
    theParticleChange.SetEnergyChange( 0.0 );

    G4DynamicParticle *  secOutParticle( new G4DynamicParticle(
                                            theOutputParticle, lVecOutPart ) );
    theParticleChange.AddSecondary( secOutParticle );
    G4DynamicParticle *  secNeutron( new G4DynamicParticle(
                                            theNeutron, lVecNeut ) );
    theParticleChange.AddSecondary( secNeutron );

    return &theParticleChange;
}


template  < typename  OutputParticle >
inline G4ParticleDefinition *
    CexmcChargeExchangeProductionModel< OutputParticle >::
                                        GetOutputParticle( void )
{
    return OutputParticle::Definition();
}


template  < typename  OutputParticle >
inline G4ParticleDefinition *
    CexmcChargeExchangeProductionModel< OutputParticle >::
                                        GetNucleusParticle( void )
{
    return G4Neutron::Definition();
}


#endif

