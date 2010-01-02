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
#include "CexmcRun.hh"
#include "CexmcCommon.hh"


class  CexmcRunSObject
{
    friend class  boost::serialization::access;
    friend class  CexmcRunManager;

    public:
        CexmcRunSObject();

        CexmcRunSObject( CexmcProductionModelType  productionModelType,
                         const std::string &  gdmlFileName,
                         const CexmcSimpleDecayTableStore &  etaDecayTable,
                         const CexmcAngularRangeList &  angularRanges,
                         G4bool  fermiMotionIsOn,
                         CexmcEventCountPolicy  eventCountPolicy,
                         G4int  incidentParticle,
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
                         CexmcOuterCrystalsVetoAlgorithm
                                                    outerCrystalsVetoAlgorithm,
                         G4double  outerCrystalsVetoFraction,
                         CexmcCalorimeterEntryPointDefinitionAlgorithm
                                                    epDefinitionAlgorithm,
                         CexmcCrystalSelectionAlgorithm  csAlgorithm,
                         G4double  epDepth, G4bool  useTableMass,
                         G4bool  useMassCut, G4double  mCutOPCenter,
                         G4double  mCutNOPCenter, G4double  mCutOPWidth,
                         G4double  mCutNOPWidth, G4double  mCutAngle,
                         CexmcNmbOfHitsInRanges  nmbOfHitsSampled,
                         CexmcNmbOfHitsInRanges  nmbOfHitsTriggeredReal,
                         CexmcNmbOfHitsInRanges  nmbOfHitsTriggeredRec,
                         CexmcNmbOfHitsInRanges  nmbOfOrphanHits,
                         G4int  nmbOfSavedEvents,
                         G4int  numberOfEventsProcessed,
                         G4int  numberOfEventsProcessedEffective,
                         G4int  numberOfEventsToBeProcessed );

    private:
        template  < typename  Archive >
        void  serialize( Archive &  archive, const unsigned int  version );

    private:
        CexmcProductionModelType     productionModelType;

        std::string                  gdmlFileName;

        CexmcSimpleDecayTableStore   etaDecayTable;

        CexmcAngularRangeList        angularRanges;

        G4bool                       fermiMotionIsOn;

        CexmcEventCountPolicy        eventCountPolicy;

        G4int                        incidentParticle;

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

        CexmcOuterCrystalsVetoAlgorithm  outerCrystalsVetoAlgorithm;

        G4double                     outerCrystalsVetoFraction;

        CexmcCalorimeterEntryPointDefinitionAlgorithm  epDefinitionAlgorithm;

        CexmcCrystalSelectionAlgorithm  csAlgorithm;

        G4double                     epDepth;

        G4bool                       useTableMass;

        G4bool                       useMassCut;

        G4double                     mCutOPCenter;

        G4double                     mCutNOPCenter;

        G4double                     mCutOPWidth;

        G4double                     mCutNOPWidth;

        G4double                     mCutAngle;

        CexmcNmbOfHitsInRanges       nmbOfHitsSampled;

        CexmcNmbOfHitsInRanges       nmbOfHitsTriggeredReal;

        CexmcNmbOfHitsInRanges       nmbOfHitsTriggeredRec;

        CexmcNmbOfHitsInRanges       nmbOfOrphanHits;

        G4int                        nmbOfSavedEvents;

        G4int                        numberOfEventsProcessed;

        G4int                        numberOfEventsProcessedEffective;

        G4int                        numberOfEventsToBeProcessed;
};


template  < typename  Archive >
void  CexmcRunSObject::serialize( Archive &  archive, const unsigned int )
{
    archive & productionModelType;
    archive & gdmlFileName;
    archive & etaDecayTable;
    archive & angularRanges;
    archive & fermiMotionIsOn;
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
    archive & outerCrystalsVetoAlgorithm;
    archive & outerCrystalsVetoFraction;
    archive & epDefinitionAlgorithm;
    archive & csAlgorithm;
    archive & epDepth;
    archive & useTableMass;
    archive & useMassCut;
    archive & mCutOPCenter;
    archive & mCutNOPCenter;
    archive & mCutOPWidth;
    archive & mCutNOPWidth;
    archive & mCutAngle;
    archive & nmbOfHitsSampled;
    archive & nmbOfHitsTriggeredReal;
    archive & nmbOfHitsTriggeredRec;
    archive & nmbOfOrphanHits;
    archive & nmbOfSavedEvents;
    archive & numberOfEventsProcessed;
    archive & numberOfEventsProcessedEffective;
    archive & numberOfEventsToBeProcessed;
}


#endif

