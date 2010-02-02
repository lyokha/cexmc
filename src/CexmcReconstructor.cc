/*
 * ============================================================================
 *
 *       Filename:  CexmcReconstructor.cc
 *
 *    Description:  reconstructor base class
 *
 *        Version:  1.0
 *        Created:  02.12.2009 16:21:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include "CexmcReconstructor.hh"
#include "CexmcReconstructorMessenger.hh"
#include "CexmcEnergyDepositStore.hh"
#include "CexmcCalorimeterGeometry.hh"
#include "CexmcTargetGeometry.hh"


CexmcReconstructor::CexmcReconstructor() :
    hasTriggered( false ),
    epDefinitionAlgorithm( CexmcEntryPointBySqrtEDWeights ),
    epDepthDefinitionAlgorithm( CexmcEntryPointDepthPlain ),
    csAlgorithm( CexmcSelectAllCrystals ), epDepth( 0 ), theAngle( 0 ),
    nCrystalsInColumn( 1 ), nCrystalsInRow( 1 ), crystalWidth( 0 ),
    crystalHeight( 0 ), crystalLength( 0 ), messenger( NULL )
{
    CexmcCalorimeterGeometry::GetGeometryData( nCrystalsInColumn,
                nCrystalsInRow, crystalWidth, crystalHeight, crystalLength );

    CexmcCalorimeterGeometry::GetCalorimeterLeftTransform(
                                                    calorimeterLeftTransform );
    CexmcCalorimeterGeometry::GetCalorimeterRightTransform(
                                                    calorimeterRightTransform );
    CexmcTargetGeometry::GetTargetTransform( targetTransform );

    messenger = new CexmcReconstructorMessenger( this );
}


CexmcReconstructor::~CexmcReconstructor()
{
    delete messenger;
}


void  CexmcReconstructor::Reconstruct(
                                    const CexmcEnergyDepositStore * edStore )
{
    ReconstructEntryPoints( edStore );
    if ( hasTriggered )
        ReconstructTargetPoint();
    if ( hasTriggered )
        ReconstructAngle();
}


void  CexmcReconstructor::ReconstructEntryPoints(
                                    const CexmcEnergyDepositStore * edStore )
{
    calorimeterEPLeftPosition.setX( 0 );
    calorimeterEPLeftPosition.setY( 0 );
    calorimeterEPLeftPosition.setZ( -crystalLength / 2 + epDepth );
    calorimeterEPLeftDirection.setX( 0 );
    calorimeterEPLeftDirection.setY( 0 );
    calorimeterEPLeftDirection.setZ( 0 );
    calorimeterEPRightPosition.setX( 0 );
    calorimeterEPRightPosition.setY( 0 );
    calorimeterEPRightPosition.setZ( -crystalLength / 2 + epDepth );
    calorimeterEPRightDirection.setX( 0 );
    calorimeterEPRightDirection.setY( 0 );
    calorimeterEPRightDirection.setZ( 0 );

    G4int     columnLeft( edStore->calorimeterEDLeftMaxX );
    G4int     rowLeft( edStore->calorimeterEDLeftMaxY );
    G4int     columnRight( edStore->calorimeterEDRightMaxX );
    G4int     rowRight( edStore->calorimeterEDRightMaxY );
    G4double  xInCalorimeterLeftOffset(
                ( G4double( columnLeft ) - G4double( nCrystalsInRow ) / 2 ) *
                                        crystalWidth  + crystalWidth / 2 );
    G4double   yInCalorimeterLeftOffset(
                ( G4double( rowLeft ) - G4double( nCrystalsInColumn ) / 2 ) *
                                        crystalHeight + crystalHeight / 2 );
    G4double  xInCalorimeterRightOffset(
                ( G4double( columnRight ) - G4double( nCrystalsInRow ) / 2 ) *
                                        crystalWidth  + crystalWidth / 2 );
    G4double   yInCalorimeterRightOffset(
                ( G4double( rowRight ) - G4double( nCrystalsInColumn ) / 2 ) *
                                        crystalHeight + crystalHeight / 2 );
    G4int     i( 0 );
    G4double  xWeightsSum( 0 );
    G4double  yWeightsSum( 0 );
    G4double  energySum( 0 );

    switch ( epDefinitionAlgorithm )
    {
    case CexmcEntryPointInTheCenter :
        break;
    case CexmcEntryPointInTheCenterOfCrystalWithMaxED :
        calorimeterEPLeftPosition.setX( xInCalorimeterLeftOffset );
        calorimeterEPLeftPosition.setY( yInCalorimeterLeftOffset );
        calorimeterEPRightPosition.setX( xInCalorimeterRightOffset );
        calorimeterEPRightPosition.setY( yInCalorimeterRightOffset );
        break;
    case CexmcEntryPointByLinearEDWeights :
        for ( CexmcEnergyDepositCalorimeterCollection::const_iterator
                  k( edStore->calorimeterEDLeftCollection.begin() );
                  k != edStore->calorimeterEDLeftCollection.end(); ++k )
        {
            if ( csAlgorithm == CexmcSelectAdjacentCrystals &&
                 ( i - rowLeft > 1 || i - rowLeft < -1 ) )
            {
                ++i;
                continue;
            }

            G4int  j( 0 );
            for ( CexmcEnergyDepositCrystalRowCollection::const_iterator
                      l( k->begin() ); l != k->end(); ++l )
            {
                if ( csAlgorithm == CexmcSelectAdjacentCrystals &&
                     ( j - columnLeft > 1 || j - columnLeft < -1 ) )
                {
                    ++j;
                    continue;
                }

                xInCalorimeterLeftOffset = ( G4double( j ) -
                                        G4double( nCrystalsInRow ) / 2 ) *
                        crystalWidth  + crystalWidth / 2;
                xWeightsSum += *l * xInCalorimeterLeftOffset;
                yInCalorimeterLeftOffset = ( G4double( i ) -
                                        G4double( nCrystalsInColumn ) / 2 ) *
                        crystalHeight  + crystalHeight / 2;
                yWeightsSum += *l * yInCalorimeterLeftOffset;
                energySum += *l;
                ++j;
            }
            ++i;
        }
        calorimeterEPLeftPosition.setX( xWeightsSum / energySum );
        calorimeterEPLeftPosition.setY( yWeightsSum / energySum );
        i = 0;
        xWeightsSum = 0;
        yWeightsSum = 0;
        energySum = 0;
        for ( CexmcEnergyDepositCalorimeterCollection::const_iterator
                  k( edStore->calorimeterEDRightCollection.begin() );
                  k != edStore->calorimeterEDRightCollection.end(); ++k )
        {
            if ( csAlgorithm == CexmcSelectAdjacentCrystals &&
                 ( i - rowRight > 1 || i - rowRight < -1 ) )
            {
                ++i;
                continue;
            }

            G4int  j( 0 );
            for ( CexmcEnergyDepositCrystalRowCollection::const_iterator
                      l( k->begin() ); l != k->end(); ++l )
            {
                if ( csAlgorithm == CexmcSelectAdjacentCrystals &&
                     ( j - columnRight > 1 || j - columnRight < -1 ) )
                {
                    ++j;
                    continue;
                }

                xInCalorimeterRightOffset = ( G4double( j ) -
                                        G4double( nCrystalsInRow ) / 2 ) *
                        crystalWidth  + crystalWidth / 2;
                xWeightsSum += *l * xInCalorimeterRightOffset;
                yInCalorimeterRightOffset = ( G4double( i ) -
                                        G4double( nCrystalsInColumn ) / 2 ) *
                        crystalHeight  + crystalHeight / 2;
                yWeightsSum += *l * yInCalorimeterRightOffset;
                energySum += *l;
                ++j;
            }
            ++i;
        }
        calorimeterEPRightPosition.setX( xWeightsSum / energySum );
        calorimeterEPRightPosition.setY( yWeightsSum / energySum );
        break;
    case CexmcEntryPointBySqrtEDWeights :
        for ( CexmcEnergyDepositCalorimeterCollection::const_iterator
                  k( edStore->calorimeterEDLeftCollection.begin() );
                  k != edStore->calorimeterEDLeftCollection.end(); ++k )
        {
            if ( csAlgorithm == CexmcSelectAdjacentCrystals &&
                 ( i - rowLeft > 1 || i - rowLeft < -1 ) )
            {
                ++i;
                continue;
            }

            G4int  j( 0 );
            for ( CexmcEnergyDepositCrystalRowCollection::const_iterator
                      l( k->begin() ); l != k->end(); ++l )
            {
                if ( csAlgorithm == CexmcSelectAdjacentCrystals &&
                     ( j - columnLeft > 1 || j - columnLeft < -1 ) )
                {
                    ++j;
                    continue;
                }

                xInCalorimeterLeftOffset = ( G4double( j ) -
                                        G4double( nCrystalsInRow ) / 2 ) *
                        crystalWidth  + crystalWidth / 2;
                xWeightsSum += std::sqrt( *l ) * xInCalorimeterLeftOffset;
                yInCalorimeterLeftOffset = ( G4double( i ) -
                                        G4double( nCrystalsInColumn ) / 2 ) *
                        crystalHeight  + crystalHeight / 2;
                yWeightsSum += std::sqrt( *l ) * yInCalorimeterLeftOffset;
                energySum += std::sqrt( *l );
                ++j;
            }
            ++i;
        }
        calorimeterEPLeftPosition.setX( xWeightsSum / energySum );
        calorimeterEPLeftPosition.setY( yWeightsSum / energySum );
        i = 0;
        xWeightsSum = 0;
        yWeightsSum = 0;
        energySum = 0;
        for ( CexmcEnergyDepositCalorimeterCollection::const_iterator
                  k( edStore->calorimeterEDRightCollection.begin() );
                  k != edStore->calorimeterEDRightCollection.end(); ++k )
        {
            if ( csAlgorithm == CexmcSelectAdjacentCrystals &&
                 ( i - rowRight > 1 || i - rowRight < -1 ) )
            {
                ++i;
                continue;
            }

            G4int  j( 0 );
            for ( CexmcEnergyDepositCrystalRowCollection::const_iterator
                      l( k->begin() ); l != k->end(); ++l )
            {
                if ( csAlgorithm == CexmcSelectAdjacentCrystals &&
                     ( j - columnRight > 1 || j - columnRight < -1 ) )
                {
                    ++j;
                    continue;
                }

                xInCalorimeterRightOffset = ( G4double( j ) -
                                        G4double( nCrystalsInRow ) / 2 ) *
                        crystalWidth  + crystalWidth / 2;
                xWeightsSum += std::sqrt( *l ) * xInCalorimeterRightOffset;
                yInCalorimeterRightOffset = ( G4double( i ) -
                                        G4double( nCrystalsInColumn ) / 2 ) *
                        crystalHeight  + crystalHeight / 2;
                yWeightsSum += std::sqrt( *l ) * yInCalorimeterRightOffset;
                energySum += std::sqrt( *l );
                ++j;
            }
            ++i;
        }
        calorimeterEPRightPosition.setX( xWeightsSum / energySum );
        calorimeterEPRightPosition.setY( yWeightsSum / energySum );
        break;
    default :
        break;
    }

    switch ( epDepthDefinitionAlgorithm )
    {
    case CexmcEntryPointDepthPlain :
        break;
    case CexmcEntryPointDepthSphere :
        {
            G4double  calorimeterEPLeftRadiusOfTheSphere(
                             calorimeterLeftTransform.NetTranslation().mag() +
                             calorimeterEPLeftPosition.z() );
            G4double  calorimeterEPLeftRadiusOfTheSphere2(
                                      calorimeterEPLeftRadiusOfTheSphere *
                                      calorimeterEPLeftRadiusOfTheSphere );
            G4double  calorimeterEPLeftPositionX2(
                                            calorimeterEPLeftPosition.x() *
                                            calorimeterEPLeftPosition.x() );
            G4double  calorimeterEPLeftPositionY2(
                                            calorimeterEPLeftPosition.y() *
                                            calorimeterEPLeftPosition.y() );
            G4double  calorimeterEPLeftPositionZOffset(
                           calorimeterEPLeftRadiusOfTheSphere - std::sqrt(
                                  calorimeterEPLeftRadiusOfTheSphere2 -
                                  calorimeterEPLeftPositionX2 -
                                  calorimeterEPLeftPositionY2 ) );
            G4double  calorimeterEPRightRadiusOfTheSphere(
                              calorimeterRightTransform.NetTranslation().mag() +
                              calorimeterEPRightPosition.z() );
            G4double  calorimeterEPRightRadiusOfTheSphere2(
                                       calorimeterEPRightRadiusOfTheSphere *
                                       calorimeterEPRightRadiusOfTheSphere );
            G4double  calorimeterEPRightPositionX2(
                                            calorimeterEPRightPosition.x() *
                                            calorimeterEPRightPosition.x() );
            G4double  calorimeterEPRightPositionY2(
                                            calorimeterEPRightPosition.y() *
                                            calorimeterEPRightPosition.y() );
            G4double  calorimeterEPRightPositionZOffset(
                            calorimeterEPRightRadiusOfTheSphere - std::sqrt(
                                    calorimeterEPRightRadiusOfTheSphere2 -
                                    calorimeterEPRightPositionX2 -
                                    calorimeterEPRightPositionY2 ) );
            calorimeterEPLeftPosition.setZ( calorimeterEPLeftPosition.z() -
                                            calorimeterEPLeftPositionZOffset );
            calorimeterEPRightPosition.setZ( calorimeterEPRightPosition.z() -
                                         calorimeterEPRightPositionZOffset );
        }
        break;
    }

    calorimeterEPLeftWorldPosition = calorimeterLeftTransform.TransformPoint(
                                                 calorimeterEPLeftPosition );
    calorimeterEPLeftWorldDirection = calorimeterLeftTransform.TransformAxis(
                                                 calorimeterEPLeftDirection );
    calorimeterEPRightWorldPosition = calorimeterRightTransform.TransformPoint(
                                                 calorimeterEPRightPosition );
    calorimeterEPRightWorldDirection = calorimeterRightTransform.TransformAxis(
                                                 calorimeterEPRightDirection );

    hasTriggered = true;
}


void  CexmcReconstructor::ReconstructTargetPoint( void )
{
    targetEPWorldPosition = targetTransform.TransformPoint(
                                                    G4ThreeVector( 0, 0, 0 ) );
    targetEPWorldDirection.setX( 0 );
    targetEPWorldDirection.setY( 0 );
    targetEPWorldDirection.setZ( 0 );

    targetEPPosition = targetTransform.Inverse().TransformPoint(
                                                    targetEPWorldPosition );
    targetEPDirection = targetTransform.Inverse().TransformAxis(
                                                    targetEPWorldDirection );

    hasTriggered = true;
}


void  CexmcReconstructor::ReconstructAngle( void )
{
    G4ThreeVector  epLeft( calorimeterEPLeftWorldPosition -
                           targetEPWorldPosition );
    G4ThreeVector  epRight( calorimeterEPRightWorldPosition -
                            targetEPWorldPosition );
    theAngle = epLeft.angle( epRight );

    hasTriggered = true;
}

