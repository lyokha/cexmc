/*
 * ============================================================================
 *
 *       Filename:  CexmcEnergyDepositDigitizerMessenger.cc
 *
 *    Description:  energy deposit digitizer messenger
 *
 *        Version:  1.0
 *        Created:  29.11.2009 19:07:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithAString.hh>
#include "CexmcEnergyDepositDigitizer.hh"
#include "CexmcEnergyDepositDigitizerMessenger.hh"
#include "CexmcMessenger.hh"
#include "CexmcCommon.hh"


CexmcEnergyDepositDigitizerMessenger::CexmcEnergyDepositDigitizerMessenger(
        CexmcEnergyDepositDigitizer *  energyDepositDigitizer ) :
    energyDepositDigitizer( energyDepositDigitizer ),
    setMonitorThreshold( NULL ), setVetoCountersThreshold( NULL ),
    setLeftVetoCounterThreshold( NULL ), setRightVetoCounterThreshold( NULL ),
    setCalorimetersThreshold( NULL ), setLeftCalorimeterThreshold( NULL ),
    setRightCalorimeterThreshold( NULL ), setOuterCrystalsVetoAlgorithm( NULL ),
    setOuterCrystalsVetoFraction( NULL )
{
    setMonitorThreshold = new G4UIcmdWithADoubleAndUnit(
            ( CexmcMessenger::monitorEDDirName + "threshold" ).c_str(), this );
    setMonitorThreshold->SetGuidance( "Monitor trigger threshold" );
    setMonitorThreshold->SetParameterName( "MonitorThreshold", false );
    setMonitorThreshold->SetDefaultValue( 0 );
    setMonitorThreshold->SetDefaultUnit( "MeV" );
    setMonitorThreshold->SetUnitCandidates( "ev keV MeV GeV" );
    setMonitorThreshold->AvailableForStates( G4State_PreInit, G4State_Idle );

    setVetoCountersThreshold = new G4UIcmdWithADoubleAndUnit(
            ( CexmcMessenger::vetoCounterEDDirName + "threshold" ).c_str(),
            this );
    setVetoCountersThreshold->SetGuidance( "Veto counters trigger threshold" );
    setVetoCountersThreshold->SetParameterName( "VetoCountersThreshold",
                                                false );
    setVetoCountersThreshold->SetDefaultValue( 0 );
    setVetoCountersThreshold->SetDefaultUnit( "MeV" );
    setVetoCountersThreshold->SetUnitCandidates( "ev keV MeV GeV" );
    setVetoCountersThreshold->AvailableForStates( G4State_PreInit,
                                                  G4State_Idle );

    setLeftVetoCounterThreshold = new G4UIcmdWithADoubleAndUnit(
            ( CexmcMessenger::vetoCounterLeftEDDirName + "threshold" ).c_str(),
            this );
    setLeftVetoCounterThreshold->SetGuidance(
                                    "Left veto counter trigger threshold" );
    setLeftVetoCounterThreshold->SetParameterName( "LeftVetoCounterThreshold",
                                                   false );
    setLeftVetoCounterThreshold->SetDefaultValue( 0 );
    setLeftVetoCounterThreshold->SetDefaultUnit( "MeV" );
    setLeftVetoCounterThreshold->SetUnitCandidates( "ev keV MeV GeV" );
    setLeftVetoCounterThreshold->AvailableForStates( G4State_PreInit,
                                                     G4State_Idle );

    setRightVetoCounterThreshold = new G4UIcmdWithADoubleAndUnit(
            ( CexmcMessenger::vetoCounterRightEDDirName + "threshold" ).c_str(),
            this );
    setRightVetoCounterThreshold->SetGuidance(
                                    "Right veto counter trigger threshold" );
    setRightVetoCounterThreshold->SetParameterName( "RightVetoCounterThreshold",
                                                    false );
    setRightVetoCounterThreshold->SetDefaultValue( 0 );
    setRightVetoCounterThreshold->SetDefaultUnit( "MeV" );
    setRightVetoCounterThreshold->SetUnitCandidates( "ev keV MeV GeV" );
    setRightVetoCounterThreshold->AvailableForStates( G4State_PreInit,
                                                      G4State_Idle );

    setCalorimetersThreshold = new G4UIcmdWithADoubleAndUnit(
            ( CexmcMessenger::calorimeterEDDirName + "threshold" ).c_str(),
            this );
    setCalorimetersThreshold->SetGuidance( "Calorimeters trigger threshold" );
    setCalorimetersThreshold->SetParameterName( "CalorimetersThreshold",
                                                false );
    setCalorimetersThreshold->SetDefaultValue( 0 );
    setCalorimetersThreshold->SetDefaultUnit( "MeV" );
    setCalorimetersThreshold->SetUnitCandidates( "ev keV MeV GeV" );
    setCalorimetersThreshold->AvailableForStates( G4State_PreInit,
                                                  G4State_Idle );

    setLeftCalorimeterThreshold = new G4UIcmdWithADoubleAndUnit(
            ( CexmcMessenger::calorimeterLeftEDDirName + "threshold" ).c_str(),
            this );
    setLeftCalorimeterThreshold->SetGuidance(
                                    "Left calorimeter trigger threshold" );
    setLeftCalorimeterThreshold->SetParameterName( "LeftCalorimeterThreshold",
                                                   false );
    setLeftCalorimeterThreshold->SetDefaultValue( 0 );
    setLeftCalorimeterThreshold->SetDefaultUnit( "MeV" );
    setLeftCalorimeterThreshold->SetUnitCandidates( "ev keV MeV GeV" );
    setLeftCalorimeterThreshold->AvailableForStates( G4State_PreInit,
                                                     G4State_Idle );

    setRightCalorimeterThreshold = new G4UIcmdWithADoubleAndUnit(
            ( CexmcMessenger::calorimeterRightEDDirName + "threshold" ).c_str(),
            this );
    setRightCalorimeterThreshold->SetGuidance(
                                    "Right calorimeter trigger threshold" );
    setRightCalorimeterThreshold->SetParameterName( "RightCalorimeterThreshold",
                                                    false );
    setRightCalorimeterThreshold->SetDefaultValue( 0 );
    setRightCalorimeterThreshold->SetDefaultUnit( "MeV" );
    setRightCalorimeterThreshold->SetUnitCandidates( "ev keV MeV GeV" );
    setRightCalorimeterThreshold->AvailableForStates( G4State_PreInit,
                                                      G4State_Idle );

    setOuterCrystalsVetoAlgorithm = new G4UIcmdWithAString(
            ( CexmcMessenger::detectorDirName +
              "outerCrystalsVetoAlgorithm" ).c_str(), this );
    setOuterCrystalsVetoAlgorithm->SetGuidance( "\n"
                "    none - events will not be rejected by any algorithm,\n"
                "    max - reject event trigger if crystal with maximum energy "
                    "deposit\n          is one of outer crystals,\n"
                "    fraction - reject event trigger if energy deposit "
                    "fraction in\n               outer crystals is more than "
                    "value of 'outerCrystalsVetoFraction'" );
    setOuterCrystalsVetoAlgorithm->SetParameterName(
                                        "OuterCrystalsVetoAlgorithm", true );
    setOuterCrystalsVetoAlgorithm->SetDefaultValue( "none" );
    setOuterCrystalsVetoAlgorithm->SetCandidates( "none max fraction" );
    setOuterCrystalsVetoAlgorithm->AvailableForStates( G4State_PreInit,
                                                       G4State_Idle );

    setOuterCrystalsVetoFraction = new G4UIcmdWithADouble(
            ( CexmcMessenger::detectorDirName +
              "outerCrystalsVetoFraction" ).c_str(), this );
    setOuterCrystalsVetoFraction->SetGuidance( "\n    fraction of whole energy "
            "deposit in one calorimeter \n    that belongs to outer crystals.\n"
            "    If 'outerCrystalsVetoAlgorithm' is 'fraction' and\n"
            "    the outer crystals energy deposit fraction exceeds "
                "this value\n    then event won't trigger" );
    setOuterCrystalsVetoFraction->SetParameterName(
                                        "OuterCrystalsVetoFraction", true );
    setOuterCrystalsVetoFraction->SetDefaultValue( 0 );
    setOuterCrystalsVetoFraction->AvailableForStates( G4State_PreInit,
                                                      G4State_Idle );
}


CexmcEnergyDepositDigitizerMessenger::~CexmcEnergyDepositDigitizerMessenger()
{
    delete setMonitorThreshold;
    delete setVetoCountersThreshold;
    delete setLeftVetoCounterThreshold;
    delete setRightVetoCounterThreshold;
    delete setCalorimetersThreshold;
    delete setLeftCalorimeterThreshold;
    delete setRightCalorimeterThreshold;
    delete setOuterCrystalsVetoAlgorithm;
    delete setOuterCrystalsVetoFraction;
}


void  CexmcEnergyDepositDigitizerMessenger::SetNewValue( G4UIcommand *  cmd,
                                                         G4String  value )
{
    do
    {
        if ( cmd == setMonitorThreshold )
        {
            energyDepositDigitizer->SetMonitorThreshold(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
            break;
        }
        if ( cmd == setVetoCountersThreshold )
        {
            energyDepositDigitizer->SetVetoCountersThreshold(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
            break;
        }
        if ( cmd == setLeftVetoCounterThreshold )
        {
            energyDepositDigitizer->SetVetoCounterLeftThreshold(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
            break;
        }
        if ( cmd == setRightVetoCounterThreshold )
        {
            energyDepositDigitizer->SetVetoCounterRightThreshold(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
            break;
        }
        if ( cmd == setCalorimetersThreshold )
        {
            energyDepositDigitizer->SetCalorimetersThreshold(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
            break;
        }
        if ( cmd == setLeftCalorimeterThreshold )
        {
            energyDepositDigitizer->SetCalorimeterLeftThreshold(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
            break;
        }
        if ( cmd == setRightCalorimeterThreshold )
        {
            energyDepositDigitizer->SetCalorimeterRightThreshold(
                        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
            break;
        }
        if ( cmd == setOuterCrystalsVetoAlgorithm )
        {
            CexmcOuterCrystalsVetoAlgorithm outerCrystalsVetoAlgorithm(
                                                    CexmcNoOuterCrystalsVeto );
            do
            {
                if ( value == "max" )
                {
                    outerCrystalsVetoAlgorithm =
                                        CexmcMaximumEDInASingleOuterCrystalVeto;
                    break;
                }
                if ( value == "fraction" )
                {
                    outerCrystalsVetoAlgorithm =
                                        CexmcFractionOfEDInOuterCrystalsVeto;
                    break;
                }
            } while ( false );
            energyDepositDigitizer->SetOuterCrystalsVetoAlgorithm(
                                                outerCrystalsVetoAlgorithm );
            break;
        }
        if ( cmd == setOuterCrystalsVetoFraction )
        {
            energyDepositDigitizer->SetOuterCrystalsVetoFraction(
                        G4UIcmdWithADouble::GetNewDoubleValue( value ) );
            break;
        }
    } while ( false );
}

