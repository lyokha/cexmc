/*
 * ============================================================================
 *
 *       Filename:  CexmcPhysicsManager.cc
 *
 *    Description:  interface for external access to physics aspects
 *
 *        Version:  1.0
 *        Created:  03.11.2009 14:22:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include "CexmcPhysicsManager.hh"

CexmcPhysicsManager::CexmcPhysicsManager() :
    numberOfTriggeredStudiedInteractions( 0 ),
    onlyIncidentParticleCanTriggerStudiedProcess( false )
{
}


CexmcPhysicsManager::~CexmcPhysicsManager()
{
}


G4double  CexmcPhysicsManager::GetProposedMaxIL( G4double  dimension )
{
    return dimension;
}

