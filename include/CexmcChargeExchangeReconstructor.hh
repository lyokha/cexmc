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
class  CexmcProductionModel;
class  CexmcParticleGun;


class  CexmcChargeExchangeReconstructor : public CexmcReconstructor
{
    public:
        explicit CexmcChargeExchangeReconstructor(
                                const CexmcProductionModel *  productionModel );

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

        void      UseAbsorbedEnergyCut( G4bool  on );

        void      SetAbsorbedEnergyCutCLCenter( G4double  value );

        void      SetAbsorbedEnergyCutCRCenter( G4double  value );

        void      SetAbsorbedEnergyCutCLWidth( G4double  value );

        void      SetAbsorbedEnergyCutCRWidth( G4double  value );

        void      SetAbsorbedEnergyCutEllipseAngle( G4double  value );

        void      SetupBeamParticle( void );

        G4bool    IsTableMassUsed( void ) const;

        G4bool    IsMassCutUsed( void ) const;

        G4double  GetMassCutOPCenter( void ) const;

        G4double  GetMassCutNOPCenter( void ) const;

        G4double  GetMassCutOPWidth( void ) const;

        G4double  GetMassCutNOPWidth( void ) const;

        G4double  GetMassCutEllipseAngle( void ) const;

        G4bool    HasMassCutTriggered( void ) const;

        G4bool    IsAbsorbedEnergyCutUsed( void ) const;

        G4double  GetAbsorbedEnergyCutCLCenter( void ) const;

        G4double  GetAbsorbedEnergyCutCRCenter( void ) const;

        G4double  GetAbsorbedEnergyCutCLWidth( void ) const;

        G4double  GetAbsorbedEnergyCutCRWidth( void ) const;

        G4double  GetAbsorbedEnergyCutEllipseAngle( void ) const;

        G4bool    HasAbsorbedEnergyCutTriggered( void ) const;

        G4bool    HasFullTrigger( void ) const;

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

        G4bool    useAbsorbedEnergyCut;

        G4double  absorbedEnergyCutCLCenter;

        G4double  absorbedEnergyCutCRCenter;

        G4double  absorbedEnergyCutCLWidth;

        G4double  absorbedEnergyCutCRWidth;

        G4double  absorbedEnergyCutEllipseAngle;

    private:
        G4bool    hasMassCutTriggered;

        G4bool    hasAbsorbedEnergyCutTriggered;

    private:
        G4bool                                       beamParticleIsInitialized;

        CexmcParticleGun *                           particleGun;

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


inline void  CexmcChargeExchangeReconstructor::UseAbsorbedEnergyCut(
                                                                G4bool  on )
{
    useAbsorbedEnergyCut = on;
}


inline void  CexmcChargeExchangeReconstructor::SetAbsorbedEnergyCutCLCenter(
                                                            G4double  value )
{
    absorbedEnergyCutCLCenter = value;
}


inline void  CexmcChargeExchangeReconstructor::SetAbsorbedEnergyCutCRCenter(
                                                            G4double  value )
{
    absorbedEnergyCutCRCenter = value;
}


inline void  CexmcChargeExchangeReconstructor::SetAbsorbedEnergyCutCLWidth(
                                                            G4double  value )
{
    absorbedEnergyCutCLWidth = value;
}


inline void  CexmcChargeExchangeReconstructor::SetAbsorbedEnergyCutCRWidth(
                                                            G4double  value )
{
    absorbedEnergyCutCRWidth = value;
}


inline void  CexmcChargeExchangeReconstructor::SetAbsorbedEnergyCutEllipseAngle(
                                                            G4double  value )
{
    absorbedEnergyCutEllipseAngle = value;
}


inline G4bool  CexmcChargeExchangeReconstructor::IsTableMassUsed( void ) const
{
    return useTableMass;
}


inline G4bool  CexmcChargeExchangeReconstructor::IsMassCutUsed( void ) const
{
    return useMassCut;
}


inline G4double  CexmcChargeExchangeReconstructor::GetMassCutOPCenter( void )
                                                                        const
{
    return massCutOPCenter;
}


inline G4double  CexmcChargeExchangeReconstructor::GetMassCutNOPCenter( void )
                                                                        const
{
    return massCutNOPCenter;
}


inline G4double  CexmcChargeExchangeReconstructor::GetMassCutOPWidth( void )
                                                                        const
{
    return massCutOPWidth;
}


inline G4double  CexmcChargeExchangeReconstructor::GetMassCutNOPWidth( void )
                                                                        const
{
    return massCutNOPWidth;
}


inline G4double  CexmcChargeExchangeReconstructor::GetMassCutEllipseAngle(
                                                                    void ) const
{
    return massCutEllipseAngle;
}


inline G4bool  CexmcChargeExchangeReconstructor::HasMassCutTriggered( void )
                                                                        const
{
    return hasMassCutTriggered;
}


inline G4bool  CexmcChargeExchangeReconstructor::IsAbsorbedEnergyCutUsed( void )
                                                                        const
{
    return useAbsorbedEnergyCut;
}


inline G4double  CexmcChargeExchangeReconstructor::GetAbsorbedEnergyCutCLCenter(
                                                                    void ) const
{
    return absorbedEnergyCutCLCenter;
}


inline G4double  CexmcChargeExchangeReconstructor::GetAbsorbedEnergyCutCRCenter(
                                                                    void ) const
{
    return absorbedEnergyCutCRCenter;
}


inline G4double  CexmcChargeExchangeReconstructor::GetAbsorbedEnergyCutCLWidth(
                                                                    void ) const
{
    return absorbedEnergyCutCLWidth;
}


inline G4double  CexmcChargeExchangeReconstructor::GetAbsorbedEnergyCutCRWidth(
                                                                    void ) const
{
    return absorbedEnergyCutCRWidth;
}


inline G4double  CexmcChargeExchangeReconstructor::
                                GetAbsorbedEnergyCutEllipseAngle( void ) const
{
    return absorbedEnergyCutEllipseAngle;
}


inline G4bool  CexmcChargeExchangeReconstructor::HasAbsorbedEnergyCutTriggered(
                                                                    void ) const
{
    return hasAbsorbedEnergyCutTriggered;
}


#endif

