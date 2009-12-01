/*
 * ============================================================================
 *
 *       Filename:  CexmcEventAction.cc
 *
 *    Description:  event action
 *
 *        Version:  1.0
 *        Created:  27.10.2009 22:48:08
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4ParticleDefinition.hh>
#include <G4HadronicInteraction.hh>
#include <G4ProcessManager.hh>
#include <G4ProcessVector.hh>
#include <G4DigiManager.hh>
#include <G4Event.hh>
#include "CexmcEventAction.hh"
#include "CexmcEventActionMessenger.hh"
#include "CexmcHistoManager.hh"
#include "CexmcPhysicsManager.hh"
#include "CexmcProductionModel.hh"
#include "CexmcProductionModelData.hh"
#include "CexmcEnergyDepositDigitizer.hh"
#include "CexmcEnergyDepositStore.hh"
#include "CexmcTrackPointsDigitizer.hh"
#include "CexmcTrackPointsStore.hh"
#include "CexmcTrackPointInfo.hh"
#include "CexmcException.hh"
#include "CexmcCommon.hh"


namespace
{
    const G4String  energyDepositDigitizerName( "EDDig" );
    const G4String  trackPointsDigitizerName( "TPDig" );
}


CexmcEventAction::CexmcEventAction( CexmcPhysicsManager *  physicsManager,
                                    G4int  verbose ) :
    physicsManager( physicsManager ), verbose( verbose ), messenger( NULL )
{
    G4DigiManager *  digiManager( G4DigiManager::GetDMpointer() );
    digiManager->AddNewModule( new CexmcEnergyDepositDigitizer(
                                                energyDepositDigitizerName ) );
    digiManager->AddNewModule( new CexmcTrackPointsDigitizer(
                                                trackPointsDigitizerName ) );
    messenger = new CexmcEventActionMessenger( this );
}


CexmcEventAction::~CexmcEventAction()
{
    delete messenger;
}


void  CexmcEventAction::BeginOfEventAction( const G4Event * )
{
    physicsManager->ActivateStudiedProcess( false );
}


CexmcEnergyDepositStore *  CexmcEventAction::MakeEnergyDepositStore(
                        const CexmcEnergyDepositDigitizer *  digitizer ) const
{
    G4double  monitorED( digitizer->GetMonitorED() );
    G4double  vetoCounterEDLeft( digitizer->GetVetoCounterEDLeft() );
    G4double  vetoCounterEDRight( digitizer->GetVetoCounterEDRight() );
    G4double  calorimeterEDLeft( digitizer->GetCalorimeterEDLeft() );
    G4double  calorimeterEDRight( digitizer->GetCalorimeterEDRight() );
    const CexmcEnergyDepositCalorimeterCollection &
                calorimeterEDLeftCollection(
                            digitizer->GetCalorimeterEDLeftCollection() );
    const CexmcEnergyDepositCalorimeterCollection &
                calorimeterEDRightCollection(
                            digitizer->GetCalorimeterEDRightCollection() );

    /* ATTENTION: return object in heap - must be freed by caller! */
    return new CexmcEnergyDepositStore( monitorED, vetoCounterEDLeft,
                    vetoCounterEDRight, calorimeterEDLeft, calorimeterEDRight,
                    calorimeterEDLeftCollection, calorimeterEDRightCollection );
}


CexmcTrackPointsStore *  CexmcEventAction::MakeTrackPointsStore(
                          const CexmcTrackPointsDigitizer *  digitizer ) const
{
    const CexmcTrackPointInfo &
                monitorTP( digitizer->GetMonitorTP() );
    const CexmcTrackPointInfo &
                targetTPIncidentParticle(
                    digitizer->GetTargetTPIncidentParticle() );
    const CexmcTrackPointInfo &
                targetTPOutputParticle(
                    digitizer->GetTargetTPOutputParticle() );
    const CexmcTrackPointInfo &
                targetTPNucleusParticle(
                    digitizer->GetTargetTPNucleusParticle() );
    const CexmcTrackPointInfo &
                targetTPOutputParticleDecayProductParticle1(
                    digitizer->
                    GetTargetTPOutputParticleDecayProductParticle( 0 ) );
    const CexmcTrackPointInfo &
                targetTPOutputParticleDecayProductParticle2(
                    digitizer->
                    GetTargetTPOutputParticleDecayProductParticle( 1 ) );
    const CexmcTrackPointInfo &
                vetoCounterTPLeft(
                    digitizer->GetVetoCounterTPLeft() );
    const CexmcTrackPointInfo &
                vetoCounterTPRight(
                    digitizer->GetVetoCounterTPRight() );
    const CexmcTrackPointInfo &
                calorimeterTPLeft(
                    digitizer->GetCalorimeterTPLeft() );
    const CexmcTrackPointInfo &
                calorimeterTPRight(
                    digitizer->GetCalorimeterTPRight() );

    /* ATTENTION: return object in heap - must be freed by caller! */
    return new CexmcTrackPointsStore( monitorTP, targetTPIncidentParticle,
                  targetTPOutputParticle, targetTPNucleusParticle,
                  targetTPOutputParticleDecayProductParticle1,
                  targetTPOutputParticleDecayProductParticle2,
                  vetoCounterTPLeft, vetoCounterTPRight,
                  calorimeterTPLeft, calorimeterTPRight );
}


void  CexmcEventAction::PrintEnergyDeposit(
                                const CexmcEnergyDepositStore *  edStore ) const
{
    G4cout << " --- Energy Deposit" << G4endl;
    G4cout << "       monitor : " <<
            G4BestUnit( edStore->monitorED, "Energy" ) << G4endl;
    G4cout << "        vc (l) : " <<
            G4BestUnit( edStore->vetoCounterEDLeft, "Energy" ) << G4endl;
    G4cout << "        vc (r) : " <<
            G4BestUnit( edStore->vetoCounterEDRight, "Energy" ) << G4endl;
    G4cout << "       cal (l) : " <<
            G4BestUnit( edStore->calorimeterEDLeft, "Energy" );
    G4cout << edStore->calorimeterEDLeftCollection;
    G4cout << "       cal (r) : " <<
            G4BestUnit( edStore->calorimeterEDRight, "Energy" );
    G4cout << edStore->calorimeterEDRightCollection;
}


void  CexmcEventAction::PrintTrackPoints(
                                const CexmcTrackPointsStore *  tpStore ) const
{
    if ( ! tpStore )
        return;

    G4cout << " --- Track Points" << G4endl;
    G4cout << "       monitor : " << tpStore->monitorTP << G4endl;
    G4cout << "        target : " << tpStore->targetTPIncidentParticle <<
                                                                        G4endl;
    G4cout << "               : " << tpStore->targetTPOutputParticle << G4endl;
    G4cout << "               : " << tpStore->targetTPNucleusParticle << G4endl;
    G4cout << "               : " <<
            tpStore->targetTPOutputParticleDecayProductParticle1 << G4endl;
    G4cout << "               : " <<
            tpStore->targetTPOutputParticleDecayProductParticle2 << G4endl;
    G4cout << "        vc (l) : " << tpStore->vetoCounterTPLeft << G4endl;
    G4cout << "        vc (r) : " << tpStore->vetoCounterTPRight << G4endl;
    G4cout << "       cal (l) : " << tpStore->calorimeterTPLeft << G4endl;
    G4cout << "       cal (r) : " << tpStore->calorimeterTPRight << G4endl;
}


void  CexmcEventAction::PrintProductionModelData(
                            const CexmcAngularRangeList &  angularRanges,
                            const CexmcProductionModelData &  pmData ) const
{
    G4cout << " --- Triggered angular ranges: " << angularRanges;
    G4cout << " --- Production model data: " << pmData;
}


void  CexmcEventAction::FillEnergyDepositHisto(
                                const CexmcEnergyDepositStore *  edStore ) const
{
    CexmcHistoManager *  histoManager( CexmcHistoManager::Instance() );

    G4int  i( 0 );

    for ( CexmcEnergyDepositCalorimeterCollection::const_iterator
            k( edStore->calorimeterEDLeftCollection.begin() );
            k != edStore->calorimeterEDLeftCollection.end(); ++k )
    {
        G4int  j( 0 );
        for ( CexmcEnergyDepositCrystalRowCollection::const_reverse_iterator
                l( k->rbegin() ); l != k->rend(); ++l )
        {
            if ( *l > 0 )
                histoManager->Add( CexmcEnergyDepositInLeftCalorimeter,
                                   j, i, *l );
            ++j;
        }
        ++i;
    }

    i = 0;

    for ( CexmcEnergyDepositCalorimeterCollection::const_iterator
            k( edStore->calorimeterEDRightCollection.begin() );
            k != edStore->calorimeterEDRightCollection.end(); ++k )
    {
        G4int  j( 0 );
        for ( CexmcEnergyDepositCrystalRowCollection::const_reverse_iterator
                l( k->rbegin() ); l != k->rend(); ++l )
        {
            if ( *l > 0 )
                histoManager->Add( CexmcEnergyDepositInRightCalorimeter,
                                   j, i, *l );
            ++j;
        }
        ++i;
    }
}


void  CexmcEventAction::EndOfEventAction( const G4Event *  event )
{
    G4DigiManager *                digiManager( G4DigiManager::GetDMpointer() );
    CexmcEnergyDepositDigitizer *  energyDepositDigitizer(
            static_cast< CexmcEnergyDepositDigitizer* >( digiManager->
                        FindDigitizerModule( energyDepositDigitizerName ) ) );
    CexmcTrackPointsDigitizer *  trackPointsDigitizer(
            static_cast< CexmcTrackPointsDigitizer* >( digiManager->
                        FindDigitizerModule( trackPointsDigitizerName ) ) );

    energyDepositDigitizer->Digitize();
    trackPointsDigitizer->Digitize();

    CexmcEnergyDepositStore *  edStore( MakeEnergyDepositStore(
                                                    energyDepositDigitizer ) );
    CexmcTrackPointsStore *    tpStore( MakeTrackPointsStore(
                                                    trackPointsDigitizer ) );

    try
    {
        CexmcProductionModel *  productionModel(
                                        physicsManager->GetProductionModel() );

        if ( ! productionModel )
            throw CexmcException( CexmcWeirdException );

        const CexmcAngularRangeList &  triggeredAngularRanges(
                                productionModel->GetTriggeredAngularRanges() );
        const CexmcProductionModelData &  pmData(
                                productionModel->GetProductionModelData() );

        if ( verbose > 0 )
        {
            G4bool  printEnergyDeposit( ( ( verbose == 1 || verbose == 3 ) &&
                      energyDepositDigitizer->HasTriggered() ) || verbose > 3 );
            G4bool  printTrackPoints( ( ( verbose == 2 || verbose == 3 ) &&
                      trackPointsDigitizer->HasTriggered() ) || verbose > 3 );
            G4cout << "Event " << event->GetEventID() << G4endl;
            if ( printEnergyDeposit )
                PrintEnergyDeposit( edStore );
            if ( printTrackPoints )
                PrintTrackPoints( tpStore );
            if ( trackPointsDigitizer->HasTriggered() )
                PrintProductionModelData( triggeredAngularRanges, pmData );
        }

        FillEnergyDepositHisto( edStore );
    }
    catch ( CexmcException &  e )
    {
        G4cout << e.what() << G4endl;
    }
    catch ( ... )
    {
        G4cout << "Unknown exception caught" << G4endl;
    }

    delete edStore;
    delete tpStore;
}

