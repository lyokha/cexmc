/*
 * =============================================================================
 *
 *       Filename:  CexmcBasicPhysicsSettings.hh
 *
 *    Description:  basic typedefs etc. to build studied physics
 *
 *        Version:  1.0
 *        Created:  28.11.2009 15:30:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_BASIC_PHYSICS_SETTINGS_HH
#define CEXMC_BASIC_PHYSICS_SETTINGS_HH

#include <QGSP_BERT.hh>
#include <G4PionMinus.hh>
#include "CexmcProductionModelFactory.hh"
#include "CexmcStudiedPhysicsChargeExchange.hh"


typedef QGSP_BERT    CexmcBasePhysics;
typedef G4PionMinus  CexmcStudiedPhysicsStarterParticle;
typedef CexmcProductionModelFactory< CexmcBasePhysics,
                                     CexmcStudiedPhysicsStarterParticle,
                                     CexmcStudiedPhysicsChargeExchange >
                     CexmcChargeExchangePMFactory;


#endif
