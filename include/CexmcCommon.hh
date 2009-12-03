/*
 * =============================================================================
 *
 *       Filename:  CexmcCommon.hh
 *
 *    Description:  common declarations
 *
 *        Version:  1.0
 *        Created:  01.11.2009 00:09:04
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_COMMON_HH
#define CEXMC_COMMON_HH

#include <G4String.hh>
#include <G4Types.hh>

#define CEXMC_LINE_START  "--- Cexmc ---  "


const G4String  CexmcStudiedProcessFirstName( "studiedPhysics_" );
const G4String  CexmcStudiedProcessLastName( "studiedChargeExchange" );
const G4String  CexmcStudiedProcessFullName( CexmcStudiedProcessFirstName +
                                             CexmcStudiedProcessLastName );

const G4double  CexmcFwhmToStddev( 0.42466 );


enum  CexmcProductionModelType
{
    CexmcUnknownProductionModel,
    CexmcPionZeroProduction,
    CexmcEtaProduction
};


enum  CexmcTrackType
{
    CexmcInsipidTrack,
    CexmcIncidentParticleTrack,
    CexmcOutputParticleTrack,
    CexmcNucleusParticleTrack,
    CexmcOutputParticleDecayProductTrack
};


enum  CexmcSide
{
    CexmcLeft,
    CexmcRight
};


#endif

