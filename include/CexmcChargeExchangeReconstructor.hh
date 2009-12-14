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
#include "CexmcProductionModelData.hh"

class  CexmcChargeExchangeReconstructorMessenger;
class  CexmcEnergyDepositStore;


class  CexmcChargeExchangeReconstructor : public CexmcReconstructor
{
    public:
        CexmcChargeExchangeReconstructor();

        ~CexmcChargeExchangeReconstructor();

    public:
        void  Reconstruct( const CexmcEnergyDepositStore *  edStore );

    public:
        G4double  GetOutputParticleMass( void ) const;

        const CexmcProductionModelData &  GetProductionModelData( void ) const;

        void      UseTableMass( G4bool  on );

    private:
        G4double  outputParticleMass;

        G4double  nucleusOutputParticleMass;

    private:
        CexmcProductionModelData  productionModelData;

    private:
        G4bool    useTableMass;

    private:
        CexmcChargeExchangeReconstructorMessenger *  messenger;
};


inline G4double  CexmcChargeExchangeReconstructor::GetOutputParticleMass( void )
                                                                        const
{
    return outputParticleMass;
}


inline const CexmcProductionModelData &
        CexmcChargeExchangeReconstructor::GetProductionModelData( void ) const
{
    return productionModelData;
}


inline void  CexmcChargeExchangeReconstructor::UseTableMass( G4bool  on )
{
    useTableMass = on;
}


#endif

