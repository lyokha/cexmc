/*
 * =============================================================================
 *
 *       Filename:  CexmcPhysicsList.hh
 *
 *    Description:  mandatory physics list
 *
 *        Version:  1.0
 *        Created:  11.10.2009 14:51:08
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_PHYSICS_LIST_HH
#define CEXMC_PHYSICS_LIST_HH

#include <Randomize.hh>
#include <G4VModularPhysicsList.hh>
#include <G4Track.hh>
#include <G4StepPoint.hh>
#include <G4VPhysicalVolume.hh>
#include <G4Tubs.hh>
#include <G4AffineTransform.hh>
#include <G4NavigationHistory.hh>
#include "CexmcStudiedPhysics.hh"
#include "CexmcStudiedProcess.hh"
#include "CexmcPhysicsManager.hh"
#include "CexmcProductionModel.hh"
#include "CexmcIncidentParticleTrackInfo.hh"
#include "CexmcCommon.hh"


template  < typename  BasePhysics, typename  Particle,
            template  < typename, typename > class  StudiedPhysics,
            typename  ProductionModel >
class  CexmcPhysicsList : public BasePhysics, public CexmcPhysicsManager
{
    public:
        CexmcPhysicsList();

    public:
        const G4ParticleDefinition *  GetIncidentParticleType( void ) const;

        CexmcProductionModel *        GetProductionModel( void );

        G4bool                        IsStudiedProcessAllowed( void ) const;

        void                          ResampleTrackLengthInTarget(
                                            const G4Track *  track,
                                            const G4StepPoint *  stepPoint );

    private:
        CexmcProductionModel *  productionModel;

        G4bool                  proposedMaxILInitialized;

        G4double                proposedMaxIL;
};


template  < typename  BasePhysics, typename  Particle,
            template  < typename, typename > class  StudiedPhysics,
            typename  ProductionModel >
CexmcPhysicsList< BasePhysics, Particle, StudiedPhysics, ProductionModel >::
                CexmcPhysicsList() :
    productionModel( NULL ), proposedMaxILInitialized( false ),
    proposedMaxIL( CexmcDblMax )
{
    this->RegisterPhysics( new StudiedPhysics< Particle, ProductionModel >(
                                                                    this ) );
}


template  < typename  BasePhysics, typename  Particle,
            template  < typename, typename > class  StudiedPhysics,
            typename  ProductionModel >
inline const G4ParticleDefinition *
    CexmcPhysicsList< BasePhysics, Particle, StudiedPhysics, ProductionModel >::
                GetIncidentParticleType( void ) const
{
    return Particle::Definition();
}


template  < typename  BasePhysics, typename  Particle,
            template  < typename, typename > class  StudiedPhysics,
            typename  ProductionModel >
CexmcProductionModel *  CexmcPhysicsList< BasePhysics, Particle,
                                           StudiedPhysics, ProductionModel >::
                GetProductionModel( void )
{
    if ( productionModel )
        return productionModel;

    G4ParticleDefinition *  particle( Particle::Definition() );
    G4ProcessManager *      processManager( particle->GetProcessManager() );
    G4ProcessVector *       processVector( processManager->GetProcessList() );
    G4int                   processVectorSize(
                                    processManager->GetProcessListLength() );

    for ( int  i( 0 ); i < processVectorSize; ++i )
    {
        if ( ( *processVector )[ i ]->GetProcessName() ==
                                                CexmcStudiedProcessFullName )
        {
            G4WrapperProcess *  studiedProcess(
                                    static_cast< G4WrapperProcess * >(
                                                    ( *processVector )[ i ] ) );
            if ( ! studiedProcess )
                break;

            G4VProcess *  process( const_cast< G4VProcess * >(
                                    studiedProcess->GetRegisteredProcess() ) );
            if ( process )
                productionModel =
                        static_cast< typename StudiedPhysics< Particle,
                            ProductionModel >::ProcessType * >( process )->
                                                        GetProductionModel();
            break;
        }
    }

    return productionModel;
}


template  < typename  BasePhysics, typename  Particle,
            template  < typename, typename > class  StudiedPhysics,
            typename  ProductionModel >
G4bool  CexmcPhysicsList< BasePhysics, Particle, StudiedPhysics,
                        ProductionModel >::
                IsStudiedProcessAllowed( void ) const
{
    return numberOfTriggeredStudiedInteractions == 0;
}


template  < typename  BasePhysics, typename  Particle,
            template  < typename, typename > class  StudiedPhysics,
            typename  ProductionModel >
void  CexmcPhysicsList< BasePhysics, Particle, StudiedPhysics,
                        ProductionModel >::
                ResampleTrackLengthInTarget( const G4Track *  track,
                                             const G4StepPoint *  stepPoint )
{
    CexmcIncidentParticleTrackInfo *  trackInfo(
                dynamic_cast< CexmcIncidentParticleTrackInfo * >(
                                                track->GetUserInformation() ) );

    if ( ! trackInfo )
        return;

    G4VPhysicalVolume *  volume( NULL );

    if ( stepPoint )
        volume = stepPoint->GetTouchable()->GetVolume();
    else
        volume = track->GetVolume();

    G4VSolid *  targetSolid( volume->GetLogicalVolume()->GetSolid() );

    if ( ! proposedMaxILInitialized )
    {
        G4double  targetRadius( CexmcDblMax );
        G4Tubs *  targetTube( dynamic_cast< G4Tubs * >( targetSolid ) );

        if ( targetTube )
            targetRadius = targetTube->GetOuterRadius();

        proposedMaxIL = GetProposedMaxIL( targetRadius * 2 );
        proposedMaxILInitialized = true;
    }

    G4ThreeVector  position;
    G4ThreeVector  direction;

    if ( stepPoint )
    {
        const G4AffineTransform &  transform( stepPoint->GetTouchable()->
                                              GetHistory()->GetTopTransform() );
        position = transform.TransformPoint( stepPoint->GetPosition() );
        direction = transform.TransformPoint(
                                        stepPoint->GetMomentumDirection() );
    }
    else
    {
        const G4AffineTransform &  transform( track->GetTouchable()->
                                              GetHistory()->GetTopTransform() );
        position = transform.TransformPoint( track->GetPosition() );
        direction = transform.TransformPoint( track->GetMomentumDirection() );
    }

    G4double  distanceInTarget( targetSolid->DistanceToOut( position,
                                                            direction ) );
    trackInfo->ResetCurrentTrackLengthInTarget();
    trackInfo->SetFinalTrackLengthInTarget( G4UniformRand() *
                                std::max( distanceInTarget, proposedMaxIL ) );
    trackInfo->SetNeedsTrackLengthResampling( false );
}


#endif

