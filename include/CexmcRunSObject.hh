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
#include <boost/serialization/string.hpp>
#include "CexmcSimpleDecayTableStore.hh"
#include "CexmcSimpleThreeVectorStore.hh"
#include "CexmcAngularRange.hh"
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
                         const std::string &  incidentParticle,
                         const CexmcSimpleThreeVectorStore &
                                                        incidentParticlePos,
                         const CexmcSimpleThreeVectorStore &
                                                        incidentParticleDir );

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

        std::string                  incidentParticle;

        CexmcSimpleThreeVectorStore  incidentParticlePos;

        CexmcSimpleThreeVectorStore  incidentParticleDir;
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
    archive & incidentParticlePos;
    archive & incidentParticleDir;
}


#endif

