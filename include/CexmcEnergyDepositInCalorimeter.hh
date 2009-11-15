/*
 * =============================================================================
 *
 *       Filename:  CexmcEnergyDepositInCalorimeter.hh
 *
 *    Description:  energy deposit scorer in calorimeters
 *
 *        Version:  1.0
 *        Created:  14.11.2009 12:45:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_ENERGY_DEPOSIT_IN_CALORIMETER_HH
#define CEXMC_ENERGY_DEPOSIT_IN_CALORIMETER_HH

#include "CexmcEnergyDepositInLeftRightSet.hh"


class  CexmcEnergyDepositInCalorimeter : public CexmcEnergyDepositInLeftRightSet
{
    public:
        explicit CexmcEnergyDepositInCalorimeter( const G4String &  name );

    public:
        void  PrintAll( void );

    protected:
        G4int  GetIndex( G4Step *  step );

    protected:
        static G4int  copyDepth1BitsOffset;
};


#endif

