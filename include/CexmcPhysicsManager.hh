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

class  G4ParticleDefinition;
class  G4Track;
class  G4StepPoint;
class  CexmcProductionModel;


class  CexmcPhysicsManager
{
    public:
        CexmcPhysicsManager();

        virtual ~CexmcPhysicsManager();

    public:
        virtual CexmcProductionModel *  GetProductionModel( void ) = 0;

        virtual G4double                GetProposedMaxIL( G4double  dimension );

        virtual G4bool  IsStudiedProcessAllowed( void ) const = 0;

        virtual void    ResampleTrackLengthInTarget( const G4Track *  track,
                                    const G4StepPoint *  stepPoint = NULL ) = 0;

    public:
        G4bool  OnlyBeamParticleCanTriggerStudiedProcess( void ) const;

        void    IncrementNumberOfTriggeredStudiedInteractions( void );

        void    ResetNumberOfTriggeredStudiedInteractions( void );

    protected:
        G4int   numberOfTriggeredStudiedInteractions;

    private:
        G4bool  onlyBeamParticleCanTriggerStudiedProcess;
};


inline G4bool  CexmcPhysicsManager::OnlyBeamParticleCanTriggerStudiedProcess(
                                                                    void ) const
{
    return onlyBeamParticleCanTriggerStudiedProcess;
}


inline void  CexmcPhysicsManager::IncrementNumberOfTriggeredStudiedInteractions(
                                                                        void )
{
    ++numberOfTriggeredStudiedInteractions;
}


inline void  CexmcPhysicsManager::ResetNumberOfTriggeredStudiedInteractions(
                                                                        void )
{
    numberOfTriggeredStudiedInteractions = 0;
}


#endif

