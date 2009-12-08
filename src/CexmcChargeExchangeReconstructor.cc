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
#include "CexmcEnergyDepositStore.hh"
#include "CexmcBasicPhysicsSettings.hh"
#include "CexmcRunManager.hh"
#include "CexmcPrimaryVertexInfo.hh"
#include "CexmcCommon.hh"


CexmcChargeExchangeReconstructor::CexmcChargeExchangeReconstructor() :
    outputParticleMass( 0 ),  nucleusOutputParticleMass( 0 ),
    incidentParticle( NULL ), nucleusParticle( NULL ), outputParticle( NULL ),
    nucleusOutputParticle( NULL ), useTableMass( false )
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
                                        primaryVertexInfo->GetDirection() );
    G4double       incidentParticleMomentumAmp(
                                        primaryVertexInfo->GetMomentumAmp() );
    incidentParticleMomentum *= incidentParticleMomentumAmp;

    G4double       incidentParticlePDGMass(
                        productionModelData.incidentParticle->GetPDGMass() );
    G4double       incidentParticleEnergy(
        std::sqrt( incidentParticleMomentumAmp * incidentParticleMomentumAmp +
                   incidentParticlePDGMass * incidentParticlePDGMass ) );

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

    hasTriggered = true;
}

