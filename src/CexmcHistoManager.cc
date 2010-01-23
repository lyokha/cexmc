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
#include "CexmcBasicPhysicsSettings.hh"
#include "CexmcRunManager.hh"
#include "CexmcException.hh"

#define EDT_TITLE  " --edt--"
#define TPT_TITLE  " --tpt--"
#define RT_TITLE   " --rt--"

extern TDirectory *  gDirectory;


namespace
{
    const G4double  CexmcHistoTPResolution( 0.1 * cm );
    const G4double  CexmcHistoTPSafetyArea( 1.0 * cm );
    const G4double  CexmcHistoMassResolution( 1.0 * MeV );
}


CexmcHistoManager *  CexmcHistoManager::instance( NULL );


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
}


CexmcHistoManager::CexmcHistoManager() : outFile( NULL ),
    isInitialized( false ), messenger( NULL )
{
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
    delete messenger;
}


void  CexmcHistoManager::Initialize( void )
{
    if ( isInitialized )
        return;

    CexmcRunManager *         runManager( static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );
    CexmcProductionModelType  productionModelType(
                                        runManager->GetProductionModelType() );
    G4ParticleDefinition *    outputParticle(
                                CexmcChargeExchangePMFactory::
                                    GetOutputParticle( productionModelType ) );
    G4ParticleDefinition *    nucleusOutputParticle(
                                CexmcChargeExchangePMFactory::
                                    GetNucleusOutputParticle(
                                                        productionModelType ) );
    if ( ! outputParticle || ! nucleusOutputParticle )
        throw CexmcException ( CexmcWeirdException );
    G4String                  opName( outputParticle->GetParticleName() );
    G4String                  nopName(
                                    nucleusOutputParticle->GetParticleName() );
    G4String                  title;
    Int_t                     nBinsX;
    Int_t                     nBinsY;
    Double_t                  nBinsMinX;
    Double_t                  nBinsMaxX;
    Double_t                  nBinsMinY;
    Double_t                  nBinsMaxY;
    TH1 *                     histo( NULL );

    if ( runManager->ProjectIsSaved() )
    {
        G4String  projectsDir( runManager->GetProjectsDir() );
        G4String  resultsFile( projectsDir + "/" + runManager->GetProjectId() +
                               ".root" );
        outFile = new TFile( resultsFile, "recreate" );
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

    histo = new TH2F( "edcl_edt", "Energy Deposit (lc)" EDT_TITLE,
                      nCrystalsInRow, 0, nCrystalsInRow, nCrystalsInColumn, 0,
                      nCrystalsInColumn );
    edcl_edt.push_back( histo );
    histos.insert( CexmcHistoPair( CexmcEDInLeftCalorimeter_EDT_Histo,
                                   &edcl_edt ) );
    histo = new TH2F( "edcr_edt", "Energy Deposit (rc)" EDT_TITLE,
                      nCrystalsInRow, 0, nCrystalsInRow, nCrystalsInColumn, 0,
                      nCrystalsInColumn );
    edcr_edt.push_back( histo );
    histos.insert( CexmcHistoPair( CexmcEDInRightCalorimeter_EDT_Histo,
                                   &edcr_edt ) );

    lVolume = lvs->GetVolume( "vMonitor" );
    G4Box *   box( static_cast< G4Box * >( lVolume->GetSolid() ) );
    width = box->GetXHalfLength() * 2;
    G4double  height( box->GetYHalfLength() * 2 );
    G4double  halfWidth( width / 2 + CexmcHistoTPSafetyArea );
    G4double  halfHeight( height / 2 + CexmcHistoTPSafetyArea );
    nBinsX = Int_t( halfWidth * 2 / CexmcHistoTPResolution );
    nBinsY = Int_t( halfHeight * 2 / CexmcHistoTPResolution );
    histo = new TH2F( "tpmon_tpt", "Track Points (mon)" TPT_TITLE,
                      nBinsX, -halfWidth, halfWidth, nBinsY, -halfHeight,
                      halfHeight );
    tpmon_tpt.push_back( histo );
    histos.insert( CexmcHistoPair( CexmcTPInMonitor_TPT_Histo, &tpmon_tpt ) );

    title = "Reconstructed masses (" + nopName + " vs. " + opName + ")" +
            EDT_TITLE;
    G4double  opMass( outputParticle->GetPDGMass() );
    G4double  nopMass( nucleusOutputParticle->GetPDGMass() );
    nBinsMinX = Int_t( opMass / 2 );
    nBinsMaxX = Int_t( opMass + opMass / 2 );
    nBinsMinY = Int_t( nopMass / 2 );
    nBinsMaxY = Int_t( nopMass + nopMass / 2 );
    nBinsX = Int_t( ( nBinsMaxX - nBinsMinX ) / CexmcHistoMassResolution );
    nBinsY = Int_t( ( nBinsMaxY - nBinsMinY ) / CexmcHistoMassResolution );
    histo = new TH2F( "recmasses_edt", title.c_str(), nBinsX, nBinsMinX,
                      nBinsMaxX, nBinsY, nBinsMinY, nBinsMaxY );
    recmasses_edt.push_back( histo );
    histos.insert( CexmcHistoPair( CexmcRecMasses_EDT_Histo, &recmasses_edt ) );

    title = "Reconstructed masses (" + nopName + " vs. " + opName + ")" +
            RT_TITLE;
    histo = new TH2F( "recmasses_rt", title.c_str(), nBinsX, nBinsMinX,
                      nBinsMaxX, nBinsY, nBinsMinY, nBinsMaxY );
    recmasses_rt.push_back( histo );
    histos.insert( CexmcHistoPair( CexmcRecMasses_RT_Histo, &recmasses_rt ) );

    isInitialized = true;
}


void  CexmcHistoManager::SetupARHistos( const CexmcAngularRangeList &  aRanges )
{
    gDirectory->Delete( "*_r[1-9]*" );
    recmassop_arreal_rt.clear();
    recmassnop_arreal_rt.clear();
    recmassop_arrec_rt.clear();
    recmassnop_arrec_rt.clear();

    for ( CexmcAngularRangeList::const_iterator  k( aRanges.begin() );
                                                    k != aRanges.end(); ++k )
    {
        AddARHistos( *k );
    }
}


void  CexmcHistoManager::AddARHistos( const CexmcAngularRange &  aRange )
{
    CexmcRunManager *         runManager( static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );
    CexmcProductionModelType  productionModelType(
                                        runManager->GetProductionModelType() );
    G4ParticleDefinition *    outputParticle(
                                CexmcChargeExchangePMFactory::
                                    GetOutputParticle( productionModelType ) );
    G4ParticleDefinition *    nucleusOutputParticle(
                                CexmcChargeExchangePMFactory::
                                    GetNucleusOutputParticle(
                                                        productionModelType ) );
    if ( ! outputParticle || ! nucleusOutputParticle )
        throw CexmcException ( CexmcWeirdException );
    G4String                  opName( outputParticle->GetParticleName() );
    G4String                  nopName(
                                    nucleusOutputParticle->GetParticleName() );
    G4String                  name;
    G4String                  title;
    std::ostringstream        out;
    Int_t                     nBinsX;
    Double_t                  nBinsMinX;
    Double_t                  nBinsMaxX;

    out << "recmassop_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Reconstructed mass of " << opName << " {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    G4double  opMass( outputParticle->GetPDGMass() );
    nBinsMinX = Int_t( opMass / 2 );
    nBinsMaxX = Int_t( opMass + opMass / 2 );
    nBinsX = Int_t( ( nBinsMaxX - nBinsMinX ) / CexmcHistoMassResolution );
    recmassop_arreal_rt.push_back( new TH1F( name.c_str(), title.c_str(),
                                             nBinsX, nBinsMinX, nBinsMaxX ) );
    histos.insert( CexmcHistoPair( CexmcRecMassOP_ARReal_RT_Histo,
                                   &recmassop_arreal_rt ) );
    out.str( "" );
    out << "recmassop_r" << aRange.index + 1 << "rec_rt";
    name = out.str();
    out.str( "" );
    out << "Reconstructed mass of " << opName << " {range " <<
            aRange.index + 1 << "rec [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    recmassop_arrec_rt.push_back( new TH1F( name.c_str(), title.c_str(),
                                            nBinsX, nBinsMinX, nBinsMaxX ) );
    histos.insert( CexmcHistoPair( CexmcRecMassOP_ARRec_RT_Histo,
                                   &recmassop_arrec_rt ) );

    out.str( "" );
    out << "recmassnop_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Reconstructed mass of " << nopName << " {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    G4double  nopMass( nucleusOutputParticle->GetPDGMass() );
    nBinsMinX = Int_t( nopMass / 2 );
    nBinsMaxX = Int_t( nopMass + nopMass / 2 );
    nBinsX = Int_t( ( nBinsMaxX - nBinsMinX ) / CexmcHistoMassResolution );
    recmassnop_arreal_rt.push_back( new TH1F( name.c_str(), title.c_str(),
                                              nBinsX, nBinsMinX, nBinsMaxX ) );
    histos.insert( CexmcHistoPair( CexmcRecMassNOP_ARReal_RT_Histo,
                                   &recmassnop_arreal_rt ) );
    out.str( "" );
    out << "recmassnop_r" << aRange.index + 1 << "rec_rt";
    name = out.str();
    out.str( "" );
    out << "Reconstructed mass of " << nopName << " {range " <<
            aRange.index + 1 << "rec [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    recmassnop_arrec_rt.push_back( new TH1F( name.c_str(), title.c_str(),
                                             nBinsX, nBinsMinX, nBinsMaxX ) );
    histos.insert( CexmcHistoPair( CexmcRecMassNOP_ARRec_RT_Histo,
                                   &recmassnop_arrec_rt ) );
}


void  CexmcHistoManager::Add( CexmcHistoType  histoType, unsigned int  index,
                              G4int  binX, G4int  binY, G4double  value )
{
    CexmcHistosMap::iterator  found( histos.find( histoType ) );
    if ( found == histos.end() || histos[ histoType ]->size() <= index )
        throw CexmcException ( CexmcWeirdException );

    ++binX;
    ++binY;
    Double_t  curValue( histos[ histoType ]->operator[]( index )->GetBinContent(
                                                                binX, binY ) );
    histos[ histoType ]->operator[]( index )->SetBinContent( binX, binY,
                                                     curValue + value / GeV );
}


void  CexmcHistoManager::Add( CexmcHistoType  histoType, unsigned int  index,
                              G4double  x )
{
    CexmcHistosMap::iterator  found( histos.find( histoType ) );
    if ( found == histos.end() || histos[ histoType ]->size() <= index )
        throw CexmcException ( CexmcWeirdException );

    histos[ histoType ]->operator[]( index )->Fill( x );
}


void  CexmcHistoManager::Add( CexmcHistoType  histoType, unsigned int  index,
                              G4double  x, G4double  y )
{
    CexmcHistosMap::iterator  found( histos.find( histoType ) );
    if ( found == histos.end() || histos[ histoType ]->size() <= index )
        throw CexmcException ( CexmcWeirdException );

    histos[ histoType ]->operator[]( index )->Fill( x, y );
}


void  CexmcHistoManager::List( void ) const
{
    gDirectory->ls();
}


void  CexmcHistoManager::Print( const G4String &  value )
{
    TObject *  histo( gDirectory->FindObject( value.c_str() ) );

    if ( ! histo )
    {
        G4cout << "Histogram '" << value << "' was not found" << G4endl;
        return;
    }

    histo->Print( "range" );
}

