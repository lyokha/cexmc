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
#include "CexmcException.hh"

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
        static G4ParticleDefinition *  GetIncidentParticle( void );

        static G4ParticleDefinition *  GetNucleusParticle( void );

        static G4ParticleDefinition *  GetOutputParticle( void );

        static G4ParticleDefinition *  GetNucleusOutputParticle( void );

    private:
        G4ParticleDefinition *  thePiMinus;

        G4ParticleDefinition *  theProton;

        G4ParticleDefinition *  theNeutron;

        G4ParticleDefinition *  theOutputParticle;
};


template  < typename  OutputParticle >
CexmcChargeExchangeProductionModel< OutputParticle >::
                                        CexmcChargeExchangeProductionModel() :
    G4HadronicInteraction( "Studied Charge Exchange" ),
    thePiMinus( G4PionMinus::Definition() ),
    theProton( G4Proton::Definition() ), theNeutron( G4Neutron::Definition() ),
    theOutputParticle( OutputParticle::Definition() )
{
}


template  < typename  OutputParticle >
G4HadFinalState *  CexmcChargeExchangeProductionModel< OutputParticle >::
                            ApplyYourself( const G4HadProjectile &  projectile,
                                           G4Nucleus &  targetNucleus )
{
    if ( projectile.GetDefinition() != thePiMinus )
        throw CexmcException( CexmcWeirdException );

    theParticleChange.Clear();

    G4double         kinEnergy( projectile.GetKineticEnergy() );

    G4double         protonMass( theProton->GetPDGMass() );
    G4double         neutronMass( theNeutron->GetPDGMass() );
    G4double         outputParticleMass( theOutputParticle->GetPDGMass() );

    G4HadProjectile &          theProjectile( const_cast< G4HadProjectile & >(
                                                                projectile ) );
    const G4LorentzRotation &  projToLab(
                                    const_cast< const G4LorentzRotation & >(
                                            theProjectile.GetTrafoToLab() ) );
    productionModelData.incidentParticleLAB = theProjectile.Get4Momentum();
    productionModelData.incidentParticleLAB.transform( projToLab );
    productionModelData.nucleusParticleLAB.setPx( 0 );
    productionModelData.nucleusParticleLAB.setPy( 0 );
    productionModelData.nucleusParticleLAB.setPz( 0 );
    productionModelData.nucleusParticleLAB.setE( protonMass );

    if ( fermiMotionIsOn )
    {
        G4ThreeVector  targetNucleusMomentum(
                                        targetNucleus.GetFermiMomentum() );
        G4double       targetNucleusEnergy(
                            std::sqrt( targetNucleusMomentum.mag2() +
                                                protonMass * protonMass ) );
        productionModelData.nucleusParticleLAB = G4LorentzVector(
                                targetNucleusMomentum, targetNucleusEnergy );
    }
    productionModelData.nucleusParticleLAB.transform( projToLab );
    G4LorentzVector  lVecSum( productionModelData.incidentParticleLAB +
                              productionModelData.nucleusParticleLAB );
    G4ThreeVector    boostVec( lVecSum.boostVector() );

    productionModelData.incidentParticleSCM =
            productionModelData.incidentParticleLAB;
    productionModelData.nucleusParticleSCM =
            productionModelData.nucleusParticleLAB;

    productionModelData.incidentParticleSCM.boost( -boostVec );
    productionModelData.nucleusParticleSCM.boost( -boostVec );

    G4double   totalEnergy( productionModelData.incidentParticleSCM.e() +
                            productionModelData.nucleusParticleSCM.e() );

    /* introduced here to take into account weird run aborts on incorrect
     * double <-> float casts */
    G4double   epsilon( 0.0001 );
    /* kinematically impossible */
    if ( totalEnergy - epsilon < neutronMass + outputParticleMass )
    {
        G4cout << "KIN IMPOSSIBLE " << totalEnergy << " " <<
                  neutronMass + outputParticleMass << G4endl;
        theParticleChange.SetEnergyChange( kinEnergy );
        theParticleChange.SetMomentumChange(
                                    projectile.Get4Momentum().vect().unit());
        return &theParticleChange;
    }

    triggeredAngularRanges.clear();

    do
    {
        genin_.np = 2;
        genin_.tecm = totalEnergy / GeV;
        genin_.amass[ 0 ] = outputParticleMass / GeV;
        genin_.amass[ 1 ] = neutronMass / GeV;
        genbod_();
        productionModelData.outputParticleSCM.setPx(
                                                genout_.pcm[ 0 ][ 0 ] * GeV );
        productionModelData.outputParticleSCM.setPy(
                                                genout_.pcm[ 0 ][ 1 ] * GeV );
        productionModelData.outputParticleSCM.setPz(
                                                genout_.pcm[ 0 ][ 2 ] * GeV );
        productionModelData.outputParticleSCM.setE(
                                                genout_.pcm[ 0 ][ 3 ] * GeV );
        productionModelData.nucleusOutputParticleSCM.setPx(
                                                genout_.pcm[ 1 ][ 0 ] * GeV );
        productionModelData.nucleusOutputParticleSCM.setPy(
                                                genout_.pcm[ 1 ][ 1 ] * GeV );
        productionModelData.nucleusOutputParticleSCM.setPz(
                                                genout_.pcm[ 1 ][ 2 ] * GeV );
        productionModelData.nucleusOutputParticleSCM.setE(
                                                genout_.pcm[ 1 ][ 3 ] * GeV );
        for ( CexmcAngularRangeList::iterator  k( angularRanges.begin() );
                                                k != angularRanges.end(); ++k )
        {
            G4double  cosTheta(
                            productionModelData.outputParticleSCM.cosTheta() );
            if ( cosTheta <= k->top && cosTheta > k->bottom )
                triggeredAngularRanges.push_back( CexmcAngularRange(
                                                k->top, k->bottom, k->index ) );
        }
    } while ( triggeredAngularRanges.empty() );

    productionModelData.outputParticleLAB =
            productionModelData.outputParticleSCM;
    productionModelData.nucleusOutputParticleLAB =
            productionModelData.nucleusOutputParticleSCM;

    productionModelData.outputParticleLAB.boost( boostVec );
    productionModelData.nucleusOutputParticleLAB.boost( boostVec );

    G4cout << "OUT EN: " << genout_.pcm[ 0 ][ 3 ] << G4endl;
    G4cout << "NEUT EN: " << genout_.pcm[ 1 ][ 3 ] << G4endl;

    theParticleChange.SetStatusChange( stopAndKill );
    theParticleChange.SetEnergyChange( 0.0 );

    G4DynamicParticle *  secOutParticle( new G4DynamicParticle(
                theOutputParticle, productionModelData.outputParticleLAB ) );
    theParticleChange.AddSecondary( secOutParticle );
    G4DynamicParticle *  secNeutron( new G4DynamicParticle(
                theNeutron, productionModelData.nucleusOutputParticleLAB ) );
    theParticleChange.AddSecondary( secNeutron );

    productionModelData.incidentParticle = thePiMinus;
    productionModelData.nucleusParticle = theProton;
    productionModelData.outputParticle = theOutputParticle;
    productionModelData.nucleusOutputParticle = theNeutron;

    return &theParticleChange;
}


template  < typename  OutputParticle >
inline G4ParticleDefinition *
    CexmcChargeExchangeProductionModel< OutputParticle >::
                                        GetIncidentParticle( void )
{
    return G4PionMinus::Definition();
}


template  < typename  OutputParticle >
inline G4ParticleDefinition *
    CexmcChargeExchangeProductionModel< OutputParticle >::
                                        GetNucleusParticle( void )
{
    return G4Proton::Definition();
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
                                        GetNucleusOutputParticle( void )
{
    return G4Neutron::Definition();
}


#endif

