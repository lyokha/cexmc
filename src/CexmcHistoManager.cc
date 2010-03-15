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

#ifdef CEXMC_USE_ROOT

#include <TH1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TFile.h>
#include <TDirectory.h>
#ifdef CEXMC_USE_ROOTQT
#include <TCanvas.h>
#include <QApplication>
#include <QFont>
#include "CexmcHistoWidget.hh"
#endif
#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include "CexmcHistoManager.hh"
#include "CexmcHistoManagerMessenger.hh"
#include "CexmcBasicPhysicsSettings.hh"
#include "CexmcPhysicsManager.hh"
#include "CexmcRunManager.hh"
#include "CexmcException.hh"

#define EDT_TITLE  " --edt--"
#define TPT_TITLE  " --tpt--"
#define RT_TITLE   " --rt--"

extern TDirectory *  gDirectory;


namespace
{
    const G4double  CexmcHistoBeamMomentumMin( 0.0 * GeV );
    const G4double  CexmcHistoBeamMomentumMax( 1.0 * GeV );
    const G4double  CexmcHistoBeamMomentumResolution( 0.5 * MeV );
    const G4double  CexmcHistoTPResolution( 0.1 * cm );
    const G4double  CexmcHistoTPSafetyArea( 1.0 * cm );
    const G4double  CexmcHistoMassResolution( 1.0 * MeV );
    const G4double  CexmcHistoEnergyMax( 1.0 * GeV );
    const G4double  CexmcHistoEnergyResolution( 1.0 * MeV );
    const G4double  CexmcHistoMissEnergyMin( -0.1 * GeV );
    const G4double  CexmcHistoMissEnergyMax( 0.2 * GeV );
    const G4double  CexmcHistoMissEnergyResolution( 0.2 * MeV );
    const G4double  CexmcHistoAngularResolution( 0.5 );
    const G4double  CexmcHistoAngularCResolution( 0.001 );
    const G4int     CexmcHistoCanvasWidth( 800 );
    const G4int     CexmcHistoCanvasHeight( 600 );
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
    isInitialized( false ),
#ifdef CEXMC_USE_ROOTQT
    rootCanvas( NULL ),
#endif
    messenger( NULL )
{
    histos.insert( CexmcHistoPair( CexmcMomentumIP_TPT_Histo, &momip_tpt ) );
    histos.insert( CexmcHistoPair( CexmcMomentumIP_RT_Histo, &momip_rt ) );
    histos.insert( CexmcHistoPair( CexmcTPInMonitor_TPT_Histo, &tpmon_tpt ) );
    histos.insert( CexmcHistoPair( CexmcTPInTarget_TPT_Histo, &tptar_tpt ) );
    histos.insert( CexmcHistoPair( CexmcTPInTarget_RT_Histo, &tptar_rt ) );
    histos.insert( CexmcHistoPair( CexmcRecMasses_EDT_Histo, &recmasses_edt ) );
    histos.insert( CexmcHistoPair( CexmcRecMasses_RT_Histo, &recmasses_rt ) );
    histos.insert( CexmcHistoPair( CexmcAbsorbedEnergy_EDT_Histo, &ae_edt ) );
    histos.insert( CexmcHistoPair( CexmcAbsorbedEnergy_RT_Histo, &ae_rt ) );
    histos.insert( CexmcHistoPair( CexmcRecMassOP_ARReal_RT_Histo,
                                   &recmassop_arreal_rt ) );
    histos.insert( CexmcHistoPair( CexmcRecMassNOP_ARReal_RT_Histo,
                                   &recmassnop_arreal_rt ) );
    histos.insert( CexmcHistoPair( CexmcOPDPAtLeftCalorimeter_ARReal_EDT_Histo,
                                   &opdpcl_arreal_edt ) );
    histos.insert( CexmcHistoPair( CexmcOPDPAtRightCalorimeter_ARReal_EDT_Histo,
                                   &opdpcr_arreal_edt ) );
    histos.insert( CexmcHistoPair( CexmcOPDPAtLeftCalorimeter_ARReal_RT_Histo,
                                   &opdpcl_arreal_rt ) );
    histos.insert( CexmcHistoPair( CexmcOPDPAtRightCalorimeter_ARReal_RT_Histo,
                                   &opdpcr_arreal_rt ) );
    histos.insert( CexmcHistoPair(
                                CexmcRecOPDPAtLeftCalorimeter_ARReal_EDT_Histo,
                                &recopdpcl_arreal_edt ) );
    histos.insert( CexmcHistoPair(
                                CexmcRecOPDPAtRightCalorimeter_ARReal_EDT_Histo,
                                &recopdpcr_arreal_edt ) );
    histos.insert( CexmcHistoPair(
                                CexmcRecOPDPAtLeftCalorimeter_ARReal_RT_Histo,
                                &recopdpcl_arreal_rt ) );
    histos.insert( CexmcHistoPair(
                                CexmcRecOPDPAtRightCalorimeter_ARReal_RT_Histo,
                                &recopdpcr_arreal_rt ) );
    histos.insert( CexmcHistoPair( CexmcKinEnAtLeftCalorimeter_ARReal_TPT_Histo,
                                   &kecl_arreal_tpt ) );
    histos.insert( CexmcHistoPair(
                                CexmcKinEnAtRightCalorimeter_ARReal_TPT_Histo,
                                &kecr_arreal_tpt ) );
    histos.insert( CexmcHistoPair( CexmcKinEnAtLeftCalorimeter_ARReal_RT_Histo,
                                   &kecl_arreal_rt ) );
    histos.insert( CexmcHistoPair( CexmcKinEnAtRightCalorimeter_ARReal_RT_Histo,
                                   &kecr_arreal_rt ) );
    histos.insert( CexmcHistoPair( CexmcAbsEnInLeftCalorimeter_ARReal_EDT_Histo,
                                   &aecl_arreal_edt ) );
    histos.insert( CexmcHistoPair(
                                CexmcAbsEnInRightCalorimeter_ARReal_EDT_Histo,
                                &aecr_arreal_edt ) );
    histos.insert( CexmcHistoPair( CexmcAbsEnInLeftCalorimeter_ARReal_RT_Histo,
                                   &aecl_arreal_rt ) );
    histos.insert( CexmcHistoPair( CexmcAbsEnInRightCalorimeter_ARReal_RT_Histo,
                                   &aecr_arreal_rt ) );
    histos.insert( CexmcHistoPair(
                                CexmcMissEnFromLeftCalorimeter_ARReal_RT_Histo,
                                &mecl_arreal_rt ) );
    histos.insert( CexmcHistoPair(
                                CexmcMissEnFromRightCalorimeter_ARReal_RT_Histo,
                                &mecr_arreal_rt ) );
    histos.insert( CexmcHistoPair( CexmcKinEnOP_LAB_ARReal_TPT_Histo,
                                   &keop_lab_arreal_tpt ) );
    histos.insert( CexmcHistoPair( CexmcKinEnOP_LAB_ARReal_RT_Histo,
                                   &keop_lab_arreal_rt ) );
    histos.insert( CexmcHistoPair( CexmcAngleOP_SCM_ARReal_TPT_Histo,
                                   &aop_scm_arreal_tpt ) );
    histos.insert( CexmcHistoPair( CexmcAngleOP_SCM_ARReal_RT_Histo,
                                   &aop_scm_arreal_rt ) );
    histos.insert( CexmcHistoPair( CexmcRecAngleOP_SCM_ARReal_RT_Histo,
                                   &recaop_scm_arreal_rt ) );
    histos.insert( CexmcHistoPair( CexmcDiffAngleOP_SCM_ARReal_RT_Histo,
                                   &diffaop_scm_arreal_rt ) );
    histos.insert( CexmcHistoPair( CexmcOpenAngle_ARReal_TPT_Histo,
                                   &oa_arreal_tpt ) );
    histos.insert( CexmcHistoPair( CexmcOpenAngle_ARReal_RT_Histo,
                                   &oa_arreal_rt ) );
    histos.insert( CexmcHistoPair( CexmcRecOpenAngle_ARReal_RT_Histo,
                                   &recoa_arreal_rt ) );
    histos.insert( CexmcHistoPair( CexmcDiffOpenAngle_ARReal_RT_Histo,
                                   &diffoa_arreal_rt ) );
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
#ifdef CEXMC_USE_ROOTQT
    delete rootCanvas;
#endif
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
    const G4LogicalVolume *       lVolume( lvs->GetVolume( "vMonitor" ) );

    nBinsMinX = CexmcHistoBeamMomentumMin;
    nBinsMaxX = CexmcHistoBeamMomentumMax;
    nBinsX = Int_t( ( nBinsMaxX - nBinsMinX ) /
                    CexmcHistoBeamMomentumResolution );
    histo = new TH1F( "momip_tpt", "Beam momentum at monitor" TPT_TITLE,
                      nBinsX, nBinsMinX, nBinsMaxX );
    momip_tpt.push_back( histo );

    histo = new TH1F( "momip_rt", "Beam momentum at monitor" RT_TITLE,
                      nBinsX, nBinsMinX, nBinsMaxX );
    momip_rt.push_back( histo );

    G4Box *   box( static_cast< G4Box * >( lVolume->GetSolid() ) );
    G4double  width( box->GetXHalfLength() * 2 );
    G4double  height( box->GetYHalfLength() * 2 );
    G4double  halfWidth( width / 2 + CexmcHistoTPSafetyArea );
    G4double  halfHeight( height / 2 + CexmcHistoTPSafetyArea );

    nBinsX = Int_t( halfWidth * 2 / CexmcHistoTPResolution );
    nBinsY = Int_t( halfHeight * 2 / CexmcHistoTPResolution );
    histo = new TH2F( "tpmon_tpt", "Track Points (mon)" TPT_TITLE,
                      nBinsX, -halfWidth, halfWidth, nBinsY, -halfHeight,
                      halfHeight );
    tpmon_tpt.push_back( histo );

    lVolume = lvs->GetVolume( "vTarget" );
    G4Tubs *  tube( static_cast< G4Tubs * >( lVolume->GetSolid() ) );
    G4double  radius( tube->GetOuterRadius() );
    height = tube->GetZHalfLength() * 2;
    halfWidth = radius + CexmcHistoTPSafetyArea;
    halfHeight = height / 2 + CexmcHistoTPSafetyArea;

    nBinsX = Int_t( halfWidth * 2 / CexmcHistoTPResolution );
    nBinsY = Int_t( halfWidth * 2 / CexmcHistoTPResolution );
    Int_t  nBinsZ( Int_t( halfHeight * 2 / CexmcHistoTPResolution ) );
    histo = new TH3F( "tptar_tpt", "Track Points (tar)" TPT_TITLE,
                      nBinsX, -halfWidth, halfWidth, nBinsY, -halfWidth,
                      halfWidth, nBinsZ, -halfHeight, halfHeight );
    tptar_tpt.push_back( histo );

    histo = new TH3F( "tptar_rt", "Track Points (tar)" RT_TITLE,
                      nBinsX, -halfWidth, halfWidth, nBinsY, -halfWidth,
                      halfWidth, nBinsZ, -halfHeight, halfHeight );
    tptar_rt.push_back( histo );

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

    title = "Reconstructed masses (" + nopName + " vs. " + opName + ")" +
            RT_TITLE;
    histo = new TH2F( "recmasses_rt", title.c_str(), nBinsX, nBinsMinX,
                      nBinsMaxX, nBinsY, nBinsMinY, nBinsMaxY );
    recmasses_rt.push_back( histo );

    nBinsMinX = 0;
    nBinsMaxX = CexmcHistoEnergyMax;
    nBinsMinY = 0;
    nBinsMaxY = CexmcHistoEnergyMax;
    nBinsX = Int_t( ( nBinsMaxX - nBinsMinX ) / CexmcHistoEnergyResolution );
    nBinsY = Int_t( ( nBinsMaxY - nBinsMinY ) / CexmcHistoEnergyResolution );
    title = G4String( "Absorbed energy (rc vs. lc)" ) + EDT_TITLE;
    histo = new TH2F( "ae_edt", title.c_str(), nBinsX, nBinsMinX,
                      nBinsMaxX, nBinsY, nBinsMinY, nBinsMaxY );
    ae_edt.push_back( histo );

    title = G4String( "Absorbed energy (rc vs. lc)" ) + RT_TITLE;
    histo = new TH2F( "ae_rt", title.c_str(), nBinsX, nBinsMinX,
                      nBinsMaxX, nBinsY, nBinsMinY, nBinsMaxY );
    ae_rt.push_back( histo );

    SetupARHistos( runManager->GetPhysicsManager()->GetProductionModel()->
                   GetAngularRanges() );

    isInitialized = true;
}


void  CexmcHistoManager::SetupARHistos( const CexmcAngularRangeList &  aRanges )
{
    gDirectory->Delete( "*_r[1-9]*" );
    recmassop_arreal_rt.clear();
    recmassnop_arreal_rt.clear();
    opdpcl_arreal_edt.clear();
    opdpcr_arreal_edt.clear();
    opdpcl_arreal_rt.clear();
    opdpcr_arreal_rt.clear();
    recopdpcl_arreal_edt.clear();
    recopdpcr_arreal_edt.clear();
    recopdpcl_arreal_rt.clear();
    recopdpcr_arreal_rt.clear();
    kecl_arreal_tpt.clear();
    kecr_arreal_tpt.clear();
    kecl_arreal_rt.clear();
    kecr_arreal_rt.clear();
    aecl_arreal_edt.clear();
    aecr_arreal_edt.clear();
    aecl_arreal_rt.clear();
    aecr_arreal_rt.clear();
    mecl_arreal_rt.clear();
    mecr_arreal_rt.clear();
    keop_lab_arreal_tpt.clear();
    keop_lab_arreal_rt.clear();
    aop_scm_arreal_tpt.clear();
    aop_scm_arreal_rt.clear();
    recaop_scm_arreal_rt.clear();
    diffaop_scm_arreal_rt.clear();
    oa_arreal_tpt.clear();
    oa_arreal_rt.clear();
    recoa_arreal_rt.clear();
    diffoa_arreal_rt.clear();

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

    const G4LogicalVolumeStore *  lvs( G4LogicalVolumeStore::GetInstance() );
    const G4LogicalVolume *       lVolume( lvs->GetVolume( "vCalorimeter" ) );

    G4Box *   box( static_cast< G4Box * >( lVolume->GetSolid() ) );
    G4double  width( box->GetXHalfLength() * 2 );
    G4double  height( box->GetYHalfLength() * 2 );
    G4double  halfWidth( width / 2 + CexmcHistoTPSafetyArea );
    G4double  halfHeight( height / 2 + CexmcHistoTPSafetyArea );

    out.str( "" );
    out << "opdpcl_r" << aRange.index + 1 << "real_edt";
    name = out.str();
    out.str( "" );
    out << "Gamma position on the surface (lc) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" EDT_TITLE;
    title = out.str();
    nBinsX = Int_t( halfWidth * 2 / CexmcHistoTPResolution );
    Int_t  nBinsY( Int_t( halfHeight * 2 / CexmcHistoTPResolution ) );
    /* looks like there is no possibility to draw descending xaxis in root,
     * so imagine that you look at calorimeters from behind, i.e. your face to
     * the beam */
    opdpcl_arreal_edt.push_back( new TH2F( name.c_str(), title.c_str(),
                                           nBinsX, -halfWidth, halfWidth,
                                           nBinsY, -halfHeight, halfHeight ) );

    out.str( "" );
    out << "opdpcr_r" << aRange.index + 1 << "real_edt";
    name = out.str();
    out.str( "" );
    out << "Gamma position on the surface (rc) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" EDT_TITLE;
    title = out.str();
    opdpcr_arreal_edt.push_back( new TH2F( name.c_str(), title.c_str(),
                                           nBinsX, -halfWidth, halfWidth,
                                           nBinsY, -halfHeight, halfHeight ) );

    out.str( "" );
    out << "opdpcl_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Gamma position on the surface (lc) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    opdpcl_arreal_rt.push_back( new TH2F( name.c_str(), title.c_str(),
                                          nBinsX, -halfWidth, halfWidth,
                                          nBinsY, -halfHeight, halfHeight ) );

    out.str( "" );
    out << "opdpcr_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Gamma position on the surface (rc) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    opdpcr_arreal_rt.push_back( new TH2F( name.c_str(), title.c_str(),
                                          nBinsX, -halfWidth, halfWidth,
                                          nBinsY, -halfHeight, halfHeight ) );

    out.str( "" );
    out << "recopdpcl_r" << aRange.index + 1 << "real_edt";
    name = out.str();
    out.str( "" );
    out << "Reconstructed gamma position on the surface (lc) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" EDT_TITLE;
    title = out.str();
    recopdpcl_arreal_edt.push_back( new TH2F( name.c_str(), title.c_str(),
                                           nBinsX, -halfWidth, halfWidth,
                                           nBinsY, -halfHeight, halfHeight ) );

    out.str( "" );
    out << "recopdpcr_r" << aRange.index + 1 << "real_edt";
    name = out.str();
    out.str( "" );
    out << "Reconstructed gamma position on the surface (rc) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" EDT_TITLE;
    title = out.str();
    recopdpcr_arreal_edt.push_back( new TH2F( name.c_str(), title.c_str(),
                                           nBinsX, -halfWidth, halfWidth,
                                           nBinsY, -halfHeight, halfHeight ) );

    out.str( "" );
    out << "recopdpcl_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Reconstructed gamma position on the surface (lc) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    recopdpcl_arreal_rt.push_back( new TH2F( name.c_str(), title.c_str(),
                                          nBinsX, -halfWidth, halfWidth,
                                          nBinsY, -halfHeight, halfHeight ) );

    out.str( "" );
    out << "recopdpcr_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Reconstructed gamma position on the surface (rc) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    recopdpcr_arreal_rt.push_back( new TH2F( name.c_str(), title.c_str(),
                                          nBinsX, -halfWidth, halfWidth,
                                          nBinsY, -halfHeight, halfHeight ) );

    out.str( "" );
    out << "kecl_r" << aRange.index + 1 << "real_tpt";
    name = out.str();
    out.str( "" );
    out << "Kinetic energy of gamma (lc) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" TPT_TITLE;
    title = out.str();
    nBinsMinX = 0;
    nBinsMaxX = Int_t( CexmcHistoEnergyMax );
    nBinsX = Int_t( ( nBinsMaxX - nBinsMinX ) / CexmcHistoEnergyResolution );
    kecl_arreal_tpt.push_back( new TH1F( name.c_str(), title.c_str(),
                                         nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "kecr_r" << aRange.index + 1 << "real_tpt";
    name = out.str();
    out.str( "" );
    out << "Kinetic energy of gamma (rc) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" TPT_TITLE;
    title = out.str();
    kecr_arreal_tpt.push_back( new TH1F( name.c_str(), title.c_str(),
                                         nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "kecl_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Kinetic energy of gamma (lc) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    kecl_arreal_rt.push_back( new TH1F( name.c_str(), title.c_str(),
                                        nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "kecr_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Kinetic energy of gamma (rc) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    kecr_arreal_rt.push_back( new TH1F( name.c_str(), title.c_str(),
                                        nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "aecl_r" << aRange.index + 1 << "real_edt";
    name = out.str();
    out.str( "" );
    out << "Absorbed energy (lc) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" EDT_TITLE;
    title = out.str();
    aecl_arreal_edt.push_back( new TH1F( name.c_str(), title.c_str(),
                                         nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "aecr_r" << aRange.index + 1 << "real_edt";
    name = out.str();
    out.str( "" );
    out << "Absorbed energy (rc) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" EDT_TITLE;
    title = out.str();
    aecr_arreal_edt.push_back( new TH1F( name.c_str(), title.c_str(),
                                         nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "aecl_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Absorbed energy (lc) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    aecl_arreal_rt.push_back( new TH1F( name.c_str(), title.c_str(),
                                        nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "aecr_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Absorbed energy (rc) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    aecr_arreal_rt.push_back( new TH1F( name.c_str(), title.c_str(),
                                        nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "mecl_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Missing energy (lc) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    nBinsMinX = Int_t( CexmcHistoMissEnergyMin );
    nBinsMaxX = Int_t( CexmcHistoMissEnergyMax );
    nBinsX = Int_t( ( nBinsMaxX - nBinsMinX ) /
                    CexmcHistoMissEnergyResolution );
    mecl_arreal_rt.push_back( new TH1F( name.c_str(), title.c_str(),
                                        nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "mecr_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Missing energy (rc) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    mecr_arreal_rt.push_back( new TH1F( name.c_str(), title.c_str(),
                                        nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "keop_lab_r" << aRange.index + 1 << "real_tpt";
    name = out.str();
    out.str( "" );
    out << "Kinetic energy of newborn " << opName << " (lab) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" TPT_TITLE;
    title = out.str();
    nBinsMinX = Int_t( 0 );
    nBinsMaxX = Int_t( CexmcHistoEnergyMax );
    nBinsX = Int_t( ( nBinsMaxX - nBinsMinX ) / CexmcHistoEnergyResolution );
    keop_lab_arreal_tpt.push_back( new TH1F( name.c_str(), title.c_str(),
                                             nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "keop_lab_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Kinetic energy of newborn " << opName << " (lab) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    nBinsMinX = Int_t( 0 );
    nBinsMaxX = Int_t( CexmcHistoEnergyMax );
    nBinsX = Int_t( ( nBinsMaxX - nBinsMinX ) / CexmcHistoEnergyResolution );
    keop_lab_arreal_rt.push_back( new TH1F( name.c_str(), title.c_str(),
                                            nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "aop_scm_r" << aRange.index + 1 << "real_tpt";
    name = out.str();
    out.str( "" );
    out << "Angle of newborn " << opName << " (scm) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" TPT_TITLE;
    title = out.str();
    nBinsMinX = Int_t( -1.0 );
    nBinsMaxX = Int_t( 1.0 );
    nBinsX = Int_t( ( nBinsMaxX - nBinsMinX ) / CexmcHistoAngularCResolution );
    aop_scm_arreal_tpt.push_back( new TH1F( name.c_str(), title.c_str(),
                                            nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "aop_scm_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Angle of newborn " << opName << " (scm) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    aop_scm_arreal_rt.push_back( new TH1F( name.c_str(), title.c_str(),
                                           nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "recaop_scm_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Reconstructed angle of newborn " << opName << " (scm) {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    recaop_scm_arreal_rt.push_back( new TH1F( name.c_str(), title.c_str(),
                                              nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "diffaop_scm_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Real - reconstructed angle of newborn " << opName <<
            " (scm) {range " << aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    diffaop_scm_arreal_rt.push_back( new TH1F( name.c_str(), title.c_str(),
                                               nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "oa_r" << aRange.index + 1 << "real_tpt";
    name = out.str();
    out.str( "" );
    out << "Open angle between the gammas {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" TPT_TITLE;
    title = out.str();
    nBinsMinX = Int_t( 0. );
    nBinsMaxX = Int_t( 360. );
    nBinsX = Int_t( ( nBinsMaxX - nBinsMinX ) / CexmcHistoAngularResolution );
    oa_arreal_tpt.push_back( new TH1F( name.c_str(), title.c_str(),
                                       nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "oa_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Open angle between the gammas {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    oa_arreal_rt.push_back( new TH1F( name.c_str(), title.c_str(),
                                      nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "recoa_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Reconstructed open angle between the gammas {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    recoa_arreal_rt.push_back( new TH1F( name.c_str(), title.c_str(),
                                         nBinsX, nBinsMinX, nBinsMaxX ) );

    out.str( "" );
    out << "diffoa_r" << aRange.index + 1 << "real_rt";
    name = out.str();
    out.str( "" );
    out << "Real - reconstructed open angle between the gammas {range " <<
            aRange.index + 1 << "real [" << std::fixed <<
            std::setprecision( 4 ) << aRange.top << ", " << aRange.bottom <<
            ")}" RT_TITLE;
    title = out.str();
    nBinsMinX = Int_t( -180. );
    nBinsMaxX = Int_t( 180. );
    nBinsX = Int_t( ( nBinsMaxX - nBinsMinX ) / CexmcHistoAngularResolution );
    diffoa_arreal_rt.push_back( new TH1F( name.c_str(), title.c_str(),
                                          nBinsX, nBinsMinX, nBinsMaxX ) );
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

    /* no cast needed because TH1 has virtual method
     * Fill( Double_t, Double_t ) */
    histos[ histoType ]->operator[]( index )->Fill( x, y );
}


void  CexmcHistoManager::Add( CexmcHistoType  histoType, unsigned int  index,
                              G4double  x, G4double  y, G4double  z )
{
    CexmcHistosMap::iterator  found( histos.find( histoType ) );
    if ( found == histos.end() || histos[ histoType ]->size() <= index )
        throw CexmcException ( CexmcWeirdException );

    /* cast needed because TH1 does not have virtual method
     * Fill( Double_t, Double_t, Double_t ) */
    TH3 *  histo( static_cast< TH3 * >(
                                histos[ histoType ]->operator[]( index ) ) );

    histo->Fill( x, y, z );
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


#ifdef CEXMC_USE_ROOTQT

void  CexmcHistoManager::Draw( const G4String &  histoName,
                               const G4String &  histoDrawOptions )
{
    CexmcRunManager *  runManager( static_cast< CexmcRunManager * >(
                                            G4RunManager::GetRunManager() ) );
    if ( ! runManager->IsLiveHistogramsEnabled() )
    {
        G4cout << "Live histograms option is disabled" << G4endl;
        return;
    }

    TObject *  histo( gDirectory->FindObject( histoName ) );

    if ( ! histo )
    {
        G4cout << "Histogram '" << histoName << "' was not found" << G4endl;
        return;
    }

    if ( ! rootCanvas )
    {
        /* save default application font because rootCanvas will break it */
        QFont  defaultAppFont( QApplication::font() );
        rootCanvas = new CexmcHistoWidget;
        QApplication::setFont( defaultAppFont );
        rootCanvas->resize( CexmcHistoCanvasWidth, CexmcHistoCanvasHeight );
        rootCanvas->GetCanvas()->cd();
    }

    histo->Draw( histoDrawOptions );
    rootCanvas->show();
    rootCanvas->GetCanvas()->Update();
}

#endif

#endif

