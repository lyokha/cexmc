/*
 * ============================================================================
 *
 *       Filename:  CexmcChargeExchangeReconstructor.cc
 *
 *    Description:  charge exchange reconstructor
 *
 *        Version:  1.0
 *        Created:  02.12.2009 15:17:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4ThreeVector.hh>
#include <G4LorentzVector.hh>
#include "CexmcChargeExchangeReconstructor.hh"
#include "CexmcChargeExchangeReconstructorMessenger.hh"
#include "CexmcEnergyDepositStore.hh"
#include "CexmcBasicPhysicsSettings.hh"
#include "CexmcRunManager.hh"
#include "CexmcPrimaryVertexInfo.hh"
#include "CexmcCommon.hh"


CexmcChargeExchangeReconstructor::CexmcChargeExchangeReconstructor() :
    outputParticleMass( 0 ),  nucleusOutputParticleMass( 0 ),
    useTableMass( false ), useMassCut( false ), massCutOPCenter( 0 ),
    massCutNOPCenter( 0 ), massCutOPWidth( 0 ), massCutNOPWidth( 0 ),
    massCutEllipseAngle( 0 ), hasMassCutTriggered( false ), messenger( NULL )
{
    CexmcRunManager *         runManager( static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );
    CexmcProductionModelType  productionModelType(
                                        runManager->GetProductionModelType() );

    productionModelData.incidentParticle =
            CexmcChargeExchangePMFactory::GetIncidentParticle(
                                                        productionModelType );
    productionModelData.nucleusParticle =
            CexmcChargeExchangePMFactory::GetNucleusParticle(
                                                        productionModelType );
    productionModelData.outputParticle =
            CexmcChargeExchangePMFactory::GetOutputParticle(
                                                        productionModelType );
    productionModelData.nucleusOutputParticle =
            CexmcChargeExchangePMFactory::GetNucleusOutputParticle(
                                                        productionModelType );

    messenger = new CexmcChargeExchangeReconstructorMessenger( this );
}


CexmcChargeExchangeReconstructor::~CexmcChargeExchangeReconstructor()
{
    delete messenger;
}


void  CexmcChargeExchangeReconstructor::Reconstruct(
                                    const CexmcEnergyDepositStore *  edStore )
{
    ReconstructEntryPoints( edStore );
    if ( hasTriggered )
        ReconstructTargetPoint();
    if ( hasTriggered )
        ReconstructAngle();

    G4ThreeVector  epLeft( calorimeterEPLeftWorldPosition -
                           targetEPWorldPosition );
    G4ThreeVector  epRight( calorimeterEPRightWorldPosition -
                            targetEPWorldPosition );

    G4double  cosTheAngle( std::cos( theAngle ) );
    G4double  calorimeterEDLeft( edStore->calorimeterEDLeft );
    G4double  calorimeterEDRight( edStore->calorimeterEDRight );

    //G4double  cosOutputParticleLAB(
        //( calorimeterEDLeft * cosAngleLeft +
          //calorimeterEDRight * cosAngleRight ) /
          //std::sqrt( calorimeterEDLeft * calorimeterEDLeft +
                     //calorimeterEDRight * calorimeterEDRight +
                     //calorimeterEDLeft * calorimeterEDRight * cosTheAngle ) );

    outputParticleMass = std::sqrt( 2 * calorimeterEDLeft *
                                    calorimeterEDRight * ( 1 - cosTheAngle ) );

    G4ThreeVector    opdpLeftMomentum( epLeft );
    opdpLeftMomentum.setMag( calorimeterEDLeft );
    G4ThreeVector    opdpRightMomentum( epRight );
    opdpRightMomentum.setMag( calorimeterEDRight );
    G4ThreeVector    opMomentum( opdpLeftMomentum + opdpRightMomentum );

    G4double         opMass( useTableMass ?
                             productionModelData.outputParticle->GetPDGMass() :
                             outputParticleMass );
    G4double         opEnergy( std::sqrt(
                                    opMomentum.mag2() + opMass * opMass ) );
    productionModelData.outputParticleLAB = G4LorentzVector( opMomentum,
                                                             opEnergy );

    G4EventManager *          eventManager( G4EventManager::GetEventManager() );
    const G4Event *           event( eventManager->GetConstCurrentEvent() );
    CexmcPrimaryVertexInfo *  primaryVertexInfo(
            static_cast< CexmcPrimaryVertexInfo * >(
                            event->GetPrimaryVertex()->GetUserInformation() ) );
    if ( ! primaryVertexInfo )
        throw CexmcException( CexmcWeirdException );

    G4ThreeVector  incidentParticleMomentum(
                    primaryVertexInfo->GetParticleGun()->GetOrigDirection() );
    G4double       incidentParticleMomentumAmp(
                    primaryVertexInfo->GetParticleGun()->GetOrigMomentumAmp() );
    incidentParticleMomentum *= incidentParticleMomentumAmp;

    G4double       incidentParticlePDGMass(
                        productionModelData.incidentParticle->GetPDGMass() );
    G4double       incidentParticlePDGMass2( incidentParticlePDGMass *
                                             incidentParticlePDGMass );
    G4double       incidentParticleEnergy(
        std::sqrt( incidentParticleMomentumAmp * incidentParticleMomentumAmp +
                   incidentParticlePDGMass2 ) );

    productionModelData.incidentParticleLAB = G4LorentzVector(
                        incidentParticleMomentum, incidentParticleEnergy );
    G4double       nucleusParticlePDGMass(
                        productionModelData.nucleusParticle->GetPDGMass() );
    productionModelData.nucleusParticleLAB = G4LorentzVector(
                        G4ThreeVector( 0, 0, 0 ), nucleusParticlePDGMass );
    G4LorentzVector  lVecSum( productionModelData.incidentParticleLAB +
                        productionModelData.nucleusParticleLAB );
    G4ThreeVector    boostVec( lVecSum.boostVector() );

    productionModelData.nucleusOutputParticleLAB =
            lVecSum - productionModelData.outputParticleLAB;

    productionModelData.incidentParticleSCM =
            productionModelData.incidentParticleLAB;
    productionModelData.nucleusParticleSCM =
            productionModelData.nucleusParticleLAB;
    productionModelData.outputParticleSCM =
            productionModelData.outputParticleLAB;
    productionModelData.nucleusOutputParticleSCM =
            productionModelData.nucleusOutputParticleLAB;

    productionModelData.incidentParticleSCM.boost( -boostVec );
    productionModelData.nucleusParticleSCM.boost( -boostVec );
    productionModelData.outputParticleSCM.boost( -boostVec );
    productionModelData.nucleusOutputParticleSCM.boost( -boostVec );

    G4double       edDelta2(
                        std::pow( ( calorimeterEDLeft - calorimeterEDRight ) /
                                  ( calorimeterEDLeft + calorimeterEDRight ),
                                  2 ) );
    G4double       outputParticleKinEnergy(
                        std::sqrt( 2 * opMass * opMass / ( 1 - cosTheAngle ) /
                                   ( 1 - edDelta2 ) ) - opMass );
    G4ThreeVector  nopMomentum( incidentParticleMomentum - opMomentum );
    G4double       nopEnergy(
                        std::sqrt( incidentParticleMomentum.mag2() +
                                   incidentParticlePDGMass2 ) +
                        nucleusParticlePDGMass -
                        ( outputParticleKinEnergy + opMass ) );
    nucleusOutputParticleMass = std::sqrt( nopEnergy * nopEnergy -
                                           nopMomentum.mag2() );

    if ( useMassCut )
    {
        G4double  cosMassCutEllipseAngle( std::cos( massCutEllipseAngle ) );
        G4double  sinMassCutEllipseAngle( std::sin( massCutEllipseAngle ) );
        G4double  massCutOPWidth2( massCutOPWidth * massCutOPWidth );
        G4double  massCutNOPWidth2( massCutNOPWidth * massCutNOPWidth );

        hasMassCutTriggered =
            std::pow( ( outputParticleMass - massCutOPCenter ) *
                          cosMassCutEllipseAngle +
                      ( nucleusOutputParticleMass - massCutNOPCenter ) *
                          sinMassCutEllipseAngle, 2 ) / massCutOPWidth2 +
            std::pow( - ( outputParticleMass - massCutOPCenter ) *
                          sinMassCutEllipseAngle +
                      ( nucleusOutputParticleMass - massCutNOPCenter ) *
                          cosMassCutEllipseAngle, 2 ) / massCutNOPWidth2 < 1;
    }

    hasTriggered = true;
}

