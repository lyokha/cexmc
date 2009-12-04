/*
 * =============================================================================
 *
 *       Filename:  CexmcReconstructor.hh
 *
 *    Description:  reconstructor base class
 *
 *        Version:  1.0
 *        Created:  02.12.2009 15:44:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_RECONSTRUCTOR_HH
#define CEXMC_RECONSTRUCTOR_HH

#include <G4ThreeVector.hh>

class  CexmcReconstructorMessenger;
class  CexmcEnergyDepositStore;


enum  CexmcCalorimeterEntryPointDefinitionAlgorithm
{
    CexmcEntryPointInTheCenter,
    CexmcEntryPointInTheCenterOfCrystalWithMaxED,
    CexmcEntryPointByLinearEDWeights,
    CexmcEntryPointBySqrtEDWeights
};


enum  CexmcCrystalSelectionAlgorithm
{
    CexmcSelectAllCrystals,
    CexmcSelectAdjacentCrystals
};


class  CexmcReconstructor
{
    public:
        CexmcReconstructor();

        virtual ~CexmcReconstructor();

    public:
        virtual void  Reconstruct( const CexmcEnergyDepositStore *  edStore );

    public:
        void  SetCalorimeterEntryPointDefinitionAlgorithm(
                                                const G4String &  algorithm );

        void  SetCrystalSelectionAlgorithm( const G4String &  algorithm );

        void  SetCalorimeterEntryPointDepth( G4double  depth );

    public:
        const G4ThreeVector &  GetCalorimeterEPLeftPosition( void ) const;

        const G4ThreeVector &  GetCalorimeterEPRightPosition( void ) const;

        const G4ThreeVector &  GetCalorimeterEPLeftDirection( void ) const;

        const G4ThreeVector &  GetCalorimeterEPRightDirection( void ) const;

    public:
        G4bool  HasTriggered( void ) const;

    protected:
        virtual void  ReconstructEntryPoints( const CexmcEnergyDepositStore *
                                              edStore );

    protected:
        G4bool  hasTriggered;

    protected:
        CexmcCalorimeterEntryPointDefinitionAlgorithm  epDefinitionAlgorithm;

        CexmcCrystalSelectionAlgorithm                 csAlgorithm;

        G4double                                       epDepth;

    protected:
        G4ThreeVector  calorimeterEPLeftPosition;

        G4ThreeVector  calorimeterEPRightPosition;

        G4ThreeVector  calorimeterEPLeftDirection;

        G4ThreeVector  calorimeterEPRightDirection;

    private:
        G4int          nCrystalsInColumn;

        G4int          nCrystalsInRow;

        G4double       crystalWidth;

        G4double       crystalHeight;

        G4double       crystalLength;

    private:
        CexmcReconstructorMessenger *  messenger;
};


inline void  CexmcReconstructor::SetCalorimeterEntryPointDefinitionAlgorithm(
                                                const G4String &  algorithm )
{
    do
    {
        if ( algorithm  == "center" )
        {
            epDefinitionAlgorithm = CexmcEntryPointInTheCenter;
            break;
        }
        if ( algorithm  == "simple" )
        {
            epDefinitionAlgorithm = CexmcEntryPointInTheCenterOfCrystalWithMaxED;
            break;
        }
        if ( algorithm  == "linear" )
        {
            epDefinitionAlgorithm = CexmcEntryPointByLinearEDWeights;
            break;
        }
        if ( algorithm  == "sqrt" )
        {
            epDefinitionAlgorithm = CexmcEntryPointBySqrtEDWeights;
            break;
        }
    } while ( false );
}


inline void  CexmcReconstructor::SetCrystalSelectionAlgorithm(
                                                const G4String &  algorithm )
{
    do
    {
        if ( algorithm  == "all" )
        {
            csAlgorithm = CexmcSelectAllCrystals;
            break;
        }
        if ( algorithm  == "adjacent" )
        {
            csAlgorithm = CexmcSelectAdjacentCrystals;
            break;
        }
    } while ( false );
}


inline void  CexmcReconstructor::SetCalorimeterEntryPointDepth(
                                                            G4double  depth )
{
    epDepth = depth;
}


inline const G4ThreeVector &
                CexmcReconstructor::GetCalorimeterEPLeftPosition( void ) const
{
    return calorimeterEPLeftPosition;
}


inline const G4ThreeVector &
                CexmcReconstructor::GetCalorimeterEPRightPosition( void ) const
{
    return calorimeterEPRightPosition;
}


inline const G4ThreeVector &
                CexmcReconstructor::GetCalorimeterEPLeftDirection( void ) const
{
    return calorimeterEPLeftDirection;
}


inline const G4ThreeVector &
                CexmcReconstructor::GetCalorimeterEPRightDirection( void ) const
{
    return calorimeterEPRightDirection;
}


inline G4bool  CexmcReconstructor::HasTriggered( void ) const
{
    return hasTriggered;
}


#endif

