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
        CexmcOuterCrystalsVetoAlgorithm  outerCrystalsVetoAlgorithm,
        G4double  outerCrystalsVetoFraction ) :
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
    outerCrystalsVetoAlgorithm( outerCrystalsVetoAlgorithm ),
    outerCrystalsVetoFraction( outerCrystalsVetoFraction )
{
}

