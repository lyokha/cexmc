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
#include "CexmcException.hh"
#include "CexmcCommon.hh"


template  < typename  BasePhysics, template  < typename > class  StudiedPhysics,
            typename  ProductionModel >
class  CexmcPhysicsList : public BasePhysics, public CexmcPhysicsManager
{
    public:
        CexmcPhysicsList();

    public:
        CexmcProductionModel *  GetProductionModel( void );

        G4bool                  IsStudiedProcessAllowed( void ) const;

        void                    ResampleTrackLengthInTarget(
                                            const G4Track *  track,
                                            const G4StepPoint *  stepPoint );

    private:
        StudiedPhysics< ProductionModel > *  studiedPhysics;

        G4bool                               proposedMaxILInitialized;

        G4double                             proposedMaxIL;
};


template  < typename  BasePhysics, template  < typename > class  StudiedPhysics,
            typename  ProductionModel >
CexmcPhysicsList< BasePhysics, StudiedPhysics, ProductionModel >::
                CexmcPhysicsList() :
    studiedPhysics( NULL ), proposedMaxILInitialized( false ),
    proposedMaxIL( CexmcDblMax )
{
    studiedPhysics = new StudiedPhysics< ProductionModel >( this );
    this->RegisterPhysics( studiedPhysics );
}


template  < typename  BasePhysics, template  < typename > class  StudiedPhysics,
            typename  ProductionModel >
CexmcProductionModel *
            CexmcPhysicsList< BasePhysics, StudiedPhysics, ProductionModel >::
                GetProductionModel( void )
{
    return studiedPhysics->GetProductionModel();
}


template  < typename  BasePhysics, template  < typename > class  StudiedPhysics,
            typename  ProductionModel >
G4bool  CexmcPhysicsList< BasePhysics, StudiedPhysics, ProductionModel >::
                IsStudiedProcessAllowed( void ) const
{
    return numberOfTriggeredStudiedInteractions == 0;
}


template  < typename  BasePhysics, template  < typename > class  StudiedPhysics,
            typename  ProductionModel >
void  CexmcPhysicsList< BasePhysics, StudiedPhysics, ProductionModel >::
                ResampleTrackLengthInTarget( const G4Track *  track,
                                             const G4StepPoint *  stepPoint )
{
    /* all callers must ensure that track info object is of type
     * CexmcIncidentParticleTrackInfo*, so we can use static_cast<> here */
    CexmcIncidentParticleTrackInfo *  trackInfo(
                static_cast< CexmcIncidentParticleTrackInfo * >(
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

        if ( ! targetTube )
            throw CexmcException( CexmcIncompatibleGeometry );

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

