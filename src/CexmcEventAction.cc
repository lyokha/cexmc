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
#include <G4VVisManager.hh>
#include <G4Circle.hh>
#include <G4VisAttributes.hh>
#include <G4VTrajectory.hh>
#include <G4TrajectoryContainer.hh>
#include "CexmcEventAction.hh"
#include "CexmcEventActionMessenger.hh"
#include "CexmcEventInfo.hh"
#include "CexmcChargeExchangeReconstructor.hh"
#include "CexmcHistoManager.hh"
#include "CexmcRunManager.hh"
#include "CexmcRun.hh"
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


CexmcEventAction::CexmcEventAction( CexmcPhysicsManager *  physicsManager,
                                    G4int  verbose ) :
    physicsManager( physicsManager ), reconstructor( NULL ), verbose( verbose ),
    verboseDraw( 4 ), messenger( NULL )
{
    G4DigiManager *  digiManager( G4DigiManager::GetDMpointer() );
    digiManager->AddNewModule( new CexmcEnergyDepositDigitizer(
                                                    CexmcEDDigitizerName ) );
    digiManager->AddNewModule( new CexmcTrackPointsDigitizer(
                                                    CexmcTPDigitizerName ) );
    reconstructor = new CexmcChargeExchangeReconstructor;
    messenger = new CexmcEventActionMessenger( this );
}


CexmcEventAction::~CexmcEventAction()
{
    delete reconstructor;
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
    G4int     calorimeterEDLeftMaxX( digitizer->GetCalorimeterEDLeftMaxX() );
    G4int     calorimeterEDLeftMaxY( digitizer->GetCalorimeterEDLeftMaxY() );
    G4int     calorimeterEDRightMaxX( digitizer->GetCalorimeterEDRightMaxX() );
    G4int     calorimeterEDRightMaxY( digitizer->GetCalorimeterEDRightMaxY() );
    const CexmcEnergyDepositCalorimeterCollection &
                calorimeterEDLeftCollection(
                            digitizer->GetCalorimeterEDLeftCollection() );
    const CexmcEnergyDepositCalorimeterCollection &
                calorimeterEDRightCollection(
                            digitizer->GetCalorimeterEDRightCollection() );

    /* ATTENTION: return object in heap - must be freed by caller! */
    return new CexmcEnergyDepositStore( monitorED, vetoCounterEDLeft,
                    vetoCounterEDRight, calorimeterEDLeft, calorimeterEDRight,
                    calorimeterEDLeftMaxX, calorimeterEDLeftMaxY,
                    calorimeterEDRightMaxX, calorimeterEDRightMaxY,
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
    G4cout << "      ---" << G4endl;
    G4cout << "      angle between the " <<
        tpStore->targetTPOutputParticle.particle->GetParticleName() <<
        " decay products : " <<
        tpStore->targetTPOutputParticleDecayProductParticle1.directionWorld.
            angle(
        tpStore->targetTPOutputParticleDecayProductParticle2.directionWorld ) /
            deg << " deg" << G4endl;
}


void  CexmcEventAction::PrintProductionModelData(
                            const CexmcAngularRangeList &  angularRanges,
                            const CexmcProductionModelData &  pmData ) const
{
    G4cout << " --- Triggered angular ranges: " << angularRanges;
    G4cout << " --- Production model data: " << pmData;
}


void  CexmcEventAction::PrintReconstructedData( const CexmcAngularRangeList &
                                            triggeredRecAngularRanges ) const
{
    G4cout << " --- Reconstructed data: " << G4endl;
    G4cout << "       -- entry points:" << G4endl;
    G4cout << "              left: " << G4BestUnit(
        reconstructor->GetCalorimeterEPLeftPosition(), "Length" ) << G4endl;
    G4cout << "             right: " << G4BestUnit(
        reconstructor->GetCalorimeterEPRightPosition(), "Length" ) << G4endl;
    G4cout << "            target: " << G4BestUnit(
        reconstructor->GetTargetEPPosition(), "Length" ) << G4endl;
    G4cout << "       -- the angle: " << reconstructor->GetTheAngle() / deg <<
        " deg" << G4endl;
    G4cout << "       -- mass of the output particle: " << G4BestUnit(
        reconstructor->GetOutputParticleMass(), "Energy" ) << G4endl;
    G4cout << "       -- mass of the nucleus output particle: " << G4BestUnit(
        reconstructor->GetNucleusOutputParticleMass(), "Energy" ) << G4endl;
    if ( reconstructor->IsMassCutUsed() )
    {
        if ( reconstructor->HasMassCutTriggered() )
            G4cout << "            < mass cut passed >" << G4endl;
        else
            G4cout << "            < mass cut failed >" << G4endl;
    }
    const CexmcProductionModelData &  pmData(
                                    reconstructor->GetProductionModelData() );
    G4cout << "       -- production model data: " << pmData;
    G4cout << "       -- triggered angular ranges: " <<
                                                    triggeredRecAngularRanges;
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


void  CexmcEventAction::DrawTrajectories( const G4Event *  event )
{
    if ( ! G4VVisManager::GetConcreteInstance() )
        return;

    G4int                    nTraj( 0 );
    G4TrajectoryContainer *  trajContainer( event->GetTrajectoryContainer() );

    if ( ! trajContainer )
        return;

    nTraj = trajContainer->entries();

    G4int  drawMode( drawTrajectoryMarkers ? 1000 : 0 );

    for ( int  i( 0 ); i < nTraj; ++i )
    {
        G4VTrajectory *  traj( ( *trajContainer )[ i ] );
        traj->DrawTrajectory( drawMode );
    }
}


void  CexmcEventAction::DrawTrackPoints(
                                const CexmcTrackPointsStore *  tpStore ) const
{
    G4VVisManager *  visManager( G4VVisManager::GetConcreteInstance() );

    if ( ! visManager )
        return;

    G4Circle         circle;
    G4VisAttributes  visAttributes( G4Color( 0.0, 1.0, 0.4 ) );
    circle.SetScreenSize( 5.0 );
    circle.SetFillStyle( G4Circle::filled );
    circle.SetVisAttributes( visAttributes );

    if ( tpStore->monitorTP.IsValid() )
    {
        circle.SetPosition( tpStore->monitorTP.positionWorld );
        visManager->Draw( circle );
    }

    if ( tpStore->targetTPIncidentParticle.IsValid() )
    {
        circle.SetPosition( tpStore->targetTPIncidentParticle.positionWorld );
        visManager->Draw( circle );
    }

    if ( tpStore->targetTPOutputParticle.IsValid() )
    {
        circle.SetPosition( tpStore->targetTPOutputParticle.positionWorld );
        visManager->Draw( circle );
    }

    if ( tpStore->vetoCounterTPLeft.IsValid() )
    {
        circle.SetPosition( tpStore->vetoCounterTPLeft.positionWorld );
        visManager->Draw( circle );
    }

    if ( tpStore->vetoCounterTPRight.IsValid() )
    {
        circle.SetPosition( tpStore->vetoCounterTPRight.positionWorld );
        visManager->Draw( circle );
    }

    if ( tpStore->calorimeterTPLeft.IsValid() )
    {
        circle.SetPosition( tpStore->calorimeterTPLeft.positionWorld );
        visManager->Draw( circle );
    }

    if ( tpStore->calorimeterTPRight.IsValid() )
    {
        circle.SetPosition( tpStore->calorimeterTPRight.positionWorld );
        visManager->Draw( circle );
    }
}


void  CexmcEventAction::DrawReconstructionData( void )
{
    G4VVisManager *  visManager( G4VVisManager::GetConcreteInstance() );

    if ( ! visManager )
        return;

    G4Circle  circle( reconstructor->GetTargetEPWorldPosition() );
    circle.SetScreenSize( 5.0 );
    circle.SetFillStyle( G4Circle::filled );
    G4VisAttributes  visAttributes( G4Color( 1.0, 0.4, 0.0 ) );
    circle.SetVisAttributes( visAttributes );
    visManager->Draw( circle );

    circle.SetScreenSize( 10.0 );
    circle.SetPosition( reconstructor->GetCalorimeterEPLeftWorldPosition() );
    visManager->Draw( circle );

    circle.SetPosition( reconstructor->GetCalorimeterEPRightWorldPosition() );
    visManager->Draw( circle );
}


void  CexmcEventAction::UpdateRunHits(
                                    const CexmcAngularRangeList &  aRangesReal,
                                    const CexmcAngularRangeList &  aRangesRec,
                                    G4bool  tpDigitizerHasTriggered,
                                    G4bool  edDigitizerHasTriggered,
                                    G4bool  reconstructorHasTriggered )
{
    G4RunManager *    runManager( G4RunManager::GetRunManager() );
    const CexmcRun *  run( static_cast< const CexmcRun * >(
                                                runManager->GetCurrentRun() ) );
    CexmcRun *        theRun( const_cast< CexmcRun * >( run ) );

    for ( CexmcAngularRangeList::const_iterator  k( aRangesReal.begin() );
                                                k != aRangesReal.end(); ++k )
    {
        if ( tpDigitizerHasTriggered )
            theRun->IncrementNmbOfHitsSampled( k->index );
        if ( edDigitizerHasTriggered )
            theRun->IncrementNmbOfHitsTriggeredReal( k->index );
    }

    if ( reconstructorHasTriggered )
    {
        for ( CexmcAngularRangeList::const_iterator  k( aRangesRec.begin() );
                                                k != aRangesRec.end(); ++k )
        {
            theRun->IncrementNmbOfHitsTriggeredRec( k->index );
        }
    }
}


void  CexmcEventAction::EndOfEventAction( const G4Event *  event )
{
    G4DigiManager *                digiManager( G4DigiManager::GetDMpointer() );
    CexmcEnergyDepositDigitizer *  energyDepositDigitizer(
            static_cast< CexmcEnergyDepositDigitizer* >( digiManager->
                                FindDigitizerModule( CexmcEDDigitizerName ) ) );
    CexmcTrackPointsDigitizer *  trackPointsDigitizer(
            static_cast< CexmcTrackPointsDigitizer* >( digiManager->
                                FindDigitizerModule( CexmcTPDigitizerName ) ) );

    energyDepositDigitizer->Digitize();
    trackPointsDigitizer->Digitize();

    G4bool  edDigitizerHasTriggered( energyDepositDigitizer->HasTriggered() );
    G4bool  tpDigitizerHasTriggered( trackPointsDigitizer->HasTriggered() );
    G4bool  reconstructorHasTriggered( false );
    G4bool  reconstructorHasFullTrigger( false );

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

        const CexmcAngularRangeList &  angularRanges(
                                productionModel->GetAngularRanges() );
        const CexmcAngularRangeList &  triggeredAngularRanges(
                                productionModel->GetTriggeredAngularRanges() );
        const CexmcProductionModelData &  pmData(
                                productionModel->GetProductionModelData() );

        if ( edDigitizerHasTriggered )
        {
            reconstructor->Reconstruct( edStore );
            reconstructorHasTriggered = reconstructor->HasTriggered();
            reconstructorHasFullTrigger = reconstructorHasTriggered &&
                                          reconstructor->HasMassCutTriggered();
        }

        CexmcAngularRangeList  triggeredRecAngularRanges;

        if ( reconstructorHasTriggered )
        {
            for ( CexmcAngularRangeList::const_iterator
                  k( angularRanges.begin() ); k != angularRanges.end(); ++k )
            {
                G4double  cosTheta( reconstructor->GetProductionModelData().
                                    outputParticleSCM.cosTheta() );
                if ( cosTheta <= k->top && cosTheta > k->bottom )
                    triggeredRecAngularRanges.push_back( CexmcAngularRange(
                                                k->top, k->bottom, k->index ) );
            }
        }

        UpdateRunHits( triggeredAngularRanges, triggeredRecAngularRanges,
                       tpDigitizerHasTriggered, edDigitizerHasTriggered,
                       reconstructorHasFullTrigger );

        if ( verbose > 0 )
        {
            G4bool  printMessages( verbose > 3 ||
                        ( ( verbose == 1 ) && tpDigitizerHasTriggered ) ||
                        ( ( verbose == 2 ) && edDigitizerHasTriggered ) ||
                        ( ( verbose == 3 ) && ( tpDigitizerHasTriggered ||
                                                edDigitizerHasTriggered ) ) );
            if ( printMessages )
            {
                G4cout << "Event " << event->GetEventID() << G4endl;
                if ( tpDigitizerHasTriggered )
                {
                    PrintTrackPoints( tpStore );
                    PrintProductionModelData( triggeredAngularRanges, pmData );
                }
                if ( reconstructorHasTriggered )
                    PrintReconstructedData( triggeredRecAngularRanges );
                if ( edDigitizerHasTriggered )
                    PrintEnergyDeposit( edStore );
            }
        }

        if ( verboseDraw > 0 )
        {
            G4bool  drawTrajectories( verboseDraw > 3 ||
                        ( ( verboseDraw == 1 ) && tpDigitizerHasTriggered ) ||
                        ( ( verboseDraw == 2 ) && edDigitizerHasTriggered ) ||
                        ( ( verboseDraw == 3 ) && ( tpDigitizerHasTriggered ||
                                                edDigitizerHasTriggered ) ) );
            if ( drawTrajectories )
            {
                DrawTrajectories( event );
                if ( tpDigitizerHasTriggered )
                    DrawTrackPoints( tpStore );
                if ( reconstructorHasTriggered )
                    DrawReconstructionData();
            }
        }

        if ( edDigitizerHasTriggered )
            FillEnergyDepositHisto( edStore );

        G4Event *  theEvent( const_cast< G4Event * >( event ) );
        theEvent->SetUserInformation( new CexmcEventInfo(
                                                edDigitizerHasTriggered,
                                                tpDigitizerHasTriggered,
                                                reconstructorHasTriggered ) );
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

