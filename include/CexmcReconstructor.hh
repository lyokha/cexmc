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
#include "CexmcEnergyDepositDigitizer.hh"

class  CexmcReconstructorMessenger;


enum  CexmcCalorimeterEntryPointDefinitionAlgorithm
{
    CexmcEntryPointInTheCenter,
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
        virtual void  Reconstruct(
                                const CexmcEnergyDepositCalorimeterCollection &
                                            calorimeterEDLeftCollection,
                                const CexmcEnergyDepositCalorimeterCollection &
                                            calorimeterEDRightCollection );

    public:
        void  SetCalorimeterEntryPointDefinitionAlgorithm( G4int  algorithm );

        void  SetCrystalSelectionAlgorithm( G4int  algorithm );

    protected:
        virtual void  ReconstructEntryPoints( void );

    protected:
        CexmcCalorimeterEntryPointDefinitionAlgorithm  epdAlgorithm;

        CexmcCrystalSelectionAlgorithm                 csAlgorithm;

    protected:
        G4ThreeVector  calorimeterEPLeftPosition;

        G4ThreeVector  calorimeterEPRightPosition;

        G4ThreeVector  calorimeterEPLeftDirection;

        G4ThreeVector  calorimeterEPRightDirection;

    private:
        CexmcReconstructorMessenger *  messenger;
};


inline void  CexmcReconstructor::SetCalorimeterEntryPointDefinitionAlgorithm(
                                                            G4int  algorithm )
{
    switch ( algorithm )
    {
    case 0 :
        epdAlgorithm = CexmcEntryPointInTheCenter;
        break;
    case 1 :
        epdAlgorithm = CexmcEntryPointByLinearEDWeights;
        break;
    case 2 :
        epdAlgorithm = CexmcEntryPointBySqrtEDWeights;
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


#endif

