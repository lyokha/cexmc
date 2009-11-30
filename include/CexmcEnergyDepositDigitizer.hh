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

class  G4String;
class  CexmcEnergyDepositDigitizerMessenger;


typedef std::vector< G4double >  CexmcEnergyDepositCrystalRowCollection;
typedef std::vector< CexmcEnergyDepositCrystalRowCollection >
                                 CexmcEnergyDepositCalorimeterCollection;

enum  CexmcOuterCrystalsVetoAlgorithm
{
    CexmcNoOuterCrystalsVeto,
    CexmcMaximumEDInASingleOuterCrystalVeto,
    CexmcFractionOfEDInOuterCrystalsVeto
};


class  CexmcEnergyDepositDigitizer : public G4VDigitizerModule
{
    public:
        explicit CexmcEnergyDepositDigitizer( const G4String &  name );

        ~CexmcEnergyDepositDigitizer();

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

    public:
        void      SetMonitorThreshold( G4double  value );

        void      SetVetoCounterLeftThreshold( G4double  value );

        void      SetVetoCounterRightThreshold( G4double  value );

        void      SetVetoCountersThreshold( G4double  value );

        void      SetCalorimeterLeftThreshold( G4double  value );

        void      SetCalorimeterRightThreshold( G4double  value );

        void      SetCalorimetersThreshold( G4double  value );

        void      SetOuterCrystalsVetoAlgorithm( G4int  value );

        void      SetOuterCrystalsVetoFraction( G4double  value );

    public:
        G4bool    IsOuterCrystal( G4int  column, G4int  row );

    private:
        void      InitializeData( void );

    private:
        G4double                                 monitorED;

        G4double                                 vetoCounterEDLeft;

        G4double                                 vetoCounterEDRight;

        CexmcEnergyDepositCalorimeterCollection  calorimeterEDLeftCollection;

        CexmcEnergyDepositCalorimeterCollection  calorimeterEDRightCollection;

        G4double                                 calorimeterEDLeft;

        G4double                                 calorimeterEDRight;

        G4bool                                   hasTriggered;

    private:
        G4double                                 monitorEDThreshold;

        G4double                                 vetoCounterEDLeftThreshold;

        G4double                                 vetoCounterEDRightThreshold;

        G4double                                 calorimeterEDLeftThreshold;

        G4double                                 calorimeterEDRightThreshold;

        CexmcOuterCrystalsVetoAlgorithm          outerCrystalsVetoAlgorithm;

        G4double                                 outerCrystalsVetoFraction;

    private:
        G4int                                    nCrystalsInColumn;

        G4int                                    nCrystalsInRow;

    private:
        CexmcEnergyDepositDigitizerMessenger *   messenger;
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


inline void  CexmcEnergyDepositDigitizer::SetMonitorThreshold( G4double  value )
{
    monitorEDThreshold = value;
}


inline void  CexmcEnergyDepositDigitizer::SetVetoCounterLeftThreshold(
                                                            G4double  value )
{
    vetoCounterEDLeftThreshold = value;
}


inline void  CexmcEnergyDepositDigitizer::SetVetoCounterRightThreshold(
                                                            G4double  value )
{
    vetoCounterEDRightThreshold = value;
}


inline void  CexmcEnergyDepositDigitizer::SetVetoCountersThreshold(
                                                            G4double  value )
{
    vetoCounterEDLeftThreshold = value;
    vetoCounterEDRightThreshold = value;
}


inline void  CexmcEnergyDepositDigitizer::SetCalorimeterLeftThreshold(
                                                            G4double  value )
{
    calorimeterEDLeftThreshold = value;
}


inline void  CexmcEnergyDepositDigitizer::SetCalorimeterRightThreshold(
                                                            G4double  value )
{
    calorimeterEDRightThreshold = value;
}


inline void  CexmcEnergyDepositDigitizer::SetCalorimetersThreshold(
                                                            G4double  value )
{
    calorimeterEDLeftThreshold = value;
    calorimeterEDRightThreshold = value;
}


inline void  CexmcEnergyDepositDigitizer::SetOuterCrystalsVetoAlgorithm(
                                                            G4int  value )
{
    switch ( value )
    {
    case 0 :
        outerCrystalsVetoAlgorithm = CexmcNoOuterCrystalsVeto;
        break;
    case 1 :
        outerCrystalsVetoAlgorithm = CexmcMaximumEDInASingleOuterCrystalVeto;
        break;
    case 2 :
        outerCrystalsVetoAlgorithm = CexmcFractionOfEDInOuterCrystalsVeto;
        break;
    default :
        break;
    }
}


inline void  CexmcEnergyDepositDigitizer::SetOuterCrystalsVetoFraction(
                                                            G4double  value )
{
    outerCrystalsVetoFraction = value;
}


inline G4bool  CexmcEnergyDepositDigitizer::IsOuterCrystal( G4int  column,
                                                            G4int  row )
{
    return column == 0 || column == nCrystalsInColumn - 1 ||
           row == 0 || row == nCrystalsInRow - 1;
}


inline std::ostream &  operator<<( std::ostream &  out,
                const CexmcEnergyDepositCalorimeterCollection &  edCollection )
{
    std::ostream::fmtflags  savedFlags( out.flags() );

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

    out.flags( savedFlags );

    return out;
}


#endif

