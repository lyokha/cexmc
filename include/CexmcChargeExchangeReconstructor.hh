/*
 * =============================================================================
 *
 *       Filename:  CexmcChargeExchangeReconstructor.hh
 *
 *    Description:  charge exchange reconstructor
 *
 *        Version:  1.0
 *        Created:  02.12.2009 15:07:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_CHARGE_EXCHANGE_RECONSTRUCTOR_HH
#define CEXMC_CHARGE_EXCHANGE_RECONSTRUCTOR_HH

#include "CexmcReconstructor.hh"


class  CexmcChargeExchangeReconstructor : public CexmcReconstructor
{
    public:
        void  Reconstruct( const CexmcEnergyDepositCalorimeterCollection &
                                                calorimeterEDLeftCollection,
                           const CexmcEnergyDepositCalorimeterCollection &
                                                calorimeterEDRightCollection );
};


#endif

