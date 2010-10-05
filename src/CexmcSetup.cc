/*
 * =============================================================================
 *
 *       Filename:  CexmcSetup.cc
 *
 *    Description:  physical setup
 *
 *        Version:  1.0
 *        Created:  10.10.2009 23:00:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#include <G4GDMLParser.hh>
#include <G4MultiFunctionalDetector.hh>
#include <G4VPrimitiveScorer.hh>
#include <G4SDManager.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4Region.hh>
#include <G4RegionStore.hh>
#include <G4ProductionCuts.hh>
#include <G4RunManager.hh>
#include <G4VUserPhysicsList.hh>
#include "CexmcSetup.hh"
#include "CexmcTrackPoints.hh"
#include "CexmcTrackPointsInLeftRightSet.hh"
#include "CexmcTrackPointsInCalorimeter.hh"
#include "CexmcTrackPointsFilter.hh"
#include "CexmcSimpleEnergyDeposit.hh"
#include "CexmcEnergyDepositInLeftRightSet.hh"
#include "CexmcEnergyDepositInCalorimeter.hh"
#include "CexmcException.hh"
#include "CexmcCommon.hh"



CexmcSetup::CexmcSetup( const G4String &  gdmlFile, G4bool  validateGDMLFile ) :
    world( 0 ), gdmlFile( gdmlFile ), validateGDMLFile( validateGDMLFile ),
    calorimeterRegionInitialized( false )
{
}


G4VPhysicalVolume *  CexmcSetup::Construct( void )
{
    if ( world )
        return world;

    G4GDMLParser gdmlParser;

    gdmlParser.Read( gdmlFile, validateGDMLFile );
    world = gdmlParser.GetWorldVolume();

    SetupSensitiveVolumes( gdmlParser );

    return world;
}


void CexmcSetup::SetupSensitiveVolumes( G4GDMLParser &  gdmlParser )
{
    const G4LogicalVolumeStore *  lvs( G4LogicalVolumeStore::GetInstance() );

    for( std::vector< G4LogicalVolume * >::const_iterator
                        lvIter( lvs->begin() ); lvIter != lvs->end(); ++lvIter )
    {
        G4MultiFunctionalDetector *  detector( NULL );
        G4String                     detectorName( G4String( ( *lvIter )->
                                                             GetName() ) );
        G4GDMLAuxListType            auxInfo(
                        gdmlParser.GetVolumeAuxiliaryInformation( *lvIter ) );
        std::vector< G4GDMLAuxPairType >::const_iterator
                                     pair( auxInfo.begin() );

        for( pair = auxInfo.begin(); pair != auxInfo.end(); ++pair )
        {
            G4VPrimitiveScorer *  scorer( NULL );
            do
            {
                if ( pair->type == "EnergyDepositDetector" )
                {
                    G4cout << CEXMC_LINE_START "Energy Deposit Scorer for "
                               "detector '" << detectorName << "'" << G4endl;
                    do
                    {
                        if ( pair->value < 0.5 )
                        {
                            scorer = new CexmcSimpleEnergyDeposit(
                                                            "Monitor/ED" );
                            break;
                        }
                        if ( pair->value < 1.5 )
                        {
                            scorer = new CexmcEnergyDepositInLeftRightSet(
                                                            "VetoCounter/ED" );
                            break;
                        }
                        if ( pair->value < 2.5 )
                        {
                            scorer = new CexmcEnergyDepositInCalorimeter(
                                                            "Calorimeter/ED" );
                            break;
                        }
                    } while ( false );
                    break;
                }
                if ( pair->type == "TrackPointsDetector" )
                {
                    G4cout << CEXMC_LINE_START "Track Points Scorer for "
                               "detector '" << detectorName << "'" << G4endl;
                    do
                    {
                        if ( pair->value < 0.5 )
                        {
                            scorer = new CexmcTrackPoints( "Target/TP" );
                            break;
                        }
                        if ( pair->value < 1.5 )
                        {
                            scorer = new CexmcTrackPoints( "Monitor/TP" );
                            break;
                        }
                        if ( pair->value < 2.5 )
                        {
                            scorer = new CexmcTrackPointsInLeftRightSet(
                                                         "VetoCounter/TP" );
                            break;
                        }
                        if ( pair->value < 3.5 )
                        {
                            scorer = new CexmcTrackPointsInCalorimeter(
                                                         "Calorimeter/TP" );
                            break;
                        }
                    } while ( false );
                    if ( scorer )
                    {
                        CexmcTrackPointsFilter *  filter(
                                 new CexmcTrackPointsFilter( "trackPoints" ) );
                        scorer->SetFilter( filter );
                    }
                    break;
                }
                if ( pair->type == "SensitiveRegion" )
                {
                    G4cout << CEXMC_LINE_START "Sensitive Region for "
                               "detector '" << detectorName << "'" << G4endl;
                    do
                    {
                        if ( pair->value < 0.5 )
                        {
                            G4Region *  region( NULL );
                            if ( calorimeterRegionInitialized )
                            {
                                region = G4RegionStore::GetInstance()->
                                        GetRegion( CexmcCalorimeterRegionName );
                            }
                            else
                            {
                                region = new G4Region(
                                                CexmcCalorimeterRegionName );
                                G4ProductionCuts *  cuts(
                                                        new G4ProductionCuts );
                                G4double  defaultProductionCut( 1.0 * mm );
                                const G4VUserPhysicsList *  physicsList(
                                            G4RunManager::GetRunManager()->
                                                        GetUserPhysicsList() );
                                if ( physicsList )
                                    defaultProductionCut =
                                            physicsList->GetDefaultCutValue();
                                cuts->SetProductionCut( defaultProductionCut );
                                region->SetProductionCuts( cuts );
                                calorimeterRegionInitialized = true;
                            }
                            region->AddRootLogicalVolume( *lvIter );
                            break;
                        }
                    } while ( false );
                    break;
                }
            }
            while ( false );

            if ( scorer )
            {
                if ( ! detector )
                    detector = new G4MultiFunctionalDetector( detectorName );
                detector->RegisterPrimitive( scorer );
            }
        }

        if ( detector )
        {
            G4SDManager::GetSDMpointer()->AddNewDetector( detector );
            ( *lvIter )->SetSensitiveDetector( detector );
        }
    }

    if ( ! calorimeterRegionInitialized )
        throw CexmcException( CexmcCalorimeterRegionNotInitialized );
}

