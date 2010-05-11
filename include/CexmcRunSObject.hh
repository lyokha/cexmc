/*
 * =============================================================================
 *
 *       Filename:  CexmcRunSObject.hh
 *
 *    Description:  run data serialization helper
 *
 *        Version:  1.0
 *        Created:  23.12.2009 14:24:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_RUN_SOBJECT_HH
#define CEXMC_RUN_SOBJECT_HH

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include "CexmcSimpleDecayTableStore.hh"
#include "CexmcSimpleThreeVectorStore.hh"
#include "CexmcAngularRange.hh"
#include "CexmcSimpleRangeWithValue.hh"
#include "CexmcRun.hh"
#include "CexmcCommon.hh"


class  CexmcRunSObject
{
    friend class  boost::serialization::access;
    friend class  CexmcRunManager;

    public:
        CexmcRunSObject();

        CexmcRunSObject( CexmcBasePhysicsUsed  basePhysicsUsed,
                         CexmcProductionModelType  productionModelType,
                         const std::string &  gdmlFileName,
                         const CexmcSimpleDecayTableStore &  etaDecayTable,
                         const CexmcAngularRangeList &  angularRanges,
                         G4bool  fermiMotionIsOn,
                         const std::vector< G4double > &  calorimeterRegCuts,
                         CexmcEventCountPolicy  eventCountPolicy,
                         const std::string &  incidentParticle,
                         const CexmcSimpleThreeVectorStore &  beamPos,
                         const CexmcSimpleThreeVectorStore &  beamDir,
                         G4double  beamMomentumAmp, G4double  beamFwhmPosX,
                         G4double  beamFwhmPosY, G4double  beamFwhmDirX,
                         G4double  beamFwhmDirY,
                         G4double  beamFwhmMomentumAmp,
                         G4double  monitorEDThreshold,
                         G4double  vetoCounterEDLeftThreshold,
                         G4double  vetoCounterEDRightThreshold,
                         G4double  calorimeterEDLeftThreshold,
                         G4double  calorimeterEDRightThreshold,
                         CexmcCalorimeterTriggerAlgorithm
                                                    calorimeterTriggerAlgorithm,
                         CexmcOuterCrystalsVetoAlgorithm
                                                    outerCrystalsVetoAlgorithm,
                         G4double  outerCrystalsVetoFraction,
                         G4bool  applyFiniteCrystalResolution,
                         const CexmcEnergyRangeWithDoubleValueList &
                                                    crystalResolutionData,
                         CexmcCalorimeterEntryPointDefinitionAlgorithm
                                                    epDefinitionAlgorithm,
                         CexmcCalorimeterEntryPointDepthDefinitionAlgorithm
                                                    epDepthDefinitionAlgorithm,
                         CexmcCrystalSelectionAlgorithm  csAlgorithm,
                         G4double  epDepth, G4bool  useTableMass,
                         G4bool  useMassCut, G4double  mCutOPCenter,
                         G4double  mCutNOPCenter, G4double  mCutOPWidth,
                         G4double  mCutNOPWidth, G4double  mCutAngle,
                         G4bool  useAbsorbedEnergyCut, G4double  aeCutCLCenter,
                         G4double  aeCutCRCenter, G4double  aeCutCLWidth,
                         G4double  aeCutCRWidth, G4double  aeCutAngle,
                         CexmcNmbOfHitsInRanges  nmbOfHitsSampled,
                         CexmcNmbOfHitsInRanges  nmbOfHitsSampledFull,
                         CexmcNmbOfHitsInRanges  nmbOfHitsTriggeredRealRange,
                         CexmcNmbOfHitsInRanges  nmbOfHitsTriggeredRecRange,
                         CexmcNmbOfHitsInRanges  nmbOfOrphanHits,
                         G4int  nmbOfFalseHitsTriggeredEDT,
                         G4int  nmbOfFalseHitsTriggeredRec,
                         G4int  nmbOfSavedEvents, G4int  nmbOfSavedFastEvents,
                         G4int  numberOfEventsProcessed,
                         G4int  numberOfEventsProcessedEffective,
                         G4int  numberOfEventsToBeProcessed );

    private:
        template  < typename  Archive >
        void  serialize( Archive &  archive, const unsigned int  version );

    private:
        CexmcBasePhysicsUsed         basePhysicsUsed;

        CexmcProductionModelType     productionModelType;

        std::string                  gdmlFileName;

        CexmcSimpleDecayTableStore   etaDecayTable;

        CexmcAngularRangeList        angularRanges;

        G4bool                       fermiMotionIsOn;

        std::vector< G4double >      calorimeterRegCuts;

        CexmcEventCountPolicy        eventCountPolicy;

        std::string                  incidentParticle;

        CexmcSimpleThreeVectorStore  beamPos;

        CexmcSimpleThreeVectorStore  beamDir;

        G4double                     beamMomentumAmp;

        G4double                     beamFwhmPosX;

        G4double                     beamFwhmPosY;

        G4double                     beamFwhmDirX;

        G4double                     beamFwhmDirY;

        G4double                     beamFwhmMomentumAmp;

        G4double                     monitorEDThreshold;

        G4double                     vetoCounterEDLeftThreshold;

        G4double                     vetoCounterEDRightThreshold;

        G4double                     calorimeterEDLeftThreshold;

        G4double                     calorimeterEDRightThreshold;

        CexmcCalorimeterTriggerAlgorithm  calorimeterTriggerAlgorithm;

        CexmcOuterCrystalsVetoAlgorithm  outerCrystalsVetoAlgorithm;

        G4double                     outerCrystalsVetoFraction;

        G4bool                       applyFiniteCrystalResolution;

        CexmcEnergyRangeWithDoubleValueList  crystalResolutionData;

        CexmcCalorimeterEntryPointDefinitionAlgorithm  epDefinitionAlgorithm;

        CexmcCalorimeterEntryPointDepthDefinitionAlgorithm
                                                    epDepthDefinitionAlgorithm;

        CexmcCrystalSelectionAlgorithm  csAlgorithm;

        G4double                     epDepth;

        G4bool                       useTableMass;

        G4bool                       useMassCut;

        G4double                     mCutOPCenter;

        G4double                     mCutNOPCenter;

        G4double                     mCutOPWidth;

        G4double                     mCutNOPWidth;

        G4double                     mCutAngle;

        G4bool                       useAbsorbedEnergyCut;

        G4double                     aeCutCLCenter;

        G4double                     aeCutCRCenter;

        G4double                     aeCutCLWidth;

        G4double                     aeCutCRWidth;

        G4double                     aeCutAngle;

        CexmcNmbOfHitsInRanges       nmbOfHitsSampled;

        CexmcNmbOfHitsInRanges       nmbOfHitsSampledFull;

        CexmcNmbOfHitsInRanges       nmbOfHitsTriggeredRealRange;

        CexmcNmbOfHitsInRanges       nmbOfHitsTriggeredRecRange;

        CexmcNmbOfHitsInRanges       nmbOfOrphanHits;

        G4int                        nmbOfFalseHitsTriggeredEDT;

        G4int                        nmbOfFalseHitsTriggeredRec;

        G4int                        nmbOfSavedEvents;

        G4int                        nmbOfSavedFastEvents;

        G4int                        numberOfEventsProcessed;

        G4int                        numberOfEventsProcessedEffective;

        G4int                        numberOfEventsToBeProcessed;
};


template  < typename  Archive >
void  CexmcRunSObject::serialize( Archive &  archive, const unsigned int )
{
    archive & basePhysicsUsed;
    archive & productionModelType;
    archive & gdmlFileName;
    archive & etaDecayTable;
    archive & angularRanges;
    archive & fermiMotionIsOn;
    archive & calorimeterRegCuts;
    archive & eventCountPolicy;
    archive & incidentParticle;
    archive & beamPos;
    archive & beamDir;
    archive & beamMomentumAmp;
    archive & beamFwhmPosX;
    archive & beamFwhmPosY;
    archive & beamFwhmDirX;
    archive & beamFwhmDirY;
    archive & beamFwhmMomentumAmp;
    archive & monitorEDThreshold;
    archive & vetoCounterEDLeftThreshold;
    archive & vetoCounterEDRightThreshold;
    archive & calorimeterEDLeftThreshold;
    archive & calorimeterEDRightThreshold;
    archive & calorimeterTriggerAlgorithm;
    archive & outerCrystalsVetoAlgorithm;
    archive & outerCrystalsVetoFraction;
    archive & applyFiniteCrystalResolution;
    archive & crystalResolutionData;
    archive & epDefinitionAlgorithm;
    archive & epDepthDefinitionAlgorithm;
    archive & csAlgorithm;
    archive & epDepth;
    archive & useTableMass;
    archive & useMassCut;
    archive & mCutOPCenter;
    archive & mCutNOPCenter;
    archive & mCutOPWidth;
    archive & mCutNOPWidth;
    archive & mCutAngle;
    archive & useAbsorbedEnergyCut;
    archive & aeCutCLCenter;
    archive & aeCutCRCenter;
    archive & aeCutCLWidth;
    archive & aeCutCRWidth;
    archive & aeCutAngle;
    archive & nmbOfHitsSampled;
    archive & nmbOfHitsSampledFull;
    archive & nmbOfHitsTriggeredRealRange;
    archive & nmbOfHitsTriggeredRecRange;
    archive & nmbOfOrphanHits;
    archive & nmbOfFalseHitsTriggeredEDT;
    archive & nmbOfFalseHitsTriggeredRec;
    archive & nmbOfSavedEvents;
    archive & nmbOfSavedFastEvents;
    archive & numberOfEventsProcessed;
    archive & numberOfEventsProcessedEffective;
    archive & numberOfEventsToBeProcessed;
}


#endif

