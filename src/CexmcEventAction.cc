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
#include <G4ProcessManager.hh>
#include <G4ProcessVector.hh>
#include <G4DigiManager.hh>
#include "CexmcEventAction.hh"
#include "CexmcPhysicsManager.hh"
#include "CexmcEnergyDepositDigitizer.hh"
#include "CexmcTrackPointsDigitizer.hh"
#include "CexmcTrackPointInfo.hh"
#include "CexmcCommon.hh"


namespace
{
    const G4String  energyDepositDigitizerName( "EDDig" );
    const G4String  trackPointsDigitizerName( "TPDig" );
}


CexmcEventAction::CexmcEventAction( CexmcPhysicsManager *  physicsManager ) :
    physicsManager( physicsManager )
{
    G4DigiManager *  digiManager( G4DigiManager::GetDMpointer() );
    digiManager->AddNewModule( new CexmcEnergyDepositDigitizer(
                                                energyDepositDigitizerName ) );
    digiManager->AddNewModule( new CexmcTrackPointsDigitizer(
                                                trackPointsDigitizerName ) );
}


void  CexmcEventAction::BeginOfEventAction( const G4Event * )
{
    const G4ParticleDefinition *  particle(
                                    physicsManager->GetIncidentParticleType() );
    G4ProcessManager *            processManager(
                                    particle->GetProcessManager() );
    G4ProcessVector *             processVector(
                                    processManager->GetProcessList() );
    G4int                         processVectorSize(
                                    processManager->GetProcessListLength() );

    for ( int  i( 0 ); i < processVectorSize; ++i )
    {
        if ( ( *processVector )[ i ]->GetProcessName() ==
                                                CexmcStudiedProcessFullName )
        {
            physicsManager->ActivateStudiedProcess( false );
            break;
        }
    }
}


void  CexmcEventAction::EndOfEventAction( const G4Event * )
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

    G4double  monitorED( energyDepositDigitizer->GetMonitorED() );
    G4double  vetoCounterEDLeft(
                            energyDepositDigitizer->GetVetoCounterEDLeft() );
    G4double  vetoCounterEDRight(
                            energyDepositDigitizer->GetVetoCounterEDRight() );
    G4double  calorimeterEDLeft(
                            energyDepositDigitizer->GetCalorimeterEDLeft() );
    G4double  calorimeterEDRight(
                            energyDepositDigitizer->GetCalorimeterEDRight() );
    const CexmcEnergyDepositCalorimeterCollection &
                calorimeterEDLeftCollection(
                    energyDepositDigitizer->GetCalorimeterEDLeftCollection() );
    const CexmcEnergyDepositCalorimeterCollection &
                calorimeterEDRightCollection(
                    energyDepositDigitizer->GetCalorimeterEDRightCollection() );
    G4cout << "MONITOR ED: " << monitorED << G4endl;
    G4cout << "L VETO COUNTER ED: " << vetoCounterEDLeft << G4endl;
    G4cout << "R VETO COUNTER ED: " << vetoCounterEDRight << G4endl;
    G4cout << "L CALORIMETER ED: " << calorimeterEDLeft << G4endl;
    G4cout << "R CALORIMETER ED: " << calorimeterEDRight << G4endl;
    G4cout << "L CALORIMETER COL ED: " << calorimeterEDLeftCollection;
    G4cout << "R CALORIMETER COL ED: " << calorimeterEDRightCollection;

    const CexmcTrackPointInfo &
                monitorTP( trackPointsDigitizer->GetMonitorTP() );
    const CexmcTrackPointInfo &
                targetTPIncidentParticle(
                    trackPointsDigitizer->GetTargetTPIncidentParticle() );
    const CexmcTrackPointInfo &
                targetTPOutputParticle(
                    trackPointsDigitizer->GetTargetTPOutputParticle() );
    const CexmcTrackPointInfo &
                targetTPNucleusParticle(
                    trackPointsDigitizer->GetTargetTPNucleusParticle() );
    const CexmcTrackPointInfo &
                targetTPOutputParticleDecayProductParticle1(
                    trackPointsDigitizer->
                    GetTargetTPOutputParticleDecayProductParticle( 0 ) );
    const CexmcTrackPointInfo &
                targetTPOutputParticleDecayProductParticle2(
                    trackPointsDigitizer->
                    GetTargetTPOutputParticleDecayProductParticle( 1 ) );
    const CexmcTrackPointInfo &
                vetoCounterTPLeft(
                    trackPointsDigitizer->GetVetoCounterTPLeft() );
    const CexmcTrackPointInfo &
                vetoCounterTPRight(
                    trackPointsDigitizer->GetVetoCounterTPRight() );
    const CexmcTrackPointInfo &
                calorimeterTPLeft(
                    trackPointsDigitizer->GetCalorimeterTPLeft() );
    const CexmcTrackPointInfo &
                calorimeterTPRight(
                    trackPointsDigitizer->GetCalorimeterTPRight() );
    G4cout << "MONITOR TP: " << monitorTP << G4endl;
    G4cout << "TARGET TP: " << targetTPIncidentParticle << G4endl;
    G4cout << "         : " << targetTPOutputParticle << G4endl;
    G4cout << "         : " << targetTPNucleusParticle << G4endl;
    G4cout << "         : " << targetTPOutputParticleDecayProductParticle1 <<
            G4endl;
    G4cout << "         : " << targetTPOutputParticleDecayProductParticle2 <<
            G4endl;
    G4cout << "VC L TP: " << vetoCounterTPLeft << G4endl;
    G4cout << "VC R TP: " << vetoCounterTPRight << G4endl;
    G4cout << "CAL L TP: " << calorimeterTPLeft << G4endl;
    G4cout << "CAL R TP: " << calorimeterTPRight << G4endl;
}

