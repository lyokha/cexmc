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

#include "CexmcChargeExchangeReconstructor.hh"


CexmcChargeExchangeReconstructor::CexmcChargeExchangeReconstructor()
{
}


void  CexmcChargeExchangeReconstructor::Reconstruct(
                                    const CexmcEnergyDepositStore *  edStore )
{
    ReconstructEntryPoints( edStore );
    ReconstructTargetPoint();
}

