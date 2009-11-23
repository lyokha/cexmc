/*
 * =============================================================================
 *
 *       Filename:  CexmcEnergyDepositDigitizer.hh
 *
 *    Description:  digitizes of energy deposit in a single event
 *
 *        Version:  1.0
 *        Created:  23.11.2009 14:14:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_ENERGY_DEPOSIT_DIGITIZER_HH
#define CEXMC_ENERGY_DEPOSIT_DIGITIZER_HH

#include <G4VDigitizerModule.hh>
#include <G4TDigiCollection.hh>

class  G4String;


typedef std::vector< G4double >  CexmcEnergyDepositCrystalRowCollection;
typedef std::vector< CexmcEnergyDepositCrystalRowCollection >
                                 CexmcEnergyDepositCalorimeterCollection;


class  CexmcEnergyDepositDigitizer : public G4VDigitizerModule
{
    public:
        explicit CexmcEnergyDepositDigitizer( const G4String &  name );

    public:
        void  Digitize( void );

    public:
        G4double  GetMonitorED( void ) const;

        G4double  GetVetoCounterEDLeft( void ) const;

        G4double  GetVetoCounterEDRight( void ) const;

        G4double  GetCalorimeterEDLeft( void ) const;

        G4double  GetCalorimeterEDRight( void ) const;

        const CexmcEnergyDepositCalorimeterCollection &
                                GetCalorimeterEDLeftCollection( void ) const;

        const CexmcEnergyDepositCalorimeterCollection &
                                GetCalorimeterEDRightCollection( void ) const;

    public:
        G4bool    HasTriggered( void ) const;

    private:
        G4double                                 monitorED;

        G4double                                 vetoCounterEDLeft;

        G4double                                 vetoCounterEDRight;

        CexmcEnergyDepositCalorimeterCollection  calorimeterEDLeftCollection;

        CexmcEnergyDepositCalorimeterCollection  calorimeterEDRightCollection;

        G4double                                 calorimeterEDLeft;

        G4double                                 calorimeterEDRight;

        G4bool                                   hasTriggered;
};


inline G4double  CexmcEnergyDepositDigitizer::GetMonitorED( void ) const
{
    return monitorED;
}


inline G4double  CexmcEnergyDepositDigitizer::GetVetoCounterEDLeft( void ) const
{
    return vetoCounterEDLeft;
}


inline G4double  CexmcEnergyDepositDigitizer::GetVetoCounterEDRight( void )
                                                                        const
{
    return vetoCounterEDRight;
}


inline G4double  CexmcEnergyDepositDigitizer::GetCalorimeterEDLeft( void ) const
{
    return calorimeterEDLeft;
}


inline G4double  CexmcEnergyDepositDigitizer::GetCalorimeterEDRight( void )
                                                                        const
{
    return calorimeterEDRight;
}


inline const CexmcEnergyDepositCalorimeterCollection &
    CexmcEnergyDepositDigitizer::GetCalorimeterEDLeftCollection( void ) const
{
    return calorimeterEDLeftCollection;
}


inline const CexmcEnergyDepositCalorimeterCollection &
    CexmcEnergyDepositDigitizer::GetCalorimeterEDRightCollection( void ) const
{
    return calorimeterEDRightCollection;
}


inline G4bool  CexmcEnergyDepositDigitizer::HasTriggered( void ) const
{
    return hasTriggered;
}


inline std::ostream &  operator<<( std::ostream &  out,
                const CexmcEnergyDepositCalorimeterCollection & edCollection )
{
    out << std::endl;
    for ( CexmcEnergyDepositCalorimeterCollection::const_reverse_iterator
            k( edCollection.rbegin() ); k != edCollection.rend(); ++k )
    {
        for ( CexmcEnergyDepositCrystalRowCollection::const_reverse_iterator
                l( k->rbegin() ); l != k->rend(); ++l )
        {
            out.precision( 4 );
            out.width( 10 );
            out << *l;
        }
        out << std::endl;
    }

    return out;
}


#endif

