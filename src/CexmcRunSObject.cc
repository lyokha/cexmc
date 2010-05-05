/*
 * ============================================================================
 *
 *       Filename:  CexmcRunSObject.cc
 *
 *    Description:  run data serialization helper
 *
 *        Version:  1.0
 *        Created:  23.12.2009 14:41:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include "CexmcRunSObject.hh"


CexmcRunSObject::CexmcRunSObject()
{
}


CexmcRunSObject::CexmcRunSObject(
        CexmcProductionModelType  productionModelType,
        const std::string &  gdmlFileName,
        const CexmcSimpleDecayTableStore &  etaDecayTable,
        const CexmcAngularRangeList &  angularRanges, G4bool  fermiMotionIsOn,
        const std::vector< G4double > &  calorimeterRegCuts,
        CexmcEventCountPolicy  eventCountPolicy,
        const std::string &  incidentParticle,
        const CexmcSimpleThreeVectorStore &  beamPos,
        const CexmcSimpleThreeVectorStore &  beamDir, G4double  beamMomentumAmp,
        G4double  beamFwhmPosX, G4double  beamFwhmPosY, G4double  beamFwhmDirX,
        G4double  beamFwhmDirY, G4double  beamFwhmMomentumAmp,
        G4double  monitorEDThreshold, G4double  vetoCounterEDLeftThreshold,
        G4double  vetoCounterEDRightThreshold,
        G4double  calorimeterEDLeftThreshold,
        G4double  calorimeterEDRightThreshold,
        CexmcCalorimeterTriggerAlgorithm  calorimeterTriggerAlgorithm,
        CexmcOuterCrystalsVetoAlgorithm  outerCrystalsVetoAlgorithm,
        G4double  outerCrystalsVetoFraction,
        G4bool  applyFiniteCrystalResolution,
        const CexmcEnergyRangeWithDoubleValueList &  crystalResolutionData,
        CexmcCalorimeterEntryPointDefinitionAlgorithm  epDefinitionAlgorithm,
        CexmcCalorimeterEntryPointDepthDefinitionAlgorithm
                                                    epDepthDefinitionAlgorithm,
        CexmcCrystalSelectionAlgorithm  csAlgorithm, G4double  epDepth,
        G4bool  useTableMass, G4bool  useMassCut, G4double  mCutOPCenter,
        G4double  mCutNOPCenter, G4double  mCutOPWidth, G4double  mCutNOPWidth,
        G4double  mCutAngle, G4bool  useAbsorbedEnergyCut,
        G4double  aeCutCLCenter, G4double  aeCutCRCenter,
        G4double  aeCutCLWidth, G4double  aeCutCRWidth, G4double  aeCutAngle,
        CexmcNmbOfHitsInRanges  nmbOfHitsSampled,
        CexmcNmbOfHitsInRanges  nmbOfHitsSampledFull,
        CexmcNmbOfHitsInRanges  nmbOfHitsTriggeredRealRange,
        CexmcNmbOfHitsInRanges  nmbOfHitsTriggeredRecRange,
        CexmcNmbOfHitsInRanges  nmbOfOrphanHits,
        G4int  nmbOfFalseHitsTriggeredEDT, G4int  nmbOfFalseHitsTriggeredRec,
        G4int  nmbOfSavedEvents, G4int  nmbOfSavedFastEvents,
        G4int  numberOfEventsProcessed, G4int  numberOfEventsProcessedEffective,
        G4int  numberOfEventsToBeProcessed ) :
    productionModelType( productionModelType ), gdmlFileName( gdmlFileName ),
    etaDecayTable( etaDecayTable ), angularRanges( angularRanges ),
    fermiMotionIsOn( fermiMotionIsOn ),
    calorimeterRegCuts( calorimeterRegCuts ),
    eventCountPolicy( eventCountPolicy ), incidentParticle( incidentParticle ),
    beamPos( beamPos ), beamDir( beamDir ), beamMomentumAmp( beamMomentumAmp ),
    beamFwhmPosX( beamFwhmPosX ), beamFwhmPosY( beamFwhmPosY ),
    beamFwhmDirX( beamFwhmDirX ), beamFwhmDirY( beamFwhmDirY ),
    beamFwhmMomentumAmp( beamFwhmMomentumAmp ),
    monitorEDThreshold( monitorEDThreshold ),
    vetoCounterEDLeftThreshold( vetoCounterEDLeftThreshold ),
    vetoCounterEDRightThreshold( vetoCounterEDRightThreshold ),
    calorimeterEDLeftThreshold( calorimeterEDLeftThreshold ),
    calorimeterEDRightThreshold( calorimeterEDRightThreshold ),
    calorimeterTriggerAlgorithm( calorimeterTriggerAlgorithm ),
    outerCrystalsVetoAlgorithm( outerCrystalsVetoAlgorithm ),
    outerCrystalsVetoFraction( outerCrystalsVetoFraction ),
    applyFiniteCrystalResolution( applyFiniteCrystalResolution ),
    crystalResolutionData( crystalResolutionData ),
    epDefinitionAlgorithm( epDefinitionAlgorithm ),
    epDepthDefinitionAlgorithm( epDepthDefinitionAlgorithm ),
    csAlgorithm( csAlgorithm ), epDepth( epDepth ),
    useTableMass( useTableMass ), useMassCut( useMassCut ),
    mCutOPCenter( mCutOPCenter ), mCutNOPCenter( mCutNOPCenter ),
    mCutOPWidth( mCutOPWidth ), mCutNOPWidth( mCutNOPWidth ),
    mCutAngle( mCutAngle ), useAbsorbedEnergyCut( useAbsorbedEnergyCut ),
    aeCutCLCenter( aeCutCLCenter ), aeCutCRCenter( aeCutCRCenter ),
    aeCutCLWidth( aeCutCLWidth ), aeCutCRWidth( aeCutCRWidth ),
    aeCutAngle( aeCutAngle ), nmbOfHitsSampled( nmbOfHitsSampled ),
    nmbOfHitsSampledFull( nmbOfHitsSampledFull ),
    nmbOfHitsTriggeredRealRange( nmbOfHitsTriggeredRealRange ),
    nmbOfHitsTriggeredRecRange( nmbOfHitsTriggeredRecRange ),
    nmbOfOrphanHits( nmbOfOrphanHits ),
    nmbOfFalseHitsTriggeredEDT( nmbOfFalseHitsTriggeredEDT ),
    nmbOfFalseHitsTriggeredRec( nmbOfFalseHitsTriggeredRec ),
    nmbOfSavedEvents( nmbOfSavedEvents ),
    nmbOfSavedFastEvents( nmbOfSavedFastEvents ),
    numberOfEventsProcessed( numberOfEventsProcessed ),
    numberOfEventsProcessedEffective( numberOfEventsProcessedEffective ),
    numberOfEventsToBeProcessed( numberOfEventsToBeProcessed )
{
}

