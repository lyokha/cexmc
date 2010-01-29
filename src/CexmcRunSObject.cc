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
        const CexmcAngularRangeList &  angularRanges,
        G4bool  fermiMotionIsOn, CexmcEventCountPolicy  eventCountPolicy,
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
        CexmcCalorimeterEntryPointDefinitionAlgorithm  epDefinitionAlgorithm,
        CexmcCrystalSelectionAlgorithm  csAlgorithm, G4double  epDepth,
        G4bool  useTableMass, G4bool  useMassCut, G4double  mCutOPCenter,
        G4double  mCutNOPCenter, G4double  mCutOPWidth,  G4double  mCutNOPWidth,
        G4double  mCutAngle, CexmcNmbOfHitsInRanges  nmbOfHitsSampled,
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
    fermiMotionIsOn( fermiMotionIsOn ), eventCountPolicy( eventCountPolicy ),
    incidentParticle( incidentParticle ), beamPos( beamPos ),
    beamDir( beamDir ), beamMomentumAmp( beamMomentumAmp ),
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
    epDefinitionAlgorithm( epDefinitionAlgorithm ), csAlgorithm( csAlgorithm ),
    epDepth( epDepth ), useTableMass( useTableMass ), useMassCut( useMassCut ),
    mCutOPCenter( mCutOPCenter ), mCutNOPCenter( mCutNOPCenter ),
    mCutOPWidth( mCutOPWidth ), mCutNOPWidth( mCutNOPWidth ),
    mCutAngle( mCutAngle ), nmbOfHitsSampled( nmbOfHitsSampled ),
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

