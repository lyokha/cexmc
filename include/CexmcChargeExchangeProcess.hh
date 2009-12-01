/*
 * =============================================================================
 *
 *       Filename:  CexmcChargeExchangeProcess.hh
 *
 *    Description:  charge exchange process
 *
 *        Version:  1.0
 *        Created:  31.10.2009 23:44:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_CHARGE_EXCHANGE_PROCESS_HH
#define CEXMC_CHARGE_EXCHANGE_PROCESS_HH

#include <G4HadronicProcess.hh>
#include "CexmcCommon.hh"

class  CexmcProductionModel;


class  CexmcChargeExchangeProcess : public G4HadronicProcess
{
    public:
        explicit CexmcChargeExchangeProcess(
                        const G4String &  name = CexmcStudiedProcessLastName );

    public:
        void  RegisterProductionModel( G4HadronicInteraction *  model );

    public:
        CexmcProductionModel *  GetProductionModel( void );

    private:
        CexmcProductionModel *  productionModel;
};


inline CexmcProductionModel *
                        CexmcChargeExchangeProcess::GetProductionModel( void )
{
    return productionModel;
}


#endif

