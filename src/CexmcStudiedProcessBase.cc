/*
 * =============================================================================
 *
 *       Filename:  CexmcStudiedProcessBase.cc
 *
 *    Description:  basic interface for Studied Process
 *
 *        Version:  1.0
 *        Created:  12.05.2010 23:22:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#include "CexmcStudiedProcessBase.hh"
#include "CexmcCommon.hh"


CexmcStudiedProcessBase::CexmcStudiedProcessBase() :
        G4WrapperProcess( CexmcStudiedProcessFirstName, fUserDefined ),
        numberOfTriggeredEvents( 0 )
{
}

