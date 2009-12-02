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


CexmcReconstructor::CexmcReconstructor() :
    epdAlgorithm( CexmcEntryPointBySqrtEDWeights ),
    csAlgorithm( CexmcSelectAllCrystals ), messenger( NULL )
{
    messenger = new CexmcReconstructorMessenger( this );
}


CexmcReconstructor::~CexmcReconstructor()
{
    delete messenger;
}


void  CexmcReconstructor::Reconstruct(
                        const CexmcEnergyDepositCalorimeterCollection &,
                        const CexmcEnergyDepositCalorimeterCollection & )
{
}


void  CexmcReconstructor::ReconstructEntryPoints( void )
{
    calorimeterEPLeftPosition.setX( 0 );
    calorimeterEPLeftPosition.setY( 0 );
    calorimeterEPLeftPosition.setZ( 0 );
    calorimeterEPLeftDirection.setX( 0 );
    calorimeterEPLeftDirection.setY( 0 );
    calorimeterEPLeftDirection.setZ( 0 );

    switch ( epdAlgorithm )
    {
    case CexmcEntryPointInTheCenter :
        break;
    default :
        break;
    }
}

