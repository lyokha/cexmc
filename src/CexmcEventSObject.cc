/*
 * ============================================================================
 *
 *       Filename:  CexmcEventSObject.cc
 *
 *    Description:  event data serialization helper
 *
 *        Version:  1.0
 *        Created:  30.12.2009 17:10:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include "CexmcEventSObject.hh"


CexmcEventSObject::CexmcEventSObject()
{
}


CexmcEventSObject::CexmcEventSObject( G4double  monitorED,
        G4double  vetoCounterEDLeft, G4double  vetoCounterEDRight,
        G4double  calorimeterEDLeft, G4double  calorimeterEDRight,
        const CexmcEnergyDepositCalorimeterCollection &
                                                calorimeterEDLeftCollection,
        const CexmcEnergyDepositCalorimeterCollection &
                                                calorimeterEDRightCollection ) :
    monitorED( monitorED ), vetoCounterEDLeft( vetoCounterEDLeft ),
    vetoCounterEDRight( vetoCounterEDRight ),
    calorimeterEDLeft( calorimeterEDLeft ),
    calorimeterEDRight( calorimeterEDRight ),
    calorimeterEDLeftCollection( calorimeterEDLeftCollection ),
    calorimeterEDRightCollection( calorimeterEDRightCollection )
{
}

