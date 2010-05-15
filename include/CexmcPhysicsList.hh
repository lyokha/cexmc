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

#include <G4VModularPhysicsList.hh>
#include "CexmcStudiedPhysics.hh"
#include "CexmcStudiedProcess.hh"
#include "CexmcPhysicsManager.hh"
#include "CexmcProductionModel.hh"
#include "CexmcCommon.hh"


template  < typename  BasePhysics, typename  Particle,
            template  < typename, typename > class  StudiedPhysics,
            typename  ProductionModel >
class  CexmcPhysicsList : virtual public BasePhysics,
                                  public CexmcPhysicsManager
{
    public:
        CexmcPhysicsList();

    public:
        const G4ParticleDefinition *  GetIncidentParticleType( void ) const;

        void  ActivateStudiedProcess( G4bool  on, G4double  maxStep = DBL_MAX );

        CexmcProductionModel *  GetProductionModel( void );

    private:
        CexmcProductionModel *  productionModel;
};


template  < typename  BasePhysics, typename  Particle,
            template  < typename, typename > class  StudiedPhysics,
            typename  ProductionModel >
CexmcPhysicsList< BasePhysics, Particle, StudiedPhysics, ProductionModel >::
                CexmcPhysicsList() : productionModel( NULL )
{
    this->RegisterPhysics( new StudiedPhysics< Particle, ProductionModel > );
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
void  CexmcPhysicsList< BasePhysics, Particle, StudiedPhysics,
                        ProductionModel >::
                ActivateStudiedProcess( G4bool  on, G4double  maxStep )
{
    G4ParticleDefinition *  particle( Particle::Definition() );
    G4ProcessManager *      processManager( particle->GetProcessManager() );
    G4ProcessVector *       processVector( processManager->GetProcessList() );
    G4int                   processVectorSize(
                                    processManager->GetProcessListLength() );

    for ( int  i( 0 ); i < processVectorSize; ++i )
    {
        G4VProcess *  process( ( *processVector )[ i ] );
        if ( process->GetProcessName() == CexmcStudiedProcessFullName )
        {
            CexmcStudiedProcess< Particle > *  studiedProcess(
                dynamic_cast< CexmcStudiedProcess< Particle > * >( process ) );
            if ( ! studiedProcess )
                break;

            G4bool  isProcessActivated( processManager->GetProcessActivation(
                                                                    process ) );

            if ( studiedProcess->GetNumberOfTriggeredEvents() > 0 )
                on = false;

            if ( ( on && isProcessActivated ) ||
                 ( ! on && ! isProcessActivated ) )
                break;

            if ( on )
            {
                studiedProcess->SetMaxStepSize( std::max( proposedMaxIL,
                                                          maxStep ) );
            }
            processManager->SetProcessActivation( process, on );
            break;
        }
    }
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


#endif

