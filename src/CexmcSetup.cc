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
#include "CexmcSetup.hh"
#include "CexmcSimpleEnergyDeposit.hh"
#include "CexmcEnergyDepositInLeftRightSet.hh"
#include "CexmcEnergyDepositInCalorimeter.hh"
#include "CexmcCommon.hh"


CexmcSetup::CexmcSetup( const G4String &  gdmlFile ) :
    world( 0 ), gdmlFile( gdmlFile )
{
}


G4VPhysicalVolume *  CexmcSetup::Construct( void )
{
    if ( world )
        return world;

    G4GDMLParser gdmlParser;

    gdmlParser.Read( gdmlFile );
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
        G4GDMLAuxListType
                auxInfo( gdmlParser.GetVolumeAuxiliaryInformation( *lvIter ) );
        std::vector< G4GDMLAuxPairType >::const_iterator
                pair( auxInfo.begin() );
        for( pair = auxInfo.begin(); pair != auxInfo.end(); ++pair )
        {
            if ( pair->type == "sensDet" )
            {
                G4String  detectorName( G4String( ( *lvIter )->GetName() ) +
                                        "_sensDet" );

                G4cout << CEXMC_LINE_START "Sensitive Detector '" <<
                                                  detectorName << "'" << G4endl;
                G4MultiFunctionalDetector *  detector(
                                new G4MultiFunctionalDetector( detectorName ) );

                G4VPrimitiveScorer *  scorer;
                do
                {
                    if ( pair->value < 0.5 )
                    {
                        scorer = new CexmcSimpleEnergyDeposit( "Monitor/ED/" );
                        break;
                    }
                    if ( pair->value < 1.5 )
                    {
                        scorer = new CexmcEnergyDepositInLeftRightSet(
                                                            "VetoCounter/ED/" );
                        break;
                    }
                    if ( pair->value < 2.5 )
                    {
                        scorer = new CexmcEnergyDepositInCalorimeter(
                                                            "Calorimeter/ED/" );
                        break;
                    }
                } while ( false );

                detector->RegisterPrimitive( scorer );

                G4SDManager::GetSDMpointer()->AddNewDetector( detector );
                ( *lvIter )->SetSensitiveDetector( detector );
            }
        }
    }
}

