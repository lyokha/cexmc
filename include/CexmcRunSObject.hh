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
#include "CexmcCommon.hh"
#include "CexmcAngularRange.hh"


class  CexmcRunSObject
{
    friend class  boost::serialization::access;
    friend class  CexmcRunManager;

    public:
        CexmcRunSObject();

        CexmcRunSObject( CexmcProductionModelType  productionModelType,
                         const CexmcAngularRangeList &  angularRanges );

    private:
        template  < typename  Archive >
        void  serialize( Archive &  archive, const unsigned int  version );

    private:
        CexmcProductionModelType  productionModelType;

        CexmcAngularRangeList     angularRanges;
};


template  < typename  Archive >
void  CexmcRunSObject::serialize( Archive &  archive, const unsigned int )
{
    archive & productionModelType;
    archive & angularRanges;
}


#endif

