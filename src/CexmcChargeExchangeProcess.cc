/*
 * ============================================================================
 *
 *       Filename:  CexmcChargeExchangeProcess.cc
 *
 *    Description:  charge exchange process
 *
 *        Version:  1.0
 *        Created:  31.10.2009 23:54:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4HadronElasticDataSet.hh>
#include <G4ParticleChange.hh>
#include <G4Track.hh>
#include <G4Step.hh>
#include <G4HadronicWhiteBoard.hh>
#include <G4HadReentrentException.hh>
#include "CexmcChargeExchangeProcess.hh"
#include "CexmcProductionModel.hh"
#include "CexmcException.hh"


CexmcChargeExchangeProcess::CexmcChargeExchangeProcess(
                                                    const G4String &  name ) :
    G4HadronicProcess( name ), productionModel( NULL )
{
    AddDataSet( new G4HadronElasticDataSet );
    theTotalResult = new G4ParticleChange();
}


CexmcChargeExchangeProcess::~CexmcChargeExchangeProcess()
{
    delete theTotalResult;
}


void  CexmcChargeExchangeProcess::RegisterProductionModel(
                                    G4HadronicInteraction *  productionModel_ )
{
    productionModel = dynamic_cast< CexmcProductionModel *>( productionModel_ );
    G4HadronicProcess::RegisterMe( productionModel_ );
}


void  CexmcChargeExchangeProcess::FillTotalResult(
                    G4HadFinalState *  hadFinalState, const G4Track &  track )
{
    theTotalResult->Clear();
    theTotalResult->ProposeLocalEnergyDeposit( 0. );
    theTotalResult->Initialize( track );
    theTotalResult->SetSecondaryWeightByProcess( true );
    theTotalResult->ProposeTrackStatus( fAlive );

    theTotalResult->ProposeTrackStatus( fStopAndKill );
    theTotalResult->ProposeEnergy( 0.0 );
    theTotalResult->ProposeLocalEnergyDeposit(
                                    hadFinalState->GetLocalEnergyDeposit() );
    theTotalResult->SetNumberOfSecondaries(
                                    hadFinalState->GetNumberOfSecondaries() );

    for( G4int i( 0 ); i < hadFinalState->GetNumberOfSecondaries(); ++i )
    {
        G4double   time( hadFinalState->GetSecondary( i )->GetTime() );
        if ( time < 0 )
            time = track.GetGlobalTime();

        G4Track *  newTrack( new G4Track(
                             hadFinalState->GetSecondary( i )->GetParticle(),
                             time, track.GetPosition() ) );

        G4double   newWeight( track.GetWeight() *
                              hadFinalState->GetSecondary( i )->GetWeight() );
        newTrack->SetWeight( newWeight );
        newTrack->SetTouchableHandle( track.GetTouchableHandle() );
        theTotalResult->AddSecondary( newTrack );
    }

    hadFinalState->Clear();
}


/* mostly adopted from G4HadronicProcess::PostStepDoIt() */
G4ParticleChange *  CexmcChargeExchangeProcess::PostStepDoIt(
                                        const G4Track &  track, const G4Step & )
{
    if ( track.GetTrackStatus() != fAlive &&
         track.GetTrackStatus() != fSuspend )
    {
        if ( track.GetTrackStatus() == fStopAndKill ||
             track.GetTrackStatus() == fKillTrackAndSecondaries ||
             track.GetTrackStatus() == fPostponeToNextEvent)
        {
            G4cerr << "G4HadronicProcess: track in unusable state - " <<
                      track.GetTrackStatus() << G4endl;
            G4cerr << "G4HadronicProcess: returning unchanged track " << G4endl;
            G4Exception( "G4HadronicProcess", "001", JustWarning,
                         "bailing out" );
        }
        // No warning for fStopButAlive which is a legal status here
        theTotalResult->Clear();
        theTotalResult->Initialize( track );
        return theTotalResult;
    }

    const G4DynamicParticle *  particle( track.GetDynamicParticle() );
    G4Material *               material( track.GetMaterial() );
    G4double                   originalEnergy( particle->GetKineticEnergy() );
    G4Nucleus                  targetNucleus;

    G4HadProjectile            projectile( track );
    G4HadFinalState *          result( 0 );
    G4int                      reentryCount( 0 );

    G4HadronicInteraction *  theInteraction(
                dynamic_cast< G4HadronicInteraction * >( productionModel ) );
    if ( ! theInteraction )
        throw CexmcException( CexmcWeirdException );

    do
    {
        try
        {
            result = theInteraction->ApplyYourself( projectile, targetNucleus );
        }
        catch ( G4HadReentrentException &  e )
        {
            G4HadronicWhiteBoard &  theBoard(
                                            G4HadronicWhiteBoard::Instance() );
            theBoard.SetProjectile( projectile );
            theBoard.SetTargetNucleus( targetNucleus );
            theBoard.SetProcessName( GetProcessName() );
            theBoard.SetModelName( theInteraction->GetModelName() );

            e.Report( G4cout );
            G4cout << " G4HadronicProcess re-entering the ApplyYourself call "
                      "for " << G4endl;
            G4cout << " - Particle energy[GeV] = " << originalEnergy / GeV <<
                      G4endl;
            G4cout << " - Material = " << material->GetName() << G4endl;
            G4cout << " - Particle type = " <<
                      particle->GetDefinition()->GetParticleName() << G4endl;
            result = 0; // here would still be leaking...

            if ( reentryCount > 100 )
            {
                G4Exception( "G4HadronicProcess", "007", FatalException,
                             "GetHadronicProcess: Reentering ApplyYourself too "
                             "often - PostStepDoIt failed." );  
            }
            G4Exception( "G4HadronicProcess", "007", FatalException,
                         "GetHadronicProcess: PostStepDoIt failed "
                         "(Reentering ApplyYourself not yet supported.)" );  
        }
        catch ( G4HadronicException &  e )
        {
            G4HadronicWhiteBoard &  theBoard(
                                            G4HadronicWhiteBoard::Instance() );
            theBoard.SetProjectile( projectile );
            theBoard.SetTargetNucleus( targetNucleus );
            theBoard.SetProcessName( GetProcessName() );
            theBoard.SetModelName( theInteraction->GetModelName() );

            e.Report( G4cout );
            G4cout << " G4HadronicProcess failed in ApplyYourself call for" <<
                      G4endl;
            G4cout << " - Particle energy[GeV] = "<< originalEnergy / GeV <<
                      G4endl;
            G4cout << " - Material = " << material->GetName() << G4endl;
            G4cout << " - Particle type = " <<
                      particle->GetDefinition()->GetParticleName() << G4endl;
            G4Exception( "G4HadronicProcess", "007", FatalException,
                         "PostStepDoIt failed." );
        }
    } while( ! result );

    ClearNumberOfInteractionLengthLeft();

    FillTotalResult( result, track );

    return theTotalResult;
}

