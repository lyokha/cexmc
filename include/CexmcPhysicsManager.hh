/*
 * =============================================================================
 *
 *       Filename:  CexmcPhysicsManager.hh
 *
 *    Description:  interface for external access to physics aspects
 *
 *        Version:  1.0
 *        Created:  27.10.2009 23:10:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_PHYSICS_MANAGER_HH
#define CEXMC_PHYSICS_MANAGER_HH

#include <G4Types.hh>
#include <templates.hh>

class G4ParticleDefinition;


class  CexmcPhysicsManager
{
    public:
        CexmcPhysicsManager();

        virtual ~CexmcPhysicsManager();

    public:
        virtual const G4ParticleDefinition *
                        GetIncidentParticleType( void ) const = 0;

        virtual void  ActivateStudiedProcess( G4bool  on,
                                              G4double  maxStep = DBL_MAX ) = 0;

        virtual void  CalculateProposedMaxIL( G4double  targetRadius );

    protected:
        G4double  proposedMaxIL;
};


#endif

