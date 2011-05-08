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

#ifdef CEXMC_USE_PERSISTENCY

#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include "CexmcSimpleDecayTableStore.hh"
#include "CexmcSimpleThreeVectorStore.hh"
#include "CexmcAngularRange.hh"
#include "CexmcSimpleRangeWithValue.hh"
#include "CexmcRun.hh"
#include "CexmcCommon.hh"


#define CEXMC_RUN_SOBJECT_VERSION 4


struct  CexmcRunSObject
{
    CexmcBasePhysicsUsed                 basePhysicsUsed;

    CexmcProductionModelType             productionModelType;

    std::string                          gdmlFileName;

    CexmcSimpleDecayTableStore           etaDecayTable;

    CexmcAngularRangeList                angularRanges;

    G4bool                               fermiMotionIsOn;

    std::vector< G4double >              calorimeterRegCuts;

    CexmcEventCountPolicy                eventCountPolicy;

    std::string                          beamParticle;

    CexmcSimpleThreeVectorStore          beamPos;

    CexmcSimpleThreeVectorStore          beamDir;

    G4double                             beamMomentumAmp;

    G4double                             beamFwhmPosX;

    G4double                             beamFwhmPosY;

    G4double                             beamFwhmDirX;

    G4double                             beamFwhmDirY;

    G4double                             beamFwhmMomentumAmp;

    G4double                             monitorEDThreshold;

    G4double                             vetoCounterEDLeftThreshold;

    G4double                             vetoCounterEDRightThreshold;

    G4double                             calorimeterEDLeftThreshold;

    G4double                             calorimeterEDRightThreshold;

    CexmcCalorimeterTriggerAlgorithm     calorimeterTriggerAlgorithm;

    CexmcOuterCrystalsVetoAlgorithm      outerCrystalsVetoAlgorithm;

    G4double                             outerCrystalsVetoFraction;

    G4bool                               applyFiniteCrystalResolution;

    CexmcEnergyRangeWithDoubleValueList  crystalResolutionData;

    CexmcCalorimeterEntryPointDefinitionAlgorithm
                                         epDefinitionAlgorithm;

    CexmcCalorimeterEntryPointDepthDefinitionAlgorithm
                                         epDepthDefinitionAlgorithm;

    CexmcCrystalSelectionAlgorithm       csAlgorithm;

    G4bool                               useInnerRefCrystal;

    G4double                             epDepth;

    G4bool                               useTableMass;

    G4bool                               useMassCut;

    G4double                             mCutOPCenter;

    G4double                             mCutNOPCenter;

    G4double                             mCutOPWidth;

    G4double                             mCutNOPWidth;

    G4double                             mCutAngle;

    G4bool                               useAbsorbedEnergyCut;

    G4double                             aeCutCLCenter;

    G4double                             aeCutCRCenter;

    G4double                             aeCutCLWidth;

    G4double                             aeCutCRWidth;

    G4double                             aeCutAngle;

    CexmcNmbOfHitsInRanges               nmbOfHitsSampled;

    CexmcNmbOfHitsInRanges               nmbOfHitsSampledFull;

    CexmcNmbOfHitsInRanges               nmbOfHitsTriggeredRealRange;

    CexmcNmbOfHitsInRanges               nmbOfHitsTriggeredRecRange;

    CexmcNmbOfHitsInRanges               nmbOfOrphanHits;

    G4int                                nmbOfFalseHitsTriggeredEDT;

    G4int                                nmbOfFalseHitsTriggeredRec;

    G4int                                nmbOfSavedEvents;

    G4int                                nmbOfSavedFastEvents;

    G4int                                numberOfEventsProcessed;

    G4int                                numberOfEventsProcessedEffective;

    G4int                                numberOfEventsToBeProcessed;

    std::string                          rProject;

    G4bool                               interactionsWithoutEDTWereSkipped;

    std::string                          cfFileName;

    CexmcEventDataVerboseLevel           evDataVerboseLevel;

    G4double                             proposedMaxIL;

    G4double                             expectedMomentumAmp;

    CexmcEDCollectionAlgoritm            edCollectionAlgorithm;

    unsigned int                         actualVersion;
};


namespace  boost
{
    namespace  serialization
    {
        template  < typename  Archive >
        void  serialize( Archive &  archive, CexmcRunSObject &  s,
                         const unsigned int  version )
        {
            archive & s.basePhysicsUsed;
            archive & s.productionModelType;
            archive & s.gdmlFileName;
            archive & s.etaDecayTable;
            archive & s.angularRanges;
            archive & s.fermiMotionIsOn;
            archive & s.calorimeterRegCuts;
            archive & s.eventCountPolicy;
            archive & s.beamParticle;
            archive & s.beamPos;
            archive & s.beamDir;
            archive & s.beamMomentumAmp;
            archive & s.beamFwhmPosX;
            archive & s.beamFwhmPosY;
            archive & s.beamFwhmDirX;
            archive & s.beamFwhmDirY;
            archive & s.beamFwhmMomentumAmp;
            archive & s.monitorEDThreshold;
            archive & s.vetoCounterEDLeftThreshold;
            archive & s.vetoCounterEDRightThreshold;
            archive & s.calorimeterEDLeftThreshold;
            archive & s.calorimeterEDRightThreshold;
            archive & s.calorimeterTriggerAlgorithm;
            archive & s.outerCrystalsVetoAlgorithm;
            archive & s.outerCrystalsVetoFraction;
            archive & s.applyFiniteCrystalResolution;
            archive & s.crystalResolutionData;
            archive & s.epDefinitionAlgorithm;
            archive & s.epDepthDefinitionAlgorithm;
            archive & s.csAlgorithm;
            if ( version > 0 )
                archive & s.useInnerRefCrystal;
            archive & s.epDepth;
            archive & s.useTableMass;
            archive & s.useMassCut;
            archive & s.mCutOPCenter;
            archive & s.mCutNOPCenter;
            archive & s.mCutOPWidth;
            archive & s.mCutNOPWidth;
            archive & s.mCutAngle;
            archive & s.useAbsorbedEnergyCut;
            archive & s.aeCutCLCenter;
            archive & s.aeCutCRCenter;
            archive & s.aeCutCLWidth;
            archive & s.aeCutCRWidth;
            archive & s.aeCutAngle;
            archive & s.nmbOfHitsSampled;
            archive & s.nmbOfHitsSampledFull;
            archive & s.nmbOfHitsTriggeredRealRange;
            archive & s.nmbOfHitsTriggeredRecRange;
            archive & s.nmbOfOrphanHits;
            archive & s.nmbOfFalseHitsTriggeredEDT;
            archive & s.nmbOfFalseHitsTriggeredRec;
            archive & s.nmbOfSavedEvents;
            archive & s.nmbOfSavedFastEvents;
            archive & s.numberOfEventsProcessed;
            archive & s.numberOfEventsProcessedEffective;
            archive & s.numberOfEventsToBeProcessed;
            if ( version > 1 )
            {
                archive & s.rProject;
                archive & s.interactionsWithoutEDTWereSkipped;
                archive & s.cfFileName;
                archive & s.evDataVerboseLevel;
            }
            if ( version > 2 )
                archive & s.proposedMaxIL;
            if ( version > 3 )
            {
                archive & s.expectedMomentumAmp;
                archive & s.edCollectionAlgorithm;
            }

            s.actualVersion = version;
        }
    }
}


BOOST_CLASS_VERSION( CexmcRunSObject, CEXMC_RUN_SOBJECT_VERSION )

#endif

#endif

