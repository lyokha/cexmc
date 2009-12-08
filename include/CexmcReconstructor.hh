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
#include <G4AffineTransform.hh>

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

        const G4ThreeVector &  GetTargetEPPosition( void ) const;

        const G4ThreeVector &  GetTargetEPDirection( void ) const;

        const G4ThreeVector &  GetCalorimeterEPLeftWorldPosition( void ) const;

        const G4ThreeVector &  GetCalorimeterEPRightWorldPosition( void ) const;

        const G4ThreeVector &  GetCalorimeterEPLeftWorldDirection( void ) const;

        const G4ThreeVector &  GetCalorimeterEPRightWorldDirection( void )
                                                                        const;

        const G4ThreeVector &  GetTargetEPWorldPosition( void ) const;

        const G4ThreeVector &  GetTargetEPWorldDirection( void ) const;

        G4double               GetTheAngle( void ) const;

    public:
        G4bool  HasTriggered( void ) const;

    protected:
        void  ReconstructEntryPoints(
                                    const CexmcEnergyDepositStore *  edStore );

        void  ReconstructTargetPoint( void );

        void  ReconstructAngle( void );

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

        G4ThreeVector  targetEPPosition;

        G4ThreeVector  targetEPDirection;

        G4ThreeVector  calorimeterEPLeftWorldPosition;

        G4ThreeVector  calorimeterEPRightWorldPosition;

        G4ThreeVector  calorimeterEPLeftWorldDirection;

        G4ThreeVector  calorimeterEPRightWorldDirection;

        G4ThreeVector  targetEPWorldPosition;

        G4ThreeVector  targetEPWorldDirection;

        G4double       theAngle;

    private:
        G4int          nCrystalsInColumn;

        G4int          nCrystalsInRow;

        G4double       crystalWidth;

        G4double       crystalHeight;

        G4double       crystalLength;

    private:
        G4AffineTransform  calorimeterLeftTransform;
        
        G4AffineTransform  calorimeterRightTransform;

        G4AffineTransform  targetTransform;

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
            epDefinitionAlgorithm =
                            CexmcEntryPointInTheCenterOfCrystalWithMaxED;
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


inline const G4ThreeVector &
                CexmcReconstructor::GetTargetEPPosition( void ) const
{
    return targetEPPosition;
}


inline const G4ThreeVector &
                CexmcReconstructor::GetTargetEPDirection( void ) const
{
    return targetEPDirection;
}


inline const G4ThreeVector &
        CexmcReconstructor::GetCalorimeterEPLeftWorldPosition( void ) const
{
    return calorimeterEPLeftWorldPosition;
}


inline const G4ThreeVector &
        CexmcReconstructor::GetCalorimeterEPRightWorldPosition( void ) const
{
    return calorimeterEPRightWorldPosition;
}


inline const G4ThreeVector &
        CexmcReconstructor::GetCalorimeterEPLeftWorldDirection( void ) const
{
    return calorimeterEPLeftWorldDirection;
}


inline const G4ThreeVector &
        CexmcReconstructor::GetCalorimeterEPRightWorldDirection( void ) const
{
    return calorimeterEPRightWorldDirection;
}


inline const G4ThreeVector &
        CexmcReconstructor::GetTargetEPWorldPosition( void ) const
{
    return targetEPWorldPosition;
}


inline const G4ThreeVector &
        CexmcReconstructor::GetTargetEPWorldDirection( void ) const
{
    return targetEPWorldDirection;
}


inline G4double  CexmcReconstructor::GetTheAngle( void ) const
{
    return theAngle;
}


inline G4bool  CexmcReconstructor::HasTriggered( void ) const
{
    return hasTriggered;
}


#endif

