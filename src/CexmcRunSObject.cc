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
        const CexmcSimpleThreeVectorStore &  incidentParticlePos,
        const CexmcSimpleThreeVectorStore &  incidentParticleDir ) :
    productionModelType( productionModelType ), gdmlFileName( gdmlFileName ),
    etaDecayTable( etaDecayTable ), angularRanges( angularRanges ),
    fermiMotionIsOn( fermiMotionIsOn ), eventCountPolicy( eventCountPolicy ),
    incidentParticle( incidentParticle ),
    incidentParticlePos( incidentParticlePos ),
    incidentParticleDir( incidentParticleDir )
{
}

