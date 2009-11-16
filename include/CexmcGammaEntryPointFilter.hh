/*
 * =============================================================================
 *
 *       Filename:  CexmcGammaEntryPointFilter.hh
 *
 *    Description:  gamma entry points on veto counters and calorimeters
 *
 *        Version:  1.0
 *        Created:  16.11.2009 22:23:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_GAMMA_ENTRY_POINT_FILTER_HH
#define CEXMC_GAMMA_ENTRY_POINT_FILTER_HH

#include <G4VSDFilter.hh>

class  G4String;
class  G4Step;


class  CexmcGammaEntryPointFilter : public G4VSDFilter
{
    public:
        explicit CexmcGammaEntryPointFilter( const G4String &  name );

    public:
        G4bool  Accept( const G4Step *  step ) const;
};


#endif

