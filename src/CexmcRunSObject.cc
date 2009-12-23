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
        const CexmcAngularRangeList &  angularRanges ) :
    productionModelType( productionModelType ), angularRanges( angularRanges )
{
}

