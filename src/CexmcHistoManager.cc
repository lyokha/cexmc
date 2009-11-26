/*
 * ============================================================================
 *
 *       Filename:  CexmcHistoManager.cc
 *
 *    Description:  histograming manager (singleton)
 *
 *        Version:  1.0
 *        Created:  26.11.2009 21:00:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <TH1.h>
#include <TH2F.h>
#include <TFile.h>
#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>
#include "CexmcHistoManager.hh"


CexmcHistoManager *  CexmcHistoManager::instance( NULL );
bool                 CexmcHistoManager::isInitialized( false );


CexmcHistoManager *  CexmcHistoManager::Instance( void )
{
    if ( instance == NULL )
        instance = new CexmcHistoManager;

    return instance;
}


void  CexmcHistoManager::Destroy( void )
{
    delete instance;
    instance = NULL;
    isInitialized = false;
}


void  CexmcHistoManager::Initialize( void )
{
    if ( isInitialized )
        return;

    Instance();

    const G4LogicalVolumeStore *  lvs( G4LogicalVolumeStore::GetInstance() );
    EAxis                         axis;
    G4int                         nCrystalsInColumn( 1 );
    G4int                         nCrystalsInRow( 1 );
    G4double                      width;
    G4double                      offset;
    G4bool                        consuming;

    G4LogicalVolume *             lVolume( lvs->GetVolume( "vCalorimeter" ) );
    if ( lVolume )
    {
        G4VPhysicalVolume *  pVolume( lVolume->GetDaughter( 0 ) );
        if ( pVolume && pVolume->IsReplicated() )
        {
            pVolume->GetReplicationData( axis, nCrystalsInColumn, width, offset,
                                         consuming );
        }
    }

    lVolume = lvs->GetVolume( "vCrystalRow" );
    if ( lVolume )
    {
        G4VPhysicalVolume *  pVolume( lVolume->GetDaughter( 0 ) );
        if ( pVolume && pVolume->IsReplicated() )
        {
            pVolume->GetReplicationData( axis, nCrystalsInRow, width, offset,
                                         consuming );
        }
    }

    instance->outFile = new TFile( "hists.root", "recreate" );
    instance->edInLeftCalorimeter = new TH2F( "edlc",
            "Energy Deposit (lc)", nCrystalsInRow, 0, nCrystalsInRow,
                                   nCrystalsInColumn, 0, nCrystalsInColumn );
    instance->histos->insert( std::pair< CexmcHistoType, TH1 * >(
                                        CexmcEnergyDepositInLeftCalorimeter,
                                        instance->edInLeftCalorimeter ) );
    instance->edInRightCalorimeter = new TH2F( "edrc",
            "Energy Deposit (rc)", nCrystalsInRow, 0, nCrystalsInRow,
                                   nCrystalsInColumn, 0, nCrystalsInColumn );
    instance->histos->insert( std::pair< CexmcHistoType, TH1 * >(
                                         CexmcEnergyDepositInRightCalorimeter,
                                         instance->edInRightCalorimeter ) );

    isInitialized = true;
}


CexmcHistoManager::CexmcHistoManager() : outFile( NULL ),
    edInLeftCalorimeter( 0 ), edInRightCalorimeter( 0 ), histos( NULL )
{
    histos = new std::map< CexmcHistoType, TH1 * >;
}


CexmcHistoManager::~CexmcHistoManager()
{
    outFile->Write();
    outFile->Close();

    /* all histograms will be deleted by outFile destructor! */
    delete outFile;
    delete histos;
}


void  CexmcHistoManager::Add( CexmcHistoType  histoType,
                              G4int  binX, G4int  binY, G4double  value )
{
    ++binX;
    ++binY;
    Double_t  curValue( ( *histos )[ histoType ]->GetBinContent( binX, binY ) );
    ( *histos )[ histoType ]->SetBinContent( binX, binY, curValue + value );
}

