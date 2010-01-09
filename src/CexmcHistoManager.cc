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
#include <TDirectory.h>
#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4Box.hh>
#include "CexmcHistoManager.hh"
#include "CexmcHistoManagerMessenger.hh"
#include "CexmcRunManager.hh"

extern TDirectory *  gDirectory;


namespace
{
    const G4double  CexmcHistoTPResolution( 0.1 * cm );
    const G4double  CexmcHistoTPSafetyArea( 1.0 * cm );
}


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

    CexmcRunManager *  runManager( static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );
    if ( runManager->ProjectIsSaved() )
    {
        G4String  projectsDir( runManager->GetProjectsDir() );
        G4String  resultsFile( projectsDir + "/" + runManager->GetProjectId() +
                               ".root" );
        instance->outFile = new TFile( resultsFile, "recreate" );
    }

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

    instance->edcl_edt = new TH2F( "edcl_edt",
            "Energy Deposit (lc) --edt--", nCrystalsInRow, 0, nCrystalsInRow,
                                   nCrystalsInColumn, 0, nCrystalsInColumn );
    instance->histos->insert( std::pair< CexmcHistoType, TH1 * >(
                                        CexmcEDInLeftCalorimeter_EDT_Histo,
                                        instance->edcl_edt ) );
    instance->edcr_edt = new TH2F( "edcr_edt",
            "Energy Deposit (rc) --edt--", nCrystalsInRow, 0, nCrystalsInRow,
                                   nCrystalsInColumn, 0, nCrystalsInColumn );
    instance->histos->insert( std::pair< CexmcHistoType, TH1 * >(
                                         CexmcEDInRightCalorimeter_EDT_Histo,
                                         instance->edcr_edt ) );

    lVolume = lvs->GetVolume( "vMonitor" );
    G4Box *   box( static_cast< G4Box * >( lVolume->GetSolid() ) );
    width = box->GetXHalfLength() * 2;
    G4double  height( box->GetYHalfLength() * 2 );
    G4double  halfWidth( width / 2 + CexmcHistoTPSafetyArea );
    G4double  halfHeight( height / 2 + CexmcHistoTPSafetyArea );
    G4int     nBinsX( halfWidth * 2 / CexmcHistoTPResolution );
    G4int     nBinsY( halfHeight * 2 / CexmcHistoTPResolution );

    instance->tpmon_tpt = new TH2F( "tpmon_tpt",
            "Track Points (mon) --tpt--", nBinsX, -halfWidth, halfWidth,
                                          nBinsY, -halfHeight, halfHeight );
    instance->histos->insert( std::pair< CexmcHistoType, TH1 * >(
                                         CexmcTPInMonitor_TPT_Histo,
                                         instance->tpmon_tpt ) );

    isInitialized = true;
}


CexmcHistoManager::CexmcHistoManager() : outFile( NULL ),
    edcl_edt( NULL ), edcr_edt( NULL ), tpmon_tpt( NULL ), histos( NULL ),
    messenger( NULL )
{
    histos = new std::map< CexmcHistoType, TH1 * >;

    messenger = new CexmcHistoManagerMessenger;
}


CexmcHistoManager::~CexmcHistoManager()
{
    if ( outFile )
    {
        outFile->Write();
        outFile->Close();
    }

    /* all histograms will be deleted by outFile destructor! */
    delete outFile;
    delete histos;
    delete messenger;
}


void  CexmcHistoManager::Add( CexmcHistoType  histoType,
                              G4int  binX, G4int  binY, G4double  value )
{
    ++binX;
    ++binY;
    Double_t  curValue( ( *histos )[ histoType ]->GetBinContent( binX, binY ) );
    ( *histos )[ histoType ]->SetBinContent( binX, binY,
                                             curValue + value / GeV );
}


void  CexmcHistoManager::Add( CexmcHistoType  histoType, G4double  x,
                              G4double  y )
{
    ( *histos )[ histoType ]->Fill( x, y );
}


void  CexmcHistoManager::List( void ) const
{
    gDirectory->ls();
}


void  CexmcHistoManager::Print( const G4String &  value )
{
    TH1 *  histo( static_cast< TH1 * >(
                                    gDirectory->FindObject( value.c_str() ) ) );
    if ( ! histo )
    {
        G4cout << "Histogram '" << value << "' was not found" << G4endl;
        return;
    }

    histo->Print( "range" );
}

