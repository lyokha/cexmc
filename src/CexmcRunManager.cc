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
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <G4Eta.hh>
#include <G4DigiManager.hh>
#include <G4DecayTable.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTable.hh>
#include <G4UImanager.hh>
#include <G4Timer.hh>
#include "CexmcRunManager.hh"
#include "CexmcRunManagerMessenger.hh"
#include "CexmcPhysicsManager.hh"
#include "CexmcProductionModel.hh"
#include "CexmcSimpleDecayTableStore.hh"
#include "CexmcPrimaryGeneratorAction.hh"
#include "CexmcEnergyDepositDigitizer.hh"
#include "CexmcParticleGun.hh"
#include "CexmcEventInfo.hh"
#include "CexmcException.hh"


CexmcRunManager::CexmcRunManager( const G4String &  projectId,
                                  const G4String &  rProject ) :
    productionModelType( CexmcUnknownProductionModel ),
    gdmlFileName( "default.gdml" ), projectsDir( "." ), projectId( projectId ),
    rProject( rProject ), eventCountPolicy( CexmcCountAllEvents ),
    messenger( NULL )
{
    /* this exception must be caught before creating the object! */
    if ( rProject != "" && rProject == projectId )
        throw CexmcException( CexmcWeirdException );

    const char *  projectsDirEnv( getenv( "CEXMC_PROJECTS_DIR" ) );

    if ( projectsDirEnv )
        projectsDir = projectsDirEnv;

    messenger = new CexmcRunManagerMessenger( this );

    if ( ProjectIsRead() )
        ReadPreinitProjectData();
}


CexmcRunManager::~CexmcRunManager()
{
    if ( ProjectIsRead() )
    {
        G4String  cmd( G4String( "bzip2 " ) + projectsDir + "/" + rProject +
                       ".gdml" );
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
    G4String  cmd( G4String( "cp " ) + projectsDir + "/" + rProject +
                   ".gdml.bz2 " + projectsDir + "/" + projectId +
                   ".gdml.bz2" );
    if ( ProjectIsSaved() && system( cmd ) != 0 )
        throw CexmcException( CexmcReadProjectIncompleteException );

    cmd = G4String( "bunzip2 " ) + projectsDir + "/" + rProject + ".gdml.bz2";
    if ( system( cmd ) != 0 )
        throw CexmcException( CexmcFileCompressException );

    gdmlFileName = projectsDir + "/" + rProject + ".gdml";
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
                                                    sObject.fermiMotionIsOn );
    eventCountPolicy = sObject.eventCountPolicy;

    const CexmcPrimaryGeneratorAction *  primaryGeneratorAction(
                            static_cast< const CexmcPrimaryGeneratorAction * >(
                                            GetUserPrimaryGeneratorAction() ) );
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
    particleGun->SetOrigPosition( sObject.beamPos );
    particleGun->SetOrigDirection( sObject.beamDir );
    particleGun->SetOrigMomentumAmp( sObject.beamMomentumAmp );

    thePrimaryGeneratorAction->SetFwhmPosX( sObject.beamFwhmPosX );
    thePrimaryGeneratorAction->SetFwhmPosY( sObject.beamFwhmPosY );
    thePrimaryGeneratorAction->SetFwhmDirX( sObject.beamFwhmDirX );
    thePrimaryGeneratorAction->SetFwhmDirY( sObject.beamFwhmDirY );
    thePrimaryGeneratorAction->SetFwhmMomentumAmp(
                                            sObject.beamFwhmMomentumAmp );

    G4DigiManager *                digiManager( G4DigiManager::GetDMpointer() );
    CexmcEnergyDepositDigitizer *  edDigitizer(
            static_cast< CexmcEnergyDepositDigitizer * >(
                digiManager->FindDigitizerModule( CexmcEDDigitizerName ) ) );
    if ( ! edDigitizer )
        throw CexmcException( CexmcWeirdException );

    edDigitizer->SetMonitorThreshold( sObject.monitorEDThreshold );
    edDigitizer->SetVetoCounterLeftThreshold(
                                        sObject.vetoCounterEDLeftThreshold );
    edDigitizer->SetVetoCounterRightThreshold(
                                        sObject.vetoCounterEDRightThreshold );
    edDigitizer->SetCalorimeterLeftThreshold(
                                        sObject.calorimeterEDLeftThreshold );
    edDigitizer->SetCalorimeterRightThreshold(
                                        sObject.calorimeterEDRightThreshold );
    edDigitizer->SetOuterCrystalsVetoAlgorithm(
                                        sObject.outerCrystalsVetoAlgorithm );
    edDigitizer->SetOuterCrystalsVetoFraction(
                                        sObject.outerCrystalsVetoFraction );
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
                                            GetUserPrimaryGeneratorAction() ) );
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

    CexmcRunSObject             sObject(
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
        edDigitizer->GetOuterCrystalsVetoAlgorithm(),
        edDigitizer->GetOuterCrystalsVetoFraction() );

    std::ofstream   runDataFile( ( projectsDir + "/" + projectId + ".rdb" ).
                                        c_str() );

    {
        boost::archive::binary_oarchive  archive( runDataFile );
        archive << sObject;
    }

    /* save gdml file */
    G4String  cmd( G4String( "bzip2 -c " ) + gdmlFileName + " > " +
                   projectsDir + "/" + projectId + ".gdml.bz2" );
    if ( system( cmd ) != 0 )
        throw CexmcException( CexmcFileCompressException );
}


/* mostly adopted from G4RunManager::DoEventLoop() */
void  CexmcRunManager::DoEventLoop( G4int  nEvent, const char *  macroFile,
                                    G4int  nSelect )
{
    if ( verboseLevel > 0 ) 
        timer->Start();

    G4String  msg;
    if ( macroFile != 0 )
    {
        if ( nSelect < 0 )
            nSelect = nEvent;
        msg = "/control/execute ";
        msg += macroFile;
    }
    else
    {
        nSelect = -1;
    }

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
            G4UImanager::GetUIpointer()->ApplyCommand( msg );
        StackPreviousEvent( currentEvent );
        currentEvent = 0;
        if( runAborted )
            break;
    }

    if( verboseLevel > 0 )
    {
        timer->Stop();
        G4cout << "Run terminated." << G4endl;
        G4cout << "Run Summary" << G4endl;
        if( runAborted )
        {
            G4cout << "  Run Aborted after " << iEvent <<
                      " events processed." << G4endl;
        }
        else
        {
            G4cout << "  Number of events processed : " << iEvent <<
                      ", effectively: " << iEventEffective << G4endl;
        }
        G4cout << "  "  << *timer << G4endl;
    }
}


void  CexmcRunManager::PrintReadData( void ) const
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
    G4cout << "  -- Outer crystals veto alforithm "
              "(0 - none, 1 - max, 2 - fraction): " <<
               sObject.outerCrystalsVetoAlgorithm << G4endl;
    G4cout << "  -- Outer crystals veto fraction: " <<
              sObject.outerCrystalsVetoFraction << G4endl;


    G4cout << G4endl;
}

