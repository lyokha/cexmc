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

#include <string.h>
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
#include "CexmcSensitiveDetectorsAttributes.hh"


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

    SetupSpecialVolumes( gdmlParser );

    return world;
}


void CexmcSetup::SetupSpecialVolumes( G4GDMLParser &  gdmlParser )
{
    const G4LogicalVolumeStore *  lvs( G4LogicalVolumeStore::GetInstance() );
    G4MultiFunctionalDetector *   detector[ CexmcNumberOfDetectorRoles ];
    memset( detector, 0, sizeof( detector ) );

    for( std::vector< G4LogicalVolume * >::const_iterator
                        lvIter( lvs->begin() ); lvIter != lvs->end(); ++lvIter )
    {
        G4GDMLAuxListType  auxInfo( gdmlParser.GetVolumeAuxiliaryInformation(
                                                                    *lvIter ) );
        std::vector< G4GDMLAuxPairType >::const_iterator  pair(
                                                            auxInfo.begin() );

        for( pair = auxInfo.begin(); pair != auxInfo.end(); ++pair )
        {
            G4VPrimitiveScorer *  scorer( NULL );
            G4String              detectorName( "uninitialized" );
            CexmcDetectorRole     curDetectorRole( CexmcMonitorDetectorRole );
            do
            {
                if ( pair->type == "EnergyDepositDetector" )
                {
                    do
                    {
                        if ( pair->value < 0.5 )
                        {
                            curDetectorRole = CexmcMonitorDetectorRole;
                            scorer = new CexmcSimpleEnergyDeposit(
                                    CexmcDetectorTypeName[ CexmcEDDetector ] );
                            break;
                        }
                        if ( pair->value < 1.5 )
                        {
                            curDetectorRole = CexmcVetoCounterDetectorRole;
                            scorer = new CexmcEnergyDepositInLeftRightSet(
                                    CexmcDetectorTypeName[ CexmcEDDetector ] );
                            break;
                        }
                        if ( pair->value < 2.5 )
                        {
                            curDetectorRole = CexmcCalorimeterDetectorRole;
                            scorer = new CexmcEnergyDepositInCalorimeter(
                                    CexmcDetectorTypeName[ CexmcEDDetector ] );
                            break;
                        }
                    } while ( false );
                    detectorName = CexmcDetectorRoleName[ curDetectorRole ];
                    G4cout << CEXMC_LINE_START "Energy Deposit Scorer for "
                               "detector '" << detectorName << "'" << G4endl;
                    break;
                }
                if ( pair->type == "TrackPointsDetector" )
                {
                    do
                    {
                        if ( pair->value < 0.5 )
                        {
                            curDetectorRole = CexmcMonitorDetectorRole;
                            scorer = new CexmcTrackPoints(
                                    CexmcDetectorTypeName[ CexmcTPDetector ] );
                            break;
                        }
                        if ( pair->value < 1.5 )
                        {
                            curDetectorRole = CexmcVetoCounterDetectorRole;
                            scorer = new CexmcTrackPointsInLeftRightSet(
                                    CexmcDetectorTypeName[ CexmcTPDetector ] );
                            break;
                        }
                        if ( pair->value < 2.5 )
                        {
                            curDetectorRole = CexmcCalorimeterDetectorRole;
                            scorer = new CexmcTrackPointsInCalorimeter(
                                    CexmcDetectorTypeName[ CexmcTPDetector ] );
                            break;
                        }
                        if ( pair->value < 3.5 )
                        {
                            curDetectorRole = CexmcTargetDetectorRole;
                            scorer = new CexmcTrackPoints(
                                    CexmcDetectorTypeName[ CexmcTPDetector ] );
                            break;
                        }
                    } while ( false );
                    detectorName = CexmcDetectorRoleName[ curDetectorRole ];
                    G4cout << CEXMC_LINE_START "Track Points Scorer for "
                               "detector '" << detectorName << "'" << G4endl;
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
                    G4String  volumeName( G4String( ( *lvIter )->GetName() ) );
                    G4cout << CEXMC_LINE_START "Sensitive Region for logical "
                               "volume '" << volumeName << "'" << G4endl;
                    break;
                }
            }
            while ( false );

            if ( scorer )
            {
                /* curDetectorRole must be intact when scorer is not NULL */
                if ( ! detector[ curDetectorRole ] )
                {
                    detector[ curDetectorRole ] =
                                new G4MultiFunctionalDetector( detectorName );
                }
                detector[ curDetectorRole ]->RegisterPrimitive( scorer );
                /* NB: logical volumes in GDML file may not have multiple
                 * detector roles: for example vMonitor may have only one role
                 * MonitorRole. This restriction arises from that fact that a
                 * logical volume may contain only one sensitive detector. */
                ( *lvIter )->SetSensitiveDetector(
                                                detector[ curDetectorRole ] );
            }
        }
    }

    if ( ! calorimeterRegionInitialized )
        throw CexmcException( CexmcCalorimeterRegionNotInitialized );

    for ( G4int  i( 0 ); i < CexmcNumberOfDetectorRoles; ++i )
    {
        if ( detector[ i ] )
            G4SDManager::GetSDMpointer()->AddNewDetector( detector[ i ] );
    }
}

