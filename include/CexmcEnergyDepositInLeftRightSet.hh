/*
 * =============================================================================
 *
 *       Filename:  CexmcEnergyDepositInLeftRightSet.hh
 *
 *    Description:  energy deposit scorer in left/right detector sets
 *                  (e.g. veto counters and calorimeters)
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

#ifndef CEXMC_ENERGY_DEPOSIT_IN_LEFT_RIGHT_SET_HH
#define CEXMC_ENERGY_DEPOSIT_IN_LEFT_RIGHT_SET_HH

#include "CexmcSimpleEnergyDeposit.hh"


class  CexmcEnergyDepositInLeftRightSet : public CexmcSimpleEnergyDeposit
{
    public:
        explicit CexmcEnergyDepositInLeftRightSet( const G4String &  name );

    public:
        void  PrintAll( void );

    protected:
        G4int  GetIndex( G4Step *  step );

    protected:
        static G4int  leftRightBitsOffset;
};


#endif

