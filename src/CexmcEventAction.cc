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
#include "CexmcCommon.hh"


namespace
{
    const G4String  energyDepositDigitizerName( "EDDig" );
}


CexmcEventAction::CexmcEventAction( CexmcPhysicsManager *  physicsManager ) :
    physicsManager( physicsManager )
{
    G4DigiManager *  digiManager( G4DigiManager::GetDMpointer() );
    digiManager->AddNewModule( new CexmcEnergyDepositDigitizer(
                                                energyDepositDigitizerName ) );
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

    energyDepositDigitizer->Digitize();

    G4double  monitorED( energyDepositDigitizer->GetMonitorED() );
    G4double  vetoCounterEDLeft(
                            energyDepositDigitizer->GetVetoCounterEDLeft() );
    G4double  vetoCounterEDRight(
                            energyDepositDigitizer->GetVetoCounterEDRight() );
    G4double  calorimeterEDLeft(
                            energyDepositDigitizer->GetCalorimeterEDLeft() );
    G4double  calorimeterEDRight(
                            energyDepositDigitizer->GetCalorimeterEDRight() );
    const CexmcEnergyDepositCalorimeterCollection  calorimeterEDLeftCollection(
                    energyDepositDigitizer->GetCalorimeterEDLeftCollection() );
    const CexmcEnergyDepositCalorimeterCollection  calorimeterEDRightCollection(
                    energyDepositDigitizer->GetCalorimeterEDRightCollection() );
    G4cout << "MONITOR ED: " << monitorED << G4endl;
    G4cout << "L VETO COUNTER ED: " << vetoCounterEDLeft << G4endl;
    G4cout << "R VETO COUNTER ED: " << vetoCounterEDRight << G4endl;
    G4cout << "L CALORIMETER ED: " << calorimeterEDLeft << G4endl;
    G4cout << "R CALORIMETER ED: " << calorimeterEDRight << G4endl;
    G4cout << "L CALORIMETER COL ED: " << calorimeterEDLeftCollection;
    G4cout << "R CALORIMETER COL ED: " << calorimeterEDRightCollection;
}

