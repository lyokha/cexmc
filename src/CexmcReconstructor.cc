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
#include "CexmcRunManager.hh"


CexmcReconstructor::CexmcReconstructor() :
    hasBasicTrigger( false ),
    epDefinitionAlgorithm( CexmcEntryPointBySqrtEDWeights ),
    epDepthDefinitionAlgorithm( CexmcEntryPointDepthPlain ),
    csAlgorithm( CexmcSelectAllCrystals ), useInnerRefCrystal( false ),
    epDepth( 0 ), theAngle( 0 ), targetEPInitialized( false ), messenger( NULL )
{
    G4RunManager *      runManager( G4RunManager::GetRunManager() );
    const CexmcSetup *  setup( static_cast< const CexmcSetup * >(
                                runManager->GetUserDetectorConstruction() ) );
    calorimeterGeometry = setup->GetCalorimeterGeometry();
    targetTransform = setup->GetTargetTransform();
    calorimeterLeftTransform = setup->GetCalorimeterLeftTransform();
    calorimeterRightTransform = setup->GetCalorimeterRightTransform();

    messenger = new CexmcReconstructorMessenger( this );
}


CexmcReconstructor::~CexmcReconstructor()
{
    delete messenger;
}


void  CexmcReconstructor::Reconstruct(
                                    const CexmcEnergyDepositStore *  edStore )
{
    ReconstructEntryPoints( edStore );
    if ( hasBasicTrigger )
        ReconstructTargetPoint();
    if ( hasBasicTrigger )
        ReconstructAngle();
}


G4bool  CexmcReconstructor::HasFullTrigger( void ) const
{
    return hasBasicTrigger;
}


void  CexmcReconstructor::ReconstructEntryPoints(
                                    const CexmcEnergyDepositStore *  edStore )
{
    G4int     nCrystalsInColumn( calorimeterGeometry.nCrystalsInColumn );
    G4int     nCrystalsInRow( calorimeterGeometry.nCrystalsInRow );
    G4double  crystalWidth( calorimeterGeometry.crystalWidth );
    G4double  crystalHeight( calorimeterGeometry.crystalHeight );
    G4double  crystalLength( calorimeterGeometry.crystalLength );

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
    G4double  yInCalorimeterLeftOffset(
                ( G4double( rowLeft ) - G4double( nCrystalsInColumn ) / 2 ) *
                                        crystalHeight + crystalHeight / 2 );
    G4double  xInCalorimeterRightOffset(
                ( G4double( columnRight ) - G4double( nCrystalsInRow ) / 2 ) *
                                        crystalWidth  + crystalWidth / 2 );
    G4double  yInCalorimeterRightOffset(
                ( G4double( rowRight ) - G4double( nCrystalsInColumn ) / 2 ) *
                                        crystalHeight + crystalHeight / 2 );
    G4double  x( 0 );
    G4double  y( 0 );

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
        CalculateWeightedEPPosition( edStore->calorimeterEDLeftCollection,
                                     rowLeft, columnLeft, false, x, y );
        calorimeterEPLeftPosition.setX( x );
        calorimeterEPLeftPosition.setY( y );
        CalculateWeightedEPPosition( edStore->calorimeterEDRightCollection,
                                     rowRight, columnRight, false, x, y );
        calorimeterEPRightPosition.setX( x );
        calorimeterEPRightPosition.setY( y );
        break;
    case CexmcEntryPointBySqrtEDWeights :
        CalculateWeightedEPPosition( edStore->calorimeterEDLeftCollection,
                                     rowLeft, columnLeft, true, x, y );
        calorimeterEPLeftPosition.setX( x );
        calorimeterEPLeftPosition.setY( y );
        CalculateWeightedEPPosition( edStore->calorimeterEDRightCollection,
                                     rowRight, columnRight, true, x, y );
        calorimeterEPRightPosition.setX( x );
        calorimeterEPRightPosition.setY( y );
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

    hasBasicTrigger = true;
}


void  CexmcReconstructor::ReconstructTargetPoint( void )
{
    if ( ! targetEPInitialized )
    {
        targetEPWorldPosition = targetTransform.TransformPoint(
                                                    G4ThreeVector( 0, 0, 0 ) );
        targetEPWorldDirection.setX( 0 );
        targetEPWorldDirection.setY( 0 );
        targetEPWorldDirection.setZ( 1 );

        targetEPPosition = targetTransform.Inverse().TransformPoint(
                                                    targetEPWorldPosition );
        targetEPDirection = targetTransform.Inverse().TransformAxis(
                                                    targetEPWorldDirection );
        targetEPInitialized = true;
    }

    hasBasicTrigger = true;
}


void  CexmcReconstructor::ReconstructAngle( void )
{
    G4ThreeVector  epLeft( calorimeterEPLeftWorldPosition -
                           targetEPWorldPosition );
    G4ThreeVector  epRight( calorimeterEPRightWorldPosition -
                            targetEPWorldPosition );
    theAngle = epLeft.angle( epRight );

    hasBasicTrigger = true;
}


void  CexmcReconstructor::CalculateWeightedEPPosition(
                        const CexmcEnergyDepositCalorimeterCollection &  edHits,
                        G4int  row, G4int  column, G4bool  isSqrtWeight,
                        G4double &  x, G4double &  y ) const
{
    G4int     nCrystalsInColumn( calorimeterGeometry.nCrystalsInColumn );
    G4int     nCrystalsInRow( calorimeterGeometry.nCrystalsInRow );
    G4double  crystalWidth( calorimeterGeometry.crystalWidth );
    G4double  crystalHeight( calorimeterGeometry.crystalHeight );

    G4int     i( 0 );
    G4double  xWeightsSum( 0 );
    G4double  yWeightsSum( 0 );
    G4double  energyWeightsSum( 0 );

    for ( CexmcEnergyDepositCalorimeterCollection::const_iterator
                                k( edHits.begin() ); k != edHits.end(); ++k )
    {
        if ( csAlgorithm == CexmcSelectAdjacentCrystals &&
                                         ( i - row > 1 || i - row < -1 ) )
        {
            ++i;
            continue;
        }

        G4int  j( 0 );
        for ( CexmcEnergyDepositCrystalRowCollection::const_iterator
                  l( k->begin() ); l != k->end(); ++l )
        {
            if ( csAlgorithm == CexmcSelectAdjacentCrystals &&
                                         ( j - column > 1 || j - column < -1 ) )
            {
                ++j;
                continue;
            }
            
            G4double  xInCalorimeterOffset(
                        ( G4double( j ) - G4double( nCrystalsInRow ) / 2 ) *
                        crystalWidth  + crystalWidth / 2 );
            G4double  energyWeight( isSqrtWeight ? std::sqrt( *l ) : *l );
            xWeightsSum += energyWeight * xInCalorimeterOffset;
            G4double  yInCalorimeterOffset(
                        ( G4double( i ) - G4double( nCrystalsInColumn ) / 2 ) *
                        crystalHeight  + crystalHeight / 2 );
            yWeightsSum += energyWeight * yInCalorimeterOffset;
            energyWeightsSum += energyWeight;
            ++j;
        }
        ++i;
    }

    x = xWeightsSum / energyWeightsSum;
    y = yWeightsSum / energyWeightsSum;
}

