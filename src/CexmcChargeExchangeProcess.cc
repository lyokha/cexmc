/*
 * ============================================================================
 *
 *       Filename:  CexmcChargeExchangeProcess.cc
 *
 *    Description:  charge exchange process
 *
 *        Version:  1.0
 *        Created:  31.10.2009 23:54:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4HadronElasticDataSet.hh>
#include "CexmcChargeExchangeProcess.hh"


CexmcChargeExchangeProcess::CexmcChargeExchangeProcess(
                                                    const G4String &  name ) :
    G4HadronicProcess( name )
{
    AddDataSet( new G4HadronElasticDataSet );
}

