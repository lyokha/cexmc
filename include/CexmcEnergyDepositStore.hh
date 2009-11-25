/*
 * =============================================================================
 *
 *       Filename:  CexmcEnergyDepositStore.hh
 *
 *    Description:  store energy deposit data and const references to
 *                  energy deposit collections in calorimeters
 *
 *        Version:  1.0
 *        Created:  25.11.2009 15:32:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_ENERGY_DEPOSIT_STORE_HH
#define CEXMC_ENERGY_DEPOSIT_STORE_HH

#include <G4Types.hh>


struct  CexmcEnergyDepositStore
{
    CexmcEnergyDepositStore( G4double  monitorED,
                             G4double  vetoCounterEDLeft,
                             G4double  vetoCounterEDRight,
                             G4double  calorimeterEDLeft,
                             G4double  calorimeterEDRight,
                             const CexmcEnergyDepositCalorimeterCollection &
                                       calorimeterEDLeftCollection,
                             const CexmcEnergyDepositCalorimeterCollection &
                                       calorimeterEDRightCollection ) :
        monitorED( monitorED ), vetoCounterEDLeft( vetoCounterEDLeft ),
        vetoCounterEDRight( vetoCounterEDRight ),
        calorimeterEDLeft( calorimeterEDLeft ),
        calorimeterEDRight( calorimeterEDRight ),
        calorimeterEDLeftCollection( calorimeterEDLeftCollection ),
        calorimeterEDRightCollection( calorimeterEDRightCollection )
    {}

    G4double  monitorED;

    G4double  vetoCounterEDLeft;

    G4double  vetoCounterEDRight;

    G4double  calorimeterEDLeft;

    G4double  calorimeterEDRight;

    const CexmcEnergyDepositCalorimeterCollection &
              calorimeterEDLeftCollection;

    const CexmcEnergyDepositCalorimeterCollection &
              calorimeterEDRightCollection;
};


#endif

