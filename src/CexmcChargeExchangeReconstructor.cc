/*
 * ============================================================================
 *
 *       Filename:  CexmcChargeExchangeReconstructor.cc
 *
 *    Description:  charge exchange reconstructor
 *
 *        Version:  1.0
 *        Created:  02.12.2009 15:17:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4UnitsTable.hh>
#include "CexmcChargeExchangeReconstructor.hh"
#include "CexmcCalorimeterGeometry.hh"
#include "CexmcTargetGeometry.hh"


CexmcChargeExchangeReconstructor::CexmcChargeExchangeReconstructor()
{
    CexmcCalorimeterGeometry::GetCalorimeterLeftTransform(
                                                    calorimeterLeftTransform );
    CexmcCalorimeterGeometry::GetCalorimeterRightTransform(
                                                    calorimeterRightTransform );
    CexmcTargetGeometry::GetTargetTransform( targetTransform );
}


void  CexmcChargeExchangeReconstructor::Reconstruct(
                                    const CexmcEnergyDepositStore *  edStore )
{
    ReconstructEntryPoints( edStore );

    G4ThreeVector  calorimeterEPLeftPositionInWorld(
                        calorimeterLeftTransform.TransformPoint(
                                                calorimeterEPLeftPosition ) );
    G4ThreeVector  calorimeterEPRightPositionInWorld(
                        calorimeterRightTransform.TransformPoint(
                                                calorimeterEPRightPosition ) );
    calorimeterRightTransform.TransformPoint( calorimeterEPRightPosition );
    G4cout << " LEFT POINT: " << G4BestUnit( calorimeterEPLeftPositionInWorld,
                                             "Length" ) << G4endl;
    G4cout << "RIGHT POINT: " << G4BestUnit( calorimeterEPRightPositionInWorld,
                                             "Length" ) << G4endl;
}

