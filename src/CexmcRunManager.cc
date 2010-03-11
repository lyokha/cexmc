/*
 * ============================================================================
 *
 *       Filename:  CexmcRunManager.cc
 *
 *    Description:  run manager
 *
 *        Version:  1.0
 *        Created:  03.11.2009 20:27:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <stdlib.h>
#include <sys/stat.h>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <G4Eta.hh>
#include <G4DigiManager.hh>
#include <G4SDManager.hh>
#include <G4DecayTable.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTable.hh>
#include <G4UImanager.hh>
#include <G4Timer.hh>
#include "CexmcRunManager.hh"
#include "CexmcRunManagerMessenger.hh"
#include "CexmcRunAction.hh"
#include "CexmcRun.hh"
#include "CexmcPhysicsManager.hh"
#include "CexmcProductionModel.hh"
#include "CexmcSimpleDecayTableStore.hh"
#include "CexmcPrimaryGeneratorAction.hh"
#include "CexmcEnergyDepositDigitizer.hh"
#include "CexmcSimpleEnergyDeposit.hh"
#include "CexmcEnergyDepositInLeftRightSet.hh"
#include "CexmcEnergyDepositInCalorimeter.hh"
#include "CexmcTrackPoints.hh"
#include "CexmcTrackPointsInLeftRightSet.hh"
#include "CexmcTrackPointsInCalorimeter.hh"
#include "CexmcChargeExchangeReconstructor.hh"
#include "CexmcEventAction.hh"
#include "CexmcParticleGun.hh"
#include "CexmcEnergyDepositStore.hh"
#include "CexmcTrackPointsStore.hh"
#include "CexmcCalorimeterGeometry.hh"
#include "CexmcEventSObject.hh"
#include "CexmcEventFastSObject.hh"
#include "CexmcTrackPointInfo.hh"
#include "CexmcEventInfo.hh"


namespace
{
    G4String  gdmlFileExtension( ".gdml" );
    G4String  gdmlbz2FileExtension( ".gdml.bz2" );
}


CexmcRunManager::CexmcRunManager( const G4String &  projectId,
                                  const G4String &  rProject,
                                  G4bool  overrideExistingProject ) :
    productionModelType( CexmcUnknownProductionModel ),
    gdmlFileName( "default.gdml" ), zipGdmlFile( false ), projectsDir( "." ),
    projectId( projectId ), rProject( rProject ), guiMacroName( "" ),
    eventCountPolicy( CexmcCountAllEvents ), numberOfEventsProcessed( 0 ),
    numberOfEventsProcessedEffective( 0 ), curEventRead( 0 ),
    eventsArchive( NULL ), fastEventsArchive( NULL ), physicsManager( NULL ),
    messenger( NULL )
{
    /* this exception must be caught before creating the object! */
    if ( rProject != "" && rProject == projectId )
        throw CexmcException( CexmcWeirdException );

    const char *  projectsDirEnv( getenv( "CEXMC_PROJECTS_DIR" ) );

    if ( projectsDirEnv )
        projectsDir = projectsDirEnv;

    struct stat  tmp;
    if ( stat( ( projectsDir + "/" + projectId + ".rdb" ).c_str(), &tmp ) == 0
         && ! overrideExistingProject )
        throw CexmcException( CexmcProjectExists );

    messenger = new CexmcRunManagerMessenger( this );

    if ( ProjectIsRead() )
        ReadPreinitProjectData();
}


CexmcRunManager::~CexmcRunManager()
{
    if ( ProjectIsRead() && zipGdmlFile )
    {
        G4String  cmd( G4String( "bzip2 " ) + projectsDir + "/" + rProject +
                       gdmlFileExtension );
        if ( system( cmd ) != 0 )
            G4cerr << "Failed to zip geometry data" << G4endl;
    }

    delete messenger;
}


void  CexmcRunManager::ReadPreinitProjectData( void )
{
    if ( ! ProjectIsRead() )
        return;

    /* read run data */
    std::ifstream    runDataFile( ( projectsDir + "/" + rProject + ".rdb" ).
                                  c_str() );
    if ( ! runDataFile )
        throw CexmcException( CexmcReadProjectIncompleteException );

    {
        boost::archive::binary_iarchive  archive( runDataFile );
        archive >> sObject;
    }

    productionModelType = sObject.productionModelType;

    /* read gdml file */
    G4String  fileExtension( zipGdmlFile ? gdmlbz2FileExtension :
                                           gdmlFileExtension );
    G4String  cmd( G4String( "cp " ) + projectsDir + "/" + rProject +
                   fileExtension + " " + projectsDir + "/" + projectId +
                   fileExtension );
    if ( ProjectIsSaved() && system( cmd ) != 0 )
        throw CexmcException( CexmcReadProjectIncompleteException );

    if ( zipGdmlFile )
    {
        cmd = G4String( "bunzip2 " ) + projectsDir + "/" + rProject +
                                                        gdmlbz2FileExtension;
        if ( system( cmd ) != 0 )
            throw CexmcException( CexmcFileCompressException );
    }

    gdmlFileName = projectsDir + "/" + rProject + gdmlFileExtension;
}


void  CexmcRunManager::ReadProject( void )
{
    if ( ! ProjectIsRead() )
        return;

    if ( ! physicsManager )
        throw CexmcException( CexmcWeirdException );

    physicsManager->GetProductionModel()->SetAngularRanges(
                                                    sObject.angularRanges );
    G4DecayTable *  etaDecayTable( G4Eta::Definition()->GetDecayTable() );
    for ( CexmcDecayBranchesStore::const_iterator
            k( sObject.etaDecayTable.GetDecayBranches().begin() );
            k != sObject.etaDecayTable.GetDecayBranches().end(); ++k )
    {
        etaDecayTable->GetDecayChannel( k->first )->SetBR( k->second );
    }

    physicsManager->GetProductionModel()->ApplyFermiMotion(
                                            sObject.fermiMotionIsOn, false );
    eventCountPolicy = sObject.eventCountPolicy;

    const CexmcPrimaryGeneratorAction *  primaryGeneratorAction(
                            static_cast< const CexmcPrimaryGeneratorAction * >(
                                                userPrimaryGeneratorAction ) );
    CexmcPrimaryGeneratorAction *        thePrimaryGeneratorAction(
                            const_cast< CexmcPrimaryGeneratorAction * >(
                                            primaryGeneratorAction ) );
    CexmcParticleGun *      particleGun(
                                thePrimaryGeneratorAction->GetParticleGun() );
    G4ParticleDefinition *  particleDefinition(
                    G4ParticleTable::GetParticleTable()->FindParticle(
                                                sObject.incidentParticle ) );
    if ( ! particleDefinition )
        throw CexmcException( CexmcWeirdException );

    particleGun->SetParticleDefinition( particleDefinition );
    particleGun->SetOrigPosition( sObject.beamPos, false );
    particleGun->SetOrigDirection( sObject.beamDir, false );
    particleGun->SetOrigMomentumAmp( sObject.beamMomentumAmp, false );

    thePrimaryGeneratorAction->SetFwhmPosX( sObject.beamFwhmPosX, false );
    thePrimaryGeneratorAction->SetFwhmPosY( sObject.beamFwhmPosY, false );
    thePrimaryGeneratorAction->SetFwhmDirX( sObject.beamFwhmDirX, false );
    thePrimaryGeneratorAction->SetFwhmDirY( sObject.beamFwhmDirY, false );
    thePrimaryGeneratorAction->SetFwhmMomentumAmp(
                                        sObject.beamFwhmMomentumAmp, false );

    G4DigiManager *                digiManager( G4DigiManager::GetDMpointer() );
    CexmcEnergyDepositDigitizer *  edDigitizer(
            static_cast< CexmcEnergyDepositDigitizer * >(
                digiManager->FindDigitizerModule( CexmcEDDigitizerName ) ) );
    if ( ! edDigitizer )
        throw CexmcException( CexmcWeirdException );

    edDigitizer->SetMonitorThreshold( sObject.monitorEDThreshold, false );
    edDigitizer->SetVetoCounterLeftThreshold(
                                sObject.vetoCounterEDLeftThreshold, false );
    edDigitizer->SetVetoCounterRightThreshold(
                                sObject.vetoCounterEDRightThreshold, false );
    edDigitizer->SetCalorimeterLeftThreshold(
                                sObject.calorimeterEDLeftThreshold, false );
    edDigitizer->SetCalorimeterRightThreshold(
                                sObject.calorimeterEDRightThreshold, false );
    edDigitizer->SetCalorimeterTriggerAlgorithm(
                                sObject.calorimeterTriggerAlgorithm, false );
    edDigitizer->SetOuterCrystalsVetoAlgorithm(
                                sObject.outerCrystalsVetoAlgorithm, false );
    edDigitizer->SetOuterCrystalsVetoFraction(
                                sObject.outerCrystalsVetoFraction, false );
    edDigitizer->ApplyFiniteCrystalResolution(
                                sObject.applyFiniteCrystalResolution, false );
    edDigitizer->SetCrystalResolutionData( sObject.crystalResolutionData );

    const CexmcEventAction *  eventAction(
                static_cast< const CexmcEventAction * >( userEventAction ) );
    if ( ! eventAction )
        throw CexmcException( CexmcWeirdException );

    CexmcEventAction *        theEventAction( const_cast< CexmcEventAction * >(
                                                                eventAction ) );
    CexmcChargeExchangeReconstructor *  reconstructor(
                                        theEventAction->GetReconstructor() );
    if ( ! reconstructor )
        throw CexmcException( CexmcWeirdException );

    reconstructor->SetCalorimeterEntryPointDefinitionAlgorithm(
                                        sObject.epDefinitionAlgorithm );
    reconstructor->SetCalorimeterEntryPointDepthDefinitionAlgorithm(
                                        sObject.epDepthDefinitionAlgorithm );
    reconstructor->SetCrystalSelectionAlgorithm( sObject.csAlgorithm );
    reconstructor->SetCalorimeterEntryPointDepth( sObject.epDepth );
    reconstructor->UseTableMass( sObject.useTableMass );
    reconstructor->UseMassCut( sObject.useMassCut );
    reconstructor->SetMassCutOPCenter( sObject.mCutOPCenter );
    reconstructor->SetMassCutNOPCenter( sObject.mCutNOPCenter );
    reconstructor->SetMassCutOPWidth( sObject.mCutOPWidth );
    reconstructor->SetMassCutNOPWidth( sObject.mCutNOPWidth );
    reconstructor->SetMassCutEllipseAngle( sObject.mCutAngle );
    reconstructor->UseAbsorbedEnergyCut( sObject.useAbsorbedEnergyCut );
    reconstructor->SetAbsorbedEnergyCutCLCenter( sObject.aeCutCLCenter );
    reconstructor->SetAbsorbedEnergyCutCRCenter( sObject.aeCutCRCenter );
    reconstructor->SetAbsorbedEnergyCutCLWidth( sObject.aeCutCLWidth );
    reconstructor->SetAbsorbedEnergyCutCRWidth( sObject.aeCutCRWidth );
    reconstructor->SetAbsorbedEnergyCutEllipseAngle( sObject.aeCutAngle );
}


void  CexmcRunManager::SaveProject( void )
{
    if ( ! ProjectIsSaved() )
        return;

    /* save run data */
    if ( ! physicsManager )
        throw CexmcException( CexmcWeirdException );

    CexmcSimpleDecayTableStore  etaDecayTable(
                                    G4Eta::Definition()->GetDecayTable() );
    const CexmcPrimaryGeneratorAction *  primaryGeneratorAction(
                            static_cast< const CexmcPrimaryGeneratorAction * >(
                                                userPrimaryGeneratorAction ) );
    CexmcPrimaryGeneratorAction *  thePrimaryGeneratorAction(
                            const_cast< CexmcPrimaryGeneratorAction * >(
                                            primaryGeneratorAction ) );
    CexmcParticleGun *  particleGun(
                            thePrimaryGeneratorAction->GetParticleGun() );

    G4DigiManager *                digiManager( G4DigiManager::GetDMpointer() );
    CexmcEnergyDepositDigitizer *  edDigitizer(
            static_cast< CexmcEnergyDepositDigitizer * >(
                digiManager->FindDigitizerModule( CexmcEDDigitizerName ) ) );
    if ( ! edDigitizer )
        throw CexmcException( CexmcWeirdException );

    const CexmcEventAction *  eventAction(
                static_cast< const CexmcEventAction * >( userEventAction ) );
    CexmcEventAction *        theEventAction( const_cast< CexmcEventAction * >(
                                                                eventAction ) );
    CexmcChargeExchangeReconstructor *  reconstructor(
                                        theEventAction->GetReconstructor() );

    CexmcNmbOfHitsInRanges  nmbOfHitsSampled;
    CexmcNmbOfHitsInRanges  nmbOfHitsSampledFull;
    CexmcNmbOfHitsInRanges  nmbOfHitsTriggeredRealRange;
    CexmcNmbOfHitsInRanges  nmbOfHitsTriggeredRecRange;
    CexmcNmbOfHitsInRanges  nmbOfOrphanHits;
    G4int                   nmbOfFalseHitsTriggeredEDT( 0 );
    G4int                   nmbOfFalseHitsTriggeredRec( 0 );
    G4int                   nmbOfSavedEvents( 0 );
    G4int                   nmbOfSavedFastEvents( 0 );
    const CexmcRun *  run( static_cast< const CexmcRun * >( GetCurrentRun() ) );
    if ( run )
    {
        nmbOfHitsSampled = run->GetNmbOfHitsSampled();
        nmbOfHitsSampledFull = run->GetNmbOfHitsSampledFull();
        nmbOfHitsTriggeredRealRange = run->GetNmbOfHitsTriggeredRealRange();
        nmbOfHitsTriggeredRecRange = run->GetNmbOfHitsTriggeredRecRange();
        nmbOfOrphanHits = run->GetNmbOfOrphanHits();
        nmbOfFalseHitsTriggeredEDT = run->GetNmbOfFalseHitsTriggeredEDT();
        nmbOfFalseHitsTriggeredRec = run->GetNmbOfFalseHitsTriggeredRec();
        nmbOfSavedEvents = run->GetNmbOfSavedEvents();
        nmbOfSavedFastEvents = run->GetNmbOfSavedFastEvents();
    }

    CexmcRunSObject  sObject(
        productionModelType, gdmlFileName, etaDecayTable,
        physicsManager->GetProductionModel()->GetAngularRanges(),
        physicsManager->GetProductionModel()->IsFermiMotionOn(),
        eventCountPolicy,
        particleGun->GetParticleDefinition()->GetParticleName(),
        particleGun->GetOrigPosition(), particleGun->GetOrigDirection(),
        particleGun->GetOrigMomentumAmp(),
        primaryGeneratorAction->GetFwhmPosX(),
        primaryGeneratorAction->GetFwhmPosY(),
        primaryGeneratorAction->GetFwhmDirX(),
        primaryGeneratorAction->GetFwhmDirY(),
        primaryGeneratorAction->GetFwhmMomentumAmp(),
        edDigitizer->GetMonitorThreshold(),
        edDigitizer->GetVetoCounterLeftThreshold(),
        edDigitizer->GetVetoCounterRightThreshold(),
        edDigitizer->GetCalorimeterLeftThreshold(),
        edDigitizer->GetCalorimeterRightThreshold(),
        edDigitizer->GetCalorimeterTriggerAlgorithm(),
        edDigitizer->GetOuterCrystalsVetoAlgorithm(),
        edDigitizer->GetOuterCrystalsVetoFraction(),
        edDigitizer->IsFiniteCrystalResolutionApplied(),
        edDigitizer->GetCrystalResolutionData(),
        reconstructor->GetCalorimeterEntryPointDefinitionAlgorithm(),
        reconstructor->GetCalorimeterEntryPointDepthDefinitionAlgorithm(),
        reconstructor->GetCrystalSelectionAlgorithm(),
        reconstructor->GetCalorimeterEntryPointDepth(),
        reconstructor->IsTableMassUsed(), reconstructor->IsMassCutUsed(),
        reconstructor->GetMassCutOPCenter(),
        reconstructor->GetMassCutNOPCenter(),
        reconstructor->GetMassCutOPWidth(), reconstructor->GetMassCutNOPWidth(),
        reconstructor->GetMassCutEllipseAngle(),
        reconstructor->IsAbsorbedEnergyCutUsed(),
        reconstructor->GetAbsorbedEnergyCutCLCenter(),
        reconstructor->GetAbsorbedEnergyCutCRCenter(),
        reconstructor->GetAbsorbedEnergyCutCLWidth(),
        reconstructor->GetAbsorbedEnergyCutCRWidth(),
        reconstructor->GetAbsorbedEnergyCutEllipseAngle(),
        nmbOfHitsSampled, nmbOfHitsSampledFull, nmbOfHitsTriggeredRealRange,
        nmbOfHitsTriggeredRecRange, nmbOfOrphanHits, nmbOfFalseHitsTriggeredEDT,
        nmbOfFalseHitsTriggeredRec, nmbOfSavedEvents, nmbOfSavedFastEvents,
        numberOfEventsProcessed, numberOfEventsProcessedEffective,
        numberOfEventToBeProcessed );

    std::ofstream   runDataFile( ( projectsDir + "/" + projectId + ".rdb" ).
                                        c_str() );

    {
        boost::archive::binary_oarchive  archive( runDataFile );
        archive << sObject;
    }

    /* save gdml file */
    G4String            cmd( G4String( "cp " ) + gdmlFileName + " " +
                             projectsDir + "/" + projectId +
                             gdmlFileExtension );
    CexmcExceptionType  exceptionType( CexmcSystemException );
    if ( zipGdmlFile )
    {
        cmd = G4String( "bzip2 -c " ) + gdmlFileName + " > " + projectsDir +
                                        "/" + projectId + gdmlbz2FileExtension;
        exceptionType = CexmcFileCompressException;
    }
    if ( system( cmd ) != 0 )
        throw CexmcException( exceptionType );
}


void  CexmcRunManager::DoCommonEventLoop( G4int  nEvent, const G4String &  cmd,
                                          G4int  nSelect )
{
    G4int  iEvent( 0 );
    G4int  iEventEffective( 0 );

    for( iEvent = 0; iEventEffective < nEvent; ++iEvent )
    {
        currentEvent = GenerateEvent( iEvent );
        eventManager->ProcessOneEvent( currentEvent );
        CexmcEventInfo *  eventInfo( static_cast< CexmcEventInfo * >(
                                        currentEvent->GetUserInformation() ) );
        switch ( eventCountPolicy )
        {
        case CexmcCountAllEvents :
            ++iEventEffective;
            break;
        case CexmcCountEventsWithInteraction :
            if ( eventInfo->TpTriggerIsOk() )
                ++iEventEffective;
            break;
        case CexmcCountEventsWithTrigger :
            if ( eventInfo->EdTriggerIsOk() )
                ++iEventEffective;
            break;
        default :
            ++iEventEffective;
            break;
        }
        AnalyzeEvent( currentEvent );
        UpdateScoring();
        if( iEvent < nSelect )
            G4UImanager::GetUIpointer()->ApplyCommand( cmd );
        StackPreviousEvent( currentEvent );
        currentEvent = 0;
        if( runAborted )
            break;
    }

    numberOfEventsProcessed = iEvent;
    numberOfEventsProcessedEffective = iEventEffective;
}


void  CexmcRunManager::DoReadEventLoop( G4int  nEvent )
{
    G4int  iEvent( 0 );
    G4int  iEventEffective( 0 );
    G4int  nEventCount( 0 );

    if ( ! ProjectIsRead() )
        return;

    if ( ! physicsManager )
        throw CexmcException( CexmcWeirdException );

    CexmcProductionModel *  productionModel(
                                        physicsManager->GetProductionModel() );

    CexmcEventSObject      evSObject;
    CexmcEventFastSObject  evFastSObject;

    /* read events data */
    std::ifstream   eventsDataFile(
                        ( projectsDir + "/" + rProject + ".edb" ).c_str() );
    if ( ! eventsDataFile )
        throw CexmcException( CexmcReadProjectIncompleteException );

    boost::archive::binary_iarchive  evArchive( eventsDataFile );

    std::ifstream   eventsFastDataFile(
                        ( projectsDir + "/" + rProject + ".fdb" ).c_str() );
    if ( ! eventsFastDataFile )
        throw CexmcException( CexmcReadProjectIncompleteException );

    boost::archive::binary_iarchive  evFastArchive( eventsFastDataFile );

    G4Event  event;
    currentEvent = &event;
    G4SDManager *      sdManager( G4SDManager::GetSDMpointer() );
    event.SetHCofThisEvent( sdManager->PrepareNewEvent() );
    G4HCofThisEvent *  hcOfThisEvent( event.GetHCofThisEvent() );

    G4DigiManager *  digiManager( G4DigiManager::GetDMpointer() );

    G4int  hcId( digiManager->GetHitsCollectionID( "vMonitor/Monitor/ED" ) );
    CexmcEnergyDepositCollection *  monitorED(
                                            new CexmcEnergyDepositCollection );
    hcOfThisEvent->AddHitsCollection( hcId, monitorED );
    hcId = digiManager->GetHitsCollectionID( "vVetoCounter/VetoCounter/ED" );
    CexmcEnergyDepositCollection *  vetoCounterED(
                                            new CexmcEnergyDepositCollection );
    hcOfThisEvent->AddHitsCollection( hcId, vetoCounterED );
    hcId = digiManager->GetHitsCollectionID( "vCrystal/Calorimeter/ED" );
    CexmcEnergyDepositCollection *  calorimeterED(
                                            new CexmcEnergyDepositCollection );
    hcOfThisEvent->AddHitsCollection( hcId, calorimeterED );
    hcId = digiManager->GetHitsCollectionID( "vMonitor/Monitor/TP" );
    CexmcTrackPointsCollection *  monitorTP( new CexmcTrackPointsCollection );
    hcOfThisEvent->AddHitsCollection( hcId, monitorTP );
    hcId = digiManager->GetHitsCollectionID( "vTarget/Target/TP" );
    CexmcTrackPointsCollection *  targetTP( new CexmcTrackPointsCollection );
    hcOfThisEvent->AddHitsCollection( hcId, targetTP );
    hcId = digiManager->GetHitsCollectionID( "vVetoCounter/VetoCounter/TP" );
    CexmcTrackPointsCollection *  vetoCounterTP(
                                            new CexmcTrackPointsCollection );
    hcOfThisEvent->AddHitsCollection( hcId, vetoCounterTP );
    hcId = digiManager->GetHitsCollectionID( "vCrystal/Calorimeter/TP" );
    CexmcTrackPointsCollection *  calorimeterTP(
                                            new CexmcTrackPointsCollection );
    hcOfThisEvent->AddHitsCollection( hcId, calorimeterTP );

    for ( int  i( 0 ); i < sObject.nmbOfSavedFastEvents; ++i )
    {
        evFastArchive >> evFastSObject;

        productionModel->SetTriggeredAngularRanges(
                                                evFastSObject.opCosThetaSCM );

        if ( nEventCount < curEventRead )
        {
            if ( evFastSObject.edDigitizerHasTriggered )
            {
                evArchive >> evSObject;
                ++nEventCount;
            }
            continue;
        }

        ++iEvent;

        if ( ! evFastSObject.edDigitizerHasTriggered )
        {
            const CexmcAngularRangeList &  triggeredAngularRanges(
                                productionModel->GetTriggeredAngularRanges() );
            for ( CexmcAngularRangeList::const_iterator
                        k( triggeredAngularRanges.begin() );
                                        k != triggeredAngularRanges.end(); ++k )
            {
                const CexmcRun *  run( static_cast< const CexmcRun * >(
                                                            GetCurrentRun() ) );
                CexmcRun *  theRun( const_cast< CexmcRun * >( run ) );
                theRun->IncrementNmbOfHitsSampledFull( k->index );
                if ( evFastSObject.edDigitizerMonitorHasTriggered )
                    theRun->IncrementNmbOfHitsSampled( k->index );
            }
            continue;
        }

        evArchive >> evSObject;

        event.SetEventID( evSObject.eventId );

        monitorED->GetMap()->operator[]( 0 ) = &evSObject.monitorED;
        vetoCounterED->GetMap()->operator[]( 0 ) = &evSObject.vetoCounterEDLeft;
        vetoCounterED->GetMap()->operator[]( 1 <<
                CexmcEnergyDepositInLeftRightSet::GetLeftRightBitsOffset() ) =
                                                &evSObject.vetoCounterEDRight;
        G4int  row( 0 );
        G4int  column( 0 );
        for ( CexmcEnergyDepositCalorimeterCollection::iterator
                k( evSObject.calorimeterEDLeftCollection.begin() );
                    k != evSObject.calorimeterEDLeftCollection.end(); ++k )
        {
            G4int  index( row <<
                CexmcEnergyDepositInCalorimeter::GetCopyDepth1BitsOffset() );
            column = 0;
            for ( CexmcEnergyDepositCrystalRowCollection::iterator
                    l( k->begin() ); l != k->end(); ++l )
            {
                calorimeterED->GetMap()->operator[]( index | column ) = &*l;
                ++column;
            }
            ++row;
        }
        row = 0;
        for ( CexmcEnergyDepositCalorimeterCollection::iterator
                k( evSObject.calorimeterEDRightCollection.begin() );
                    k != evSObject.calorimeterEDRightCollection.end(); ++k )
        {
            G4int  index(
                1 << CexmcEnergyDepositInLeftRightSet::GetLeftRightBitsOffset()
                | row <<
                CexmcEnergyDepositInCalorimeter::GetCopyDepth1BitsOffset() );
            column = 0;
            for ( CexmcEnergyDepositCrystalRowCollection::iterator
                    l( k->begin() ); l != k->end(); ++l )
            {
                calorimeterED->GetMap()->operator[]( index | column ) = &*l;
                ++column;
            }
            ++row;
        }

        CexmcTrackPointInfo  monitorTPInfo( evSObject.monitorTP );
        CexmcTrackPointInfo  targetTPIncidentParticleInfo(
                                        evSObject.targetTPIncidentParticle );
        CexmcTrackPointInfo  targetTPOutputParticleInfo(
                                        evSObject.targetTPOutputParticle );
        CexmcTrackPointInfo  targetTPNucleusParticleInfo(
                                        evSObject.targetTPNucleusParticle );
        CexmcTrackPointInfo  targetTPOutputParticleDecayProductParticle1Info(
                        evSObject.targetTPOutputParticleDecayProductParticle1 );
        CexmcTrackPointInfo  targetTPOutputParticleDecayProductParticle2Info(
                        evSObject.targetTPOutputParticleDecayProductParticle2 );
        CexmcTrackPointInfo  vetoCounterTPLeftInfo(
                                                evSObject.vetoCounterTPLeft );
        CexmcTrackPointInfo  vetoCounterTPRightInfo(
                                                evSObject.vetoCounterTPRight );
        CexmcTrackPointInfo  calorimeterTPLeftInfo(
                                                evSObject.calorimeterTPLeft );
        CexmcTrackPointInfo  calorimeterTPRightInfo(
                                                evSObject.calorimeterTPRight );

        if ( monitorTPInfo.IsValid() )
            monitorTP->GetMap()->operator[]( monitorTPInfo.trackId ) =
                                                &monitorTPInfo;
        if ( targetTPIncidentParticleInfo.IsValid() )
            targetTP->GetMap()->operator[](
                    targetTPIncidentParticleInfo.trackId ) =
                                                &targetTPIncidentParticleInfo;
        if ( targetTPOutputParticleInfo.IsValid() )
            targetTP->GetMap()->operator[](
                    targetTPOutputParticleInfo.trackId ) =
                                                &targetTPOutputParticleInfo;
        if ( targetTPNucleusParticleInfo.IsValid() )
            targetTP->GetMap()->operator[](
                    targetTPNucleusParticleInfo.trackId ) =
                                                &targetTPNucleusParticleInfo;
        if ( targetTPOutputParticleDecayProductParticle1Info.IsValid() )
            targetTP->GetMap()->operator[](
                    targetTPOutputParticleDecayProductParticle1Info.trackId ) =
                            &targetTPOutputParticleDecayProductParticle1Info;
        if ( targetTPOutputParticleDecayProductParticle2Info.IsValid() )
            targetTP->GetMap()->operator[](
                    targetTPOutputParticleDecayProductParticle2Info.trackId ) =
                            &targetTPOutputParticleDecayProductParticle2Info;
        if ( vetoCounterTPLeftInfo.IsValid() )
            vetoCounterTP->GetMap()->operator[](
                    vetoCounterTPLeftInfo.trackId ) = &vetoCounterTPLeftInfo;
        if ( vetoCounterTPRightInfo.IsValid() )
            vetoCounterTP->GetMap()->operator[](
                1 << CexmcTrackPointsInLeftRightSet::GetLeftRightBitsOffset() |
                    vetoCounterTPRightInfo.trackId ) = &vetoCounterTPRightInfo;

        G4ThreeVector  pos;
        if ( calorimeterTPLeftInfo.IsValid() )
        {
            pos = calorimeterTPLeftInfo.positionLocal;
            CexmcCalorimeterGeometry::ConvertToCrystalGeometry(
                    calorimeterTPLeftInfo.positionLocal, row, column, pos );
            calorimeterTPLeftInfo.positionLocal = pos;
            calorimeterTP->GetMap()->operator[](
                row << CexmcTrackPointsInCalorimeter::
                                                GetCopyDepth1BitsOffset() |
                column << CexmcTrackPointsInCalorimeter::
                                                GetCopyDepth0BitsOffset() |
                calorimeterTPLeftInfo.trackId ) = &calorimeterTPLeftInfo;
        }
        if ( calorimeterTPRightInfo.IsValid() )
        {
            pos = calorimeterTPRightInfo.positionLocal;
            CexmcCalorimeterGeometry::ConvertToCrystalGeometry(
                    calorimeterTPRightInfo.positionLocal, row, column, pos );
            calorimeterTPRightInfo.positionLocal = pos;
            calorimeterTP->GetMap()->operator[](
                1 << CexmcTrackPointsInLeftRightSet::GetLeftRightBitsOffset() |
                row << CexmcTrackPointsInCalorimeter::
                                                GetCopyDepth1BitsOffset() |
                column << CexmcTrackPointsInCalorimeter::
                                                GetCopyDepth0BitsOffset() |
                calorimeterTPRightInfo.trackId ) = &calorimeterTPRightInfo;
        }

        productionModel->SetProductionModelData(
                                            evSObject.productionModelData );

        const CexmcEventAction *  eventAction(
                static_cast< const CexmcEventAction * >( userEventAction ) );
        if ( ! eventAction )
            throw CexmcException( CexmcWeirdException );

        CexmcEventAction *  theEventAction( const_cast< CexmcEventAction * >(
                                                                eventAction ) );
        theEventAction->EndOfEventAction( &event );

        CexmcEventInfo *  eventInfo( static_cast< CexmcEventInfo * >(
                                                event.GetUserInformation() ) );

        if ( eventInfo->EdTriggerIsOk() )
            ++iEventEffective;

        delete event.GetUserInformation();
        event.SetUserInformation( NULL );

        monitorED->GetMap()->clear();
        vetoCounterED->GetMap()->clear();
        calorimeterED->GetMap()->clear();
        monitorTP->GetMap()->clear();
        targetTP->GetMap()->clear();
        vetoCounterTP->GetMap()->clear();
        calorimeterTP->GetMap()->clear();

        if ( nEvent > 0 && iEventEffective == nEvent )
            break;
    }

    curEventRead = nEventCount + iEventEffective;

    numberOfEventsProcessed = iEvent;
    numberOfEventsProcessedEffective = iEventEffective;
}


/* mostly adopted from G4RunManager::DoEventLoop() */
void  CexmcRunManager::DoEventLoop( G4int  nEvent, const char *  macroFile,
                                    G4int  nSelect )
{
    if ( verboseLevel > 0 ) 
        timer->Start();

    G4String  cmd;
    if ( macroFile != 0 )
    {
        if ( nSelect < 0 )
            nSelect = nEvent;
        cmd = "/control/execute ";
        cmd += macroFile;
    }
    else
    {
        nSelect = -1;
    }

    eventsArchive = NULL;
    fastEventsArchive = NULL;
    numberOfEventsProcessed = 0;
    numberOfEventsProcessedEffective = 0;

    if ( ProjectIsRead() )
    {
        if ( ProjectIsSaved() )
        {
            std::ofstream   eventsDataFile(
                        ( projectsDir + "/" + projectId + ".edb" ).c_str() );
            boost::archive::binary_oarchive  eventsArchive_( eventsDataFile );
            eventsArchive = &eventsArchive_;
            std::ofstream   fastEventsDataFile(
                        ( projectsDir + "/" + projectId + ".fdb" ).c_str() );
            boost::archive::binary_oarchive  fastEventsArchive_(
                                                        fastEventsDataFile );
            eventsArchive = &eventsArchive_;
            fastEventsArchive = &fastEventsArchive_;
            DoReadEventLoop( nEvent );
        }
        else
        {
            DoReadEventLoop( nEvent );
        }
    }
    else
    {
        if ( ProjectIsSaved() )
        {
            std::ofstream   eventsDataFile(
                        ( projectsDir + "/" + projectId + ".edb" ).c_str() );
            boost::archive::binary_oarchive  eventsArchive_( eventsDataFile );
            std::ofstream   fastEventsDataFile(
                        ( projectsDir + "/" + projectId + ".fdb" ).c_str() );
            boost::archive::binary_oarchive  fastEventsArchive_(
                                                        fastEventsDataFile );
            eventsArchive = &eventsArchive_;
            fastEventsArchive = &fastEventsArchive_;
            DoCommonEventLoop( nEvent, cmd, nSelect );
        }
        else
        {
            DoCommonEventLoop( nEvent, cmd, nSelect );
        }
    }

    if( verboseLevel > 0 )
    {
        timer->Stop();
        G4cout << "Run terminated." << G4endl;
        G4cout << "Run Summary" << G4endl;
        if( runAborted )
        {
            G4cout << "  Run Aborted after " << numberOfEventsProcessed <<
                      " events processed." << G4endl;
        }
        else
        {
            G4cout << "  Number of events processed : " <<
                      numberOfEventsProcessed << ", effectively: " <<
                      numberOfEventsProcessedEffective << G4endl;
        }
        G4cout << "  "  << *timer << G4endl;
    }

    eventsArchive = NULL;
    fastEventsArchive = NULL;
}


void  CexmcRunManager::PrintReadRunData( void ) const
{
    if ( ! ProjectIsRead() )
        return;

    G4cout << CEXMC_LINE_START << "Run data read from project '" << rProject <<
              "'" << G4endl;
    G4cout << "  -- Production model (1 - pi0, 2 - eta): " <<
              sObject.productionModelType << G4endl;
    G4cout << "  -- Geometry definition file: " << sObject.gdmlFileName <<
              G4endl;
    G4cout << "  -- Angular ranges: " << sObject.angularRanges << G4endl;
    G4cout << "  -- Eta decay modes: " << G4endl;
    G4Eta::Definition()->GetDecayTable()->DumpInfo();
    G4cout << "  -- Fermi motion status (0 - disabled, 1 - enabled): " <<
              sObject.fermiMotionIsOn << G4endl;
    G4cout << "  -- Event count policy (0 - all, 1 - interaction, 2 - trigger)"
              ": " << sObject.eventCountPolicy << G4endl;
    G4cout << "  -- Number of events (processed / effective / ordered): " <<
              sObject.numberOfEventsProcessed << " / " <<
              sObject.numberOfEventsProcessedEffective << " / " <<
              sObject.numberOfEventsToBeProcessed << G4endl;
    G4cout << "  -- Incident particle: " << sObject.incidentParticle << G4endl;
    G4cout << "              position: " <<
              G4BestUnit( sObject.beamPos, "Length" ) << G4endl;
    G4cout << "             direction: " << G4ThreeVector( sObject.beamDir ) <<
              G4endl;
    G4cout << "              momentum: " <<
              G4BestUnit( sObject.beamMomentumAmp, "Energy" ) << G4endl;
    G4cout << "         momentum fwhm: " << sObject.beamFwhmMomentumAmp <<
              G4endl;
    G4cout << "          pos fwhm (x): " <<
              G4BestUnit( sObject.beamFwhmPosX, "Length" ) << G4endl;
    G4cout << "          pos fwhm (y): " <<
              G4BestUnit( sObject.beamFwhmPosY, "Length" ) << G4endl;
    G4cout << "          dir fwhm (x): " << sObject.beamFwhmDirX / deg <<
              " deg" << G4endl;
    G4cout << "          dir fwhm (y): " << sObject.beamFwhmDirY / deg <<
              " deg" << G4endl;
    G4cout << "  -- Monitor ED threshold: " <<
              G4BestUnit( sObject.monitorEDThreshold, "Energy" ) << G4endl;
    G4cout << "  -- Veto counter (l/r) ED threshold: " <<
              G4BestUnit( sObject.vetoCounterEDLeftThreshold, "Energy" ) <<
              " / " <<
              G4BestUnit( sObject.vetoCounterEDRightThreshold, "Energy" ) <<
              G4endl;
    G4cout << "  -- Calorimeter (l/r) ED threshold: " <<
              G4BestUnit( sObject.calorimeterEDLeftThreshold, "Energy" ) <<
              " / " <<
              G4BestUnit( sObject.calorimeterEDRightThreshold, "Energy" ) <<
              G4endl;
    G4cout << "  -- Calorimeter trigger algorithm (0 - all, 1 - inner): " <<
              sObject.calorimeterTriggerAlgorithm << G4endl;
    G4cout << "  -- Outer crystals veto algorithm "
              "(0 - none, 1 - max, 2 - fraction): " <<
              sObject.outerCrystalsVetoAlgorithm << G4endl;
    if ( sObject.outerCrystalsVetoAlgorithm ==
         CexmcFractionOfEDInOuterCrystalsVeto )
    {
        G4cout << "  -- Outer crystals veto fraction: " <<
                  sObject.outerCrystalsVetoFraction << G4endl;
    }
    G4cout << "  -- Finite crystal resolution applied (0 - no, 1 - yes): " <<
              sObject.applyFiniteCrystalResolution << G4endl;
    if ( sObject.applyFiniteCrystalResolution )
    {
        G4cout << "  -- Crystal resolution data: " <<
                  sObject.crystalResolutionData;
    }
    G4cout << "  -- Reconstructor settings: " << G4endl;
    G4cout << "     -- entry point definition algorithm " << G4endl;
    G4cout << "        (0 - center of calorimeter, 1 - center of crystal with "
                       "max ED," << G4endl;
    G4cout << "         2 - linear, 3 - square): " <<
              sObject.epDefinitionAlgorithm << G4endl;
    G4cout << "     -- entry point depth definition algorithm "
                      "(0 - plain, 1 - sphere): " <<
                          sObject.epDepthDefinitionAlgorithm << G4endl;
    G4cout << "     -- entry point depth: " <<
              G4BestUnit( sObject.epDepth, "Length" ) << G4endl;
    G4cout << "     -- crystal selection algorithm (0 - all, 1 - adjacent): " <<
              sObject.csAlgorithm << G4endl;
    G4cout << "     -- table mass of output particle used "
                      "(0 - no, 1 - yes): " << sObject.useTableMass << G4endl;
    G4cout << "     -- mass cut is enabled (0 - no, 1 - yes): " <<
              sObject.useMassCut << G4endl;
    if ( sObject.useMassCut )
    {
        G4cout << "     -- mass cut output particle center: " <<
                  G4BestUnit( sObject.mCutOPCenter, "Energy" ) << G4endl;
        G4cout << "     -- mass cut nucleus output particle center: " <<
                  G4BestUnit( sObject.mCutNOPCenter, "Energy" ) << G4endl;
        G4cout << "     -- mass cut output particle width of the ellipse: " <<
                  G4BestUnit( sObject.mCutOPWidth, "Energy" ) << G4endl;
        G4cout << "     -- mass cut nucleus output particle width of the "
                          "ellipse: "
               << G4BestUnit( sObject.mCutNOPWidth, "Energy" ) << G4endl;
        G4cout << "     -- mass cut angle of the ellipse: " <<
                  sObject.mCutAngle / deg << " deg" << G4endl;
    }
    G4cout << "     -- absorbed energy cut is enabled (0 - no, 1 - yes): " <<
              sObject.useAbsorbedEnergyCut << G4endl;
    if ( sObject.useAbsorbedEnergyCut )
    {
        G4cout << "     -- absorbed energy cut left calorimeter center: " <<
                  G4BestUnit( sObject.aeCutCLCenter, "Energy" ) << G4endl;
        G4cout << "     -- absorbed energy cut right calorimeter center: " <<
                  G4BestUnit( sObject.aeCutCRCenter, "Energy" ) << G4endl;
        G4cout << "     -- absorbed energy cut left calorimeter width of the "
                          "ellipse: " <<
                  G4BestUnit( sObject.aeCutCLWidth, "Energy" ) << G4endl;
        G4cout << "     -- absorbed energy cut right calorimeter width of the "
                          "ellipse: "
               << G4BestUnit( sObject.aeCutCRWidth, "Energy" ) << G4endl;
        G4cout << "     -- absorbed energy cut angle of the ellipse: " <<
                  sObject.aeCutAngle / deg << " deg" << G4endl;
    }
    G4cout << "  -- Setup acceptances (real, rec): " << G4endl;
    CexmcRunAction::PrintResults( sObject.nmbOfHitsSampled,
                                  sObject.nmbOfHitsSampledFull,
                                  sObject.nmbOfHitsTriggeredRealRange,
                                  sObject.nmbOfHitsTriggeredRecRange,
                                  sObject.nmbOfOrphanHits,
                                  sObject.angularRanges,
                                  sObject.nmbOfFalseHitsTriggeredEDT,
                                  sObject.nmbOfFalseHitsTriggeredRec );

    G4cout << G4endl;
}


void  CexmcRunManager::ReadAndPrintEventsData( void ) const
{
    CexmcEventSObject  evSObject;

    /* read events data */
    std::ifstream   eventsDataFile(
                        ( projectsDir + "/" + rProject + ".edb" ).c_str() );
    if ( ! eventsDataFile )
        throw CexmcException( CexmcReadProjectIncompleteException );

    boost::archive::binary_iarchive  evArchive( eventsDataFile );

    for ( int  i( 0 ); i < sObject.nmbOfSavedEvents; ++i )
    {
        evArchive >> evSObject;

        CexmcEnergyDepositStore  edStore( evSObject.monitorED,
            evSObject.vetoCounterEDLeft, evSObject.vetoCounterEDRight,
            evSObject.calorimeterEDLeft, evSObject.calorimeterEDRight,
            0, 0, 0, 0, evSObject.calorimeterEDLeftCollection,
            evSObject.calorimeterEDRightCollection );

        CexmcTrackPointsStore    tpStore( evSObject.monitorTP,
            evSObject.targetTPIncidentParticle,
            evSObject.targetTPOutputParticle, evSObject.targetTPNucleusParticle,
            evSObject.targetTPOutputParticleDecayProductParticle1,
            evSObject.targetTPOutputParticleDecayProductParticle2,
            evSObject.vetoCounterTPLeft, evSObject.vetoCounterTPRight,
            evSObject.calorimeterTPLeft, evSObject.calorimeterTPRight );

        const CexmcProductionModelData &  pmData(
                                            evSObject.productionModelData );

        G4cout << "Event " << evSObject.eventId << G4endl;
        CexmcEventAction::PrintTrackPoints( &tpStore );
        G4cout << " --- Production model data: " << pmData;
        CexmcEventAction::PrintEnergyDeposit( &edStore );
    }
}


void  CexmcRunManager::PrintReadData(
                            const CexmcOutputDataTypeSet &  outputData ) const
{
    CexmcOutputDataTypeSet::const_iterator  found(
                                    outputData.find( CexmcOutputGeometry ) );
    if ( found != outputData.end() )
    {
        G4String  cmd( G4String( "cat " ) + projectsDir + "/" + rProject +
                       gdmlFileExtension );
        if ( system( cmd ) != 0 )
            G4cerr << "Failed to cat geometry data" << G4endl;
    }

    found = outputData.find( CexmcOutputEvents );
    if ( found != outputData.end() )
    {
        ReadAndPrintEventsData();
    }

    found = outputData.find( CexmcOutputRun );
    if ( found != outputData.end() )
    {
        G4DecayTable *  etaDecayTable( G4Eta::Definition()->GetDecayTable() );
        for ( CexmcDecayBranchesStore::const_iterator
                k( sObject.etaDecayTable.GetDecayBranches().begin() );
                k != sObject.etaDecayTable.GetDecayBranches().end(); ++k )
        {
            etaDecayTable->GetDecayChannel( k->first )->SetBR( k->second );
        }
        PrintReadRunData();
    }
}

