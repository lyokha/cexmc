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
#include "CexmcGenbod.hh"
#include "CexmcReimplementedGenbod.hh"
#include "CexmcException.hh"


template  < typename  OutputParticle >
class  CexmcChargeExchangeProductionModel : public G4HadronicInteraction,
                                            public CexmcProductionModel
{
    public:
        CexmcChargeExchangeProductionModel();

        ~CexmcChargeExchangeProductionModel();

    public:
        G4HadFinalState *  ApplyYourself( const G4HadProjectile &  projectile,
                                          G4Nucleus &  targetNucleus );

    public:
        static G4ParticleDefinition *  GetIncidentParticle( void );

        static G4ParticleDefinition *  GetNucleusParticle( void );

        static G4ParticleDefinition *  GetOutputParticle( void );

        static G4ParticleDefinition *  GetNucleusOutputParticle( void );

    private:
        CexmcPhaseSpaceGenerator *  phaseSpaceGenerator;

    private:
        G4ParticleDefinition *      thePiMinus;

        G4ParticleDefinition *      theProton;

        G4ParticleDefinition *      theNeutron;

        G4ParticleDefinition *      theOutputParticle;
};


template  < typename  OutputParticle >
CexmcChargeExchangeProductionModel< OutputParticle >::
                                        CexmcChargeExchangeProductionModel() :
    G4HadronicInteraction( "Studied Charge Exchange" ),
    phaseSpaceGenerator( NULL ), thePiMinus( G4PionMinus::Definition() ),
    theProton( G4Proton::Definition() ), theNeutron( G4Neutron::Definition() ),
    theOutputParticle( OutputParticle::Definition() )
{
    CexmcPhaseSpaceInVector   inVec;

    inVec.push_back( &productionModelData.incidentParticleSCM );
    inVec.push_back( &productionModelData.nucleusParticleSCM );

    CexmcPhaseSpaceOutVector  outVec;

    outVec.push_back( CexmcPhaseSpaceOutVectorElement(
                            &productionModelData.outputParticleSCM,
                            theOutputParticle->GetPDGMass() ) );
    outVec.push_back( CexmcPhaseSpaceOutVectorElement(
                            &productionModelData.nucleusOutputParticleSCM,
                            theNeutron->GetPDGMass() ) );

#ifdef CEXMC_USE_GENBOD
    phaseSpaceGenerator = new CexmcGenbod;
#else
    phaseSpaceGenerator = new CexmcReimplementedGenbod;
#endif

    phaseSpaceGenerator->SetParticles( inVec, outVec );
}


template  < typename  OutputParticle >
CexmcChargeExchangeProductionModel< OutputParticle >::
                                        ~CexmcChargeExchangeProductionModel()
{
    delete phaseSpaceGenerator;
}


template  < typename  OutputParticle >
G4HadFinalState *  CexmcChargeExchangeProductionModel< OutputParticle >::
                            ApplyYourself( const G4HadProjectile &  projectile,
                                           G4Nucleus &  targetNucleus )
{
    if ( projectile.GetDefinition() != thePiMinus )
        throw CexmcException( CexmcWeirdException );

    theParticleChange.Clear();

    G4double           kinEnergy( projectile.GetKineticEnergy() );
    G4double           protonMass( theProton->GetPDGMass() );
    G4HadProjectile &  theProjectile( const_cast< G4HadProjectile & >(
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

    triggeredAngularRanges.clear();

    if ( ! phaseSpaceGenerator->CheckKinematics() )
    {
        theParticleChange.SetEnergyChange( kinEnergy );
        theParticleChange.SetMomentumChange(
                                    projectile.Get4Momentum().vect().unit());
        return &theParticleChange;
    }

    do
    {
        phaseSpaceGenerator->Generate();
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

