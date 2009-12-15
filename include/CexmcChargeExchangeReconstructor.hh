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

        G4double  GetNucleusOutputParticleMass( void ) const;

        const CexmcProductionModelData &  GetProductionModelData( void ) const;

        void      UseTableMass( G4bool  on );

        void      UseMassCut( G4bool  on );

        void      SetMassCutOPCenter( G4double  value );

        void      SetMassCutNOPCenter( G4double  value );

        void      SetMassCutOPWidth( G4double  value );

        void      SetMassCutNOPWidth( G4double  value );

        void      SetMassCutEllipseAngle( G4double  value );

        G4bool    IsMassCutUsed( void ) const;

        G4bool    HasMassCutTriggered( void ) const;

    private:
        G4double  outputParticleMass;

        G4double  nucleusOutputParticleMass;

    private:
        CexmcProductionModelData  productionModelData;

    private:
        G4bool    useTableMass;

        G4bool    useMassCut;

        G4double  massCutOPCenter;

        G4double  massCutNOPCenter;

        G4double  massCutOPWidth;

        G4double  massCutNOPWidth;

        G4double  massCutEllipseAngle;

    private:
        G4bool    hasMassCutTriggered;

    private:
        CexmcChargeExchangeReconstructorMessenger *  messenger;
};


inline G4double  CexmcChargeExchangeReconstructor::GetOutputParticleMass(
                                                                    void ) const
{
    return outputParticleMass;
}


inline G4double  CexmcChargeExchangeReconstructor::GetNucleusOutputParticleMass(
                                                                    void ) const
{
    return nucleusOutputParticleMass;
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


inline void  CexmcChargeExchangeReconstructor::UseMassCut( G4bool  on )
{
    useMassCut = on;
}


inline void  CexmcChargeExchangeReconstructor::SetMassCutOPCenter(
                                                            G4double  value )
{
    massCutOPCenter = value;
}


inline void  CexmcChargeExchangeReconstructor::SetMassCutNOPCenter(
                                                            G4double  value )
{
    massCutNOPCenter = value;
}


inline void  CexmcChargeExchangeReconstructor::SetMassCutOPWidth(
                                                            G4double  value )
{
    massCutOPWidth = value;
}


inline void  CexmcChargeExchangeReconstructor::SetMassCutNOPWidth(
                                                            G4double  value )
{
    massCutNOPWidth = value;
}


inline void  CexmcChargeExchangeReconstructor::SetMassCutEllipseAngle(
                                                            G4double  value )
{
    massCutEllipseAngle = value;
}


inline G4bool  CexmcChargeExchangeReconstructor::IsMassCutUsed( void ) const
{
    return useMassCut;
}


inline G4bool  CexmcChargeExchangeReconstructor::HasMassCutTriggered( void )
                                                                        const
{
    return hasMassCutTriggered;
}


#endif

