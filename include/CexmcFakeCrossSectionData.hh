/*
 * =============================================================================
 *
 *       Filename:  CexmcFakeCrossSectionData.hh
 *
 *    Description:  fake cross section data (to be never really applied)
 *
 *        Version:  1.0
 *        Created:  18.09.2011 21:47:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_FAKE_CROSS_SECTION_DATA_HH
#define CEXMC_FAKE_CROSS_SECTION_DATA_HH

#include <G4VCrossSectionDataSet.hh>

class  G4DynamicParticle;
class  G4Element;
class  G4ParticleDefinition;


class  CexmcFakeCrossSectionData : public G4VCrossSectionDataSet
{
    public:
        G4bool    IsApplicable( const G4DynamicParticle *, const G4Element * )
        {
            return false;
        }

        G4double  GetCrossSection( const G4DynamicParticle *,
                                   const G4Element *, G4double )
        {
            return 0;
        }

        void      BuildPhysicsTable( const G4ParticleDefinition & )
        {}

        void      DumpPhysicsTable( const G4ParticleDefinition & )
        {}
};


#endif

