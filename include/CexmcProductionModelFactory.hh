/*
 * =============================================================================
 *
 *       Filename:  CexmcProductionModelFactory.hh
 *
 *    Description:  production model factory
 *
 *        Version:  1.0
 *        Created:  03.11.2009 23:20:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_PRODUCTION_MODEL_FACTORY_HH
#define CEXMC_PRODUCTION_MODEL_FACTORY_HH

#include <G4VUserPhysicsList.hh>
#include <G4PionZero.hh>
#include <G4Eta.hh>
#include "CexmcChargeExchangeProductionModel.hh"
#include "CexmcPhysicsList.hh"
#include "CexmcCommon.hh"


template  < typename  BasePhysics, typename  Particle,
            template  < typename, typename > class  StudiedPhysics >
class  CexmcProductionModelFactory
{
    public:
        G4VUserPhysicsList *  Create(
                                CexmcProductionModelType  productionModelType );
};


template  < typename  BasePhysics, typename  Particle,
            template  < typename, typename > class  StudiedPhysics >
G4VUserPhysicsList *
    CexmcProductionModelFactory< BasePhysics, Particle, StudiedPhysics >::
                    Create( CexmcProductionModelType  productionModelType )
{
    switch ( productionModelType )
    {
    case CexmcPionZeroProduction :
        return new CexmcPhysicsList< BasePhysics, Particle, StudiedPhysics,
                            CexmcChargeExchangeProductionModel< G4PionZero > >;
    case CexmcEtaProduction :
        return new CexmcPhysicsList< BasePhysics, Particle, StudiedPhysics,
                            CexmcChargeExchangeProductionModel< G4Eta > >;
    default:
        return NULL;
    }
}


#endif

