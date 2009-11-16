/*
 * =============================================================================
 *
 *       Filename:  CexmcOriginalParticleEntryPointFilter.hh
 *
 *    Description:  original particle entry point
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

#ifndef CEXMC_ORIGINAL_PARTICLE_ENTRY_POINT_FILTER_HH
#define CEXMC_ORIGINAL_PARTICLE_ENTRY_POINT_FILTER_HH

#include <G4VSDFilter.hh>

class  G4String;
class  G4Step;


class  CexmcOriginalParticleEntryPointFilter : public G4VSDFilter
{
    public:
        explicit CexmcOriginalParticleEntryPointFilter(
                                                    const G4String &  name );

    public:
        G4bool  Accept( const G4Step *  step ) const;
};


#endif

