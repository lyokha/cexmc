/*
 * ============================================================================
 *
 *       Filename:  CexmcProductionModel.cc
 *
 *    Description:  interface for production model
 *
 *        Version:  1.0
 *        Created:  03.11.2009 16:56:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include "CexmcProductionModel.hh"
#include "CexmcProductionModelMessenger.hh"


CexmcProductionModel::CexmcProductionModel( const G4String &  name,
                                            G4bool  fermiMotionIsOn ) :
    name( name ), fermiMotionIsOn( fermiMotionIsOn ), incidentParticle( NULL ),
    nucleusParticle( NULL ), outputParticle( NULL ),
    nucleusOutputParticle( NULL ), messenger( NULL )
{
    G4cout << CEXMC_LINE_START << "Production model '" << name <<
              "' instantiated" << G4endl;

    angularRanges.push_back( CexmcAngularRange( 1.0, -1.0, 0 ) );
    messenger = new CexmcProductionModelMessenger( this );
}


CexmcProductionModel::~CexmcProductionModel()
{
    delete messenger;
}


void  CexmcProductionModel::SetTriggeredAngularRanges( G4double  opCosThetaSCM )
{
    triggeredAngularRanges.clear();

    for ( CexmcAngularRangeList::iterator  k( angularRanges.begin() );
                                            k != angularRanges.end(); ++k )
    {
        if ( opCosThetaSCM <= k->top && opCosThetaSCM > k->bottom )
            triggeredAngularRanges.push_back( CexmcAngularRange(
                                            k->top, k->bottom, k->index ) );
    }
}


void  CexmcProductionModel::FermiMotionStatusChangeHook( void )
{
}

