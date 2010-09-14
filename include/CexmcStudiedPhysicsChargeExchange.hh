/*
 * =============================================================================
 *
 *       Filename:  CexmcStudiedPhysicsChargeExchange.hh
 *
 *    Description:  charge exchange physics related flavour
 *
 *        Version:  1.0
 *        Created:  28.10.2009 17:16:44
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_STUDIED_PHYSICS_CHARGE_EXCHANGE_HH
#define CEXMC_STUDIED_PHYSICS_CHARGE_EXCHANGE_HH

#include "CexmcStudiedPhysics.hh"
#include "CexmcChargeExchangeProcess.hh"


template  < typename  ProductionModel >
class  CexmcStudiedPhysicsChargeExchange :
                        public CexmcStudiedPhysics< CexmcChargeExchangeProcess >
{
    public:
        explicit CexmcStudiedPhysicsChargeExchange(
                    CexmcPhysicsManager *  physicsManager, G4int  verbose = 0 );

        ~CexmcStudiedPhysicsChargeExchange();

    private:
        void  ApplyInteractionModel( G4HadronicProcess *  process );

    private:
        G4int  verbose;
};


template  < typename  ProductionModel >
CexmcStudiedPhysicsChargeExchange< ProductionModel >::
            CexmcStudiedPhysicsChargeExchange(
                    CexmcPhysicsManager *  physicsManager, G4int  verbose ) :
    CexmcStudiedPhysics< CexmcChargeExchangeProcess >( physicsManager ),
    verbose( verbose )
{
    if ( verbose > 1 )
        G4cout << "### StudiedPhysics: Charge Exchange" << G4endl;
}


template  < typename  ProductionModel >
CexmcStudiedPhysicsChargeExchange< ProductionModel >::
                                            ~CexmcStudiedPhysicsChargeExchange()
{
    delete productionModel;
}


template  < typename  ProductionModel >
void  CexmcStudiedPhysicsChargeExchange< ProductionModel >::
                        ApplyInteractionModel( G4HadronicProcess *  process )
{
    productionModel = new ProductionModel;
    CexmcChargeExchangeProcess *  chargeExchangeProcess(
                    static_cast< CexmcChargeExchangeProcess * >( process ) );
    chargeExchangeProcess->RegisterProductionModel( productionModel );
}


#endif

