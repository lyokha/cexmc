/*
 * ============================================================================
 *
 *       Filename:  CexmcRunManager.cc
 *
 *    Description:  run manager
 *
 *        Version:  1.0
 *        Created:  03.11.2009 20:27:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include "CexmcRunManager.hh"
#include "CexmcRunManagerMessenger.hh"


CexmcRunManager::CexmcRunManager() :
    productionModelType( CexmcUnknownProductionModel ),
    gdmlFileName( "default.gdml" ), saveResults( false ), resultsDir( "." ),
    runId( "trash" ), messenger( NULL )
{
    messenger = new CexmcRunManagerMessenger( this );
}


CexmcRunManager::~CexmcRunManager()
{
    delete messenger;
}

