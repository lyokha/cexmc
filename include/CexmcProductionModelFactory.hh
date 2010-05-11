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


namespace  CexmcPrivate
{
    template  < typename  BasePhysics >
    class  CexmcBasePhysicsInstance
    {
        public:
            static const CexmcBasePhysicsUsed  value = CexmcNoBasePhysics;
    };

#ifdef CEXMC_USE_QGSP_BIC_EMY
    template<>
    class CexmcBasePhysicsInstance< QGSP_BIC_EMY >
    {
        public:
            static const CexmcBasePhysicsUsed  value = Cexmc_QGSP_BIC_EMY;
    };
#else
    template<>
    class CexmcBasePhysicsInstance< QGSP_BERT >
    {
        public:
            static const CexmcBasePhysicsUsed  value = Cexmc_QGSP_BERT;
    };
#endif
}


template  < typename  BasePhysics, typename  Particle,
            template  < typename, typename > class  StudiedPhysics >
class  CexmcProductionModelFactory
{
    public:
        static G4VUserPhysicsList *    Create(
                                CexmcProductionModelType  productionModelType );

        static G4ParticleDefinition *  GetIncidentParticle(
                                CexmcProductionModelType  productionModelType );

        static G4ParticleDefinition *  GetNucleusParticle(
                                CexmcProductionModelType  productionModelType );

        static G4ParticleDefinition *  GetOutputParticle(
                                CexmcProductionModelType  productionModelType );

        static G4ParticleDefinition *  GetNucleusOutputParticle(
                                CexmcProductionModelType  productionModelType );

        static CexmcBasePhysicsUsed    GetBasePhysics( void );

    private:
        CexmcProductionModelFactory();
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
    default :
        return NULL;
    }
}


template  < typename  BasePhysics, typename  Particle,
            template  < typename, typename > class  StudiedPhysics >
G4ParticleDefinition *
    CexmcProductionModelFactory< BasePhysics, Particle, StudiedPhysics >::
            GetIncidentParticle( CexmcProductionModelType  productionModelType )
{
    switch ( productionModelType )
    {
    case CexmcPionZeroProduction :
        return CexmcChargeExchangeProductionModel< G4PionZero >::
                                                        GetIncidentParticle();
    case CexmcEtaProduction :
        return CexmcChargeExchangeProductionModel< G4Eta >::
                                                        GetIncidentParticle();
    default :
        return NULL;
    }
}


template  < typename  BasePhysics, typename  Particle,
            template  < typename, typename > class  StudiedPhysics >
G4ParticleDefinition *
    CexmcProductionModelFactory< BasePhysics, Particle, StudiedPhysics >::
            GetNucleusParticle( CexmcProductionModelType  productionModelType )
{
    switch ( productionModelType )
    {
    case CexmcPionZeroProduction :
        return CexmcChargeExchangeProductionModel< G4PionZero >::
                                                        GetNucleusParticle();
    case CexmcEtaProduction :
        return CexmcChargeExchangeProductionModel< G4Eta >::
                                                        GetNucleusParticle();
    default :
        return NULL;
    }
}


template  < typename  BasePhysics, typename  Particle,
            template  < typename, typename > class  StudiedPhysics >
G4ParticleDefinition *
    CexmcProductionModelFactory< BasePhysics, Particle, StudiedPhysics >::
            GetOutputParticle( CexmcProductionModelType  productionModelType )
{
    switch ( productionModelType )
    {
    case CexmcPionZeroProduction :
        return CexmcChargeExchangeProductionModel< G4PionZero >::
                                                            GetOutputParticle();
    case CexmcEtaProduction :
        return CexmcChargeExchangeProductionModel< G4Eta >::GetOutputParticle();
    default :
        return NULL;
    }
}


template  < typename  BasePhysics, typename  Particle,
            template  < typename, typename > class  StudiedPhysics >
G4ParticleDefinition *
    CexmcProductionModelFactory< BasePhysics, Particle, StudiedPhysics >::
            GetNucleusOutputParticle(
                                CexmcProductionModelType  productionModelType )
{
    switch ( productionModelType )
    {
    case CexmcPionZeroProduction :
        return CexmcChargeExchangeProductionModel< G4PionZero >::
                                                    GetNucleusOutputParticle();
    case CexmcEtaProduction :
        return CexmcChargeExchangeProductionModel< G4Eta >::
                                                    GetNucleusOutputParticle();
    default :
        return NULL;
    }
}


template  < typename  BasePhysics, typename  Particle,
            template  < typename, typename > class  StudiedPhysics >
CexmcBasePhysicsUsed
    CexmcProductionModelFactory< BasePhysics, Particle, StudiedPhysics >::
            GetBasePhysics( void )
{
    return CexmcPrivate::CexmcBasePhysicsInstance< BasePhysics >::value;
}


#endif

