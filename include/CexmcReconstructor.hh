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
        void  SetCalorimeterEntryPointDefinitionAlgorithm( G4int  algorithm );

        void  SetCrystalSelectionAlgorithm( G4int  algorithm );

        void  SetCalorimeterEntryPointDepth( G4double  depth );

    public:
        const G4ThreeVector &  GetCalorimeterEPLeftPosition( void ) const;

        const G4ThreeVector &  GetCalorimeterEPRightPosition( void ) const;

        const G4ThreeVector &  GetCalorimeterEPLeftDirection( void ) const;

        const G4ThreeVector &  GetCalorimeterEPRightDirection( void ) const;

    protected:
        virtual void  ReconstructEntryPoints( const CexmcEnergyDepositStore *
                                              edStore );

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
                                                            G4int  algorithm )
{
    switch ( algorithm )
    {
    case 0 :
        epDefinitionAlgorithm = CexmcEntryPointInTheCenter;
        break;
    case 1 :
        epDefinitionAlgorithm = CexmcEntryPointInTheCenterOfCrystalWithMaxED;
        break;
    case 2 :
        epDefinitionAlgorithm = CexmcEntryPointByLinearEDWeights;
        break;
    case 3 :
        epDefinitionAlgorithm = CexmcEntryPointBySqrtEDWeights;
        break;
    default :
        break;
    }
}


inline void  CexmcReconstructor::SetCrystalSelectionAlgorithm(
                                                            G4int  algorithm )
{
    switch ( algorithm )
    {
    case 0 :
        csAlgorithm = CexmcSelectAllCrystals;
        break;
    case 1 :
        csAlgorithm = CexmcSelectAdjacentCrystals;
        break;
    default :
        break;
    }
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


#endif

