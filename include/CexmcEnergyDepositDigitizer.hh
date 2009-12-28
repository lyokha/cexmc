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

#include <iosfwd>
#include <G4VDigitizerModule.hh>
#include "CexmcEnergyDepositStore.hh"
#include "CexmcRunManager.hh"
#include "CexmcException.hh"
#include "CexmcCommon.hh"


class  G4String;
class  CexmcEnergyDepositDigitizerMessenger;


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

        G4int     GetCalorimeterEDLeftMaxX( void ) const;

        G4int     GetCalorimeterEDLeftMaxY( void ) const;

        G4int     GetCalorimeterEDRightMaxX( void ) const;

        G4int     GetCalorimeterEDRightMaxY( void ) const;

        const CexmcEnergyDepositCalorimeterCollection &
                                GetCalorimeterEDLeftCollection( void ) const;

        const CexmcEnergyDepositCalorimeterCollection &
                                GetCalorimeterEDRightCollection( void ) const;

    public:
        G4bool    HasTriggered( void ) const;

    public:
        void      SetMonitorThreshold( G4double  value,
                                       G4bool fromMessenger = true );

        void      SetVetoCounterLeftThreshold( G4double  value,
                                               G4bool fromMessenger = true );

        void      SetVetoCounterRightThreshold( G4double  value,
                                                G4bool fromMessenger = true );

        void      SetVetoCountersThreshold( G4double  value );

        void      SetCalorimeterLeftThreshold( G4double  value,
                                               G4bool fromMessenger = true );

        void      SetCalorimeterRightThreshold( G4double  value,
                                                G4bool fromMessenger = true );

        void      SetCalorimetersThreshold( G4double  value );

        void      SetOuterCrystalsVetoAlgorithm(
                                    CexmcOuterCrystalsVetoAlgorithm  value,
                                    G4bool fromMessenger = true );

        void      SetOuterCrystalsVetoFraction( G4double  value,
                                                G4bool fromMessenger = true );

        G4double  GetMonitorThreshold( void ) const;

        G4double  GetVetoCounterLeftThreshold( void ) const;

        G4double  GetVetoCounterRightThreshold( void ) const;

        G4double  GetCalorimeterLeftThreshold( void ) const;

        G4double  GetCalorimeterRightThreshold( void ) const;

        CexmcOuterCrystalsVetoAlgorithm
                  GetOuterCrystalsVetoAlgorithm( void ) const;

        G4double  GetOuterCrystalsVetoFraction( void ) const;

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

        G4int                                    calorimeterEDLeftMaxX;

        G4int                                    calorimeterEDLeftMaxY;

        G4int                                    calorimeterEDRightMaxX;

        G4int                                    calorimeterEDRightMaxY;

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


inline G4int  CexmcEnergyDepositDigitizer::GetCalorimeterEDLeftMaxX( void )
                                                                        const
{
    return calorimeterEDLeftMaxX;
}


inline G4int  CexmcEnergyDepositDigitizer::GetCalorimeterEDLeftMaxY( void )
                                                                        const
{
    return calorimeterEDLeftMaxY;
}


inline G4int  CexmcEnergyDepositDigitizer::GetCalorimeterEDRightMaxX( void )
                                                                        const
{
    return calorimeterEDRightMaxX;
}


inline G4int  CexmcEnergyDepositDigitizer::GetCalorimeterEDRightMaxY( void )
                                                                        const
{
    return calorimeterEDRightMaxY;
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


inline void  CexmcEnergyDepositDigitizer::SetMonitorThreshold(
                                      G4double  value, G4bool  fromMessenger )
{
    if ( fromMessenger )
    {
        CexmcRunManager *  runManager( static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );
        if ( runManager->ProjectIsRead() && monitorEDThreshold < value )
            throw CexmcException( CexmcBadThreshold );
    }

    monitorEDThreshold = value;
}


inline void  CexmcEnergyDepositDigitizer::SetVetoCounterLeftThreshold(
                                      G4double  value, G4bool  fromMessenger )
{
    if ( fromMessenger )
    {
        CexmcRunManager *  runManager( static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );
        if ( runManager->ProjectIsRead() && vetoCounterEDLeftThreshold > value )
            throw CexmcException( CexmcBadThreshold );
    }

    vetoCounterEDLeftThreshold = value;
}


inline void  CexmcEnergyDepositDigitizer::SetVetoCounterRightThreshold(
                                      G4double  value, G4bool  fromMessenger )
{
    if ( fromMessenger )
    {
        CexmcRunManager *  runManager( static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );
        if ( runManager->ProjectIsRead() &&
             vetoCounterEDRightThreshold < value )
            throw CexmcException( CexmcBadThreshold );
    }

    vetoCounterEDRightThreshold = value;
}


inline void  CexmcEnergyDepositDigitizer::SetVetoCountersThreshold(
                                                            G4double  value )
{
    vetoCounterEDLeftThreshold = value;
    vetoCounterEDRightThreshold = value;
}


inline void  CexmcEnergyDepositDigitizer::SetCalorimeterLeftThreshold(
                                      G4double  value, G4bool  fromMessenger )
{
    if ( fromMessenger )
    {
        CexmcRunManager *  runManager( static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );
        if ( runManager->ProjectIsRead() && calorimeterEDLeftThreshold < value )
            throw CexmcException( CexmcBadThreshold );
    }

    calorimeterEDLeftThreshold = value;
}


inline void  CexmcEnergyDepositDigitizer::SetCalorimeterRightThreshold(
                                      G4double  value, G4bool  fromMessenger )
{
    if ( fromMessenger )
    {
        CexmcRunManager *  runManager( static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );
        if ( runManager->ProjectIsRead() &&
             calorimeterEDRightThreshold < value )
            throw CexmcException( CexmcBadThreshold );
    }

    calorimeterEDRightThreshold = value;
}


inline void  CexmcEnergyDepositDigitizer::SetCalorimetersThreshold(
                                                            G4double  value )
{
    calorimeterEDLeftThreshold = value;
    calorimeterEDRightThreshold = value;
}


inline void  CexmcEnergyDepositDigitizer::SetOuterCrystalsVetoAlgorithm(
                CexmcOuterCrystalsVetoAlgorithm  value, G4bool  fromMessenger )
{
    if ( fromMessenger )
    {
        CexmcRunManager *  runManager( static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );
        if ( runManager->ProjectIsRead() &&
             outerCrystalsVetoAlgorithm != CexmcNoOuterCrystalsVeto )
            throw CexmcException( CexmcBadOCVetoAlgorithm );
    }

    outerCrystalsVetoAlgorithm = value;
}


inline void  CexmcEnergyDepositDigitizer::SetOuterCrystalsVetoFraction(
                                      G4double  value, G4bool  fromMessenger )
{
    if ( fromMessenger )
    {
        CexmcRunManager *  runManager( static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );
        if ( runManager->ProjectIsRead() &&
             outerCrystalsVetoFraction > value )
            throw CexmcException( CexmcBadOCVetoFraction );
    }

    outerCrystalsVetoFraction = value;
}


inline G4bool  CexmcEnergyDepositDigitizer::IsOuterCrystal( G4int  column,
                                                            G4int  row )
{
    return column == 0 || column == nCrystalsInRow - 1 ||
           row == 0 || row == nCrystalsInColumn - 1;
}


inline G4double  CexmcEnergyDepositDigitizer::GetMonitorThreshold( void ) const
{
    return monitorEDThreshold;
}


inline G4double  CexmcEnergyDepositDigitizer::GetVetoCounterLeftThreshold(
                                                                    void ) const
{
    return vetoCounterEDLeftThreshold;
}


inline G4double  CexmcEnergyDepositDigitizer::GetVetoCounterRightThreshold(
                                                                    void ) const
{
    return vetoCounterEDRightThreshold;
}


inline G4double  CexmcEnergyDepositDigitizer::GetCalorimeterLeftThreshold(
                                                                    void ) const
{
    return calorimeterEDLeftThreshold;
}


inline G4double  CexmcEnergyDepositDigitizer::GetCalorimeterRightThreshold(
                                                                    void ) const
{
    return calorimeterEDRightThreshold;
}


inline  CexmcOuterCrystalsVetoAlgorithm
                CexmcEnergyDepositDigitizer::GetOuterCrystalsVetoAlgorithm(
                                                                    void ) const
{
    return outerCrystalsVetoAlgorithm;
}


inline G4double  CexmcEnergyDepositDigitizer::GetOuterCrystalsVetoFraction(
                                                                    void ) const
{
    return outerCrystalsVetoFraction;
}


std::ostream &  operator<<( std::ostream &  out,
                const CexmcEnergyDepositCalorimeterCollection &  edCollection );


#endif

