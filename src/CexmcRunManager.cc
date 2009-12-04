/*
 * ============================================================================
 *
 *       Filename:  CexmcRunManager.cc
 *
 *    Description:  run manager
 *
 *        Version:  1.0
 *        Created:  03.11.2009 20:27:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4ScoringManager.hh>
#include <G4UImanager.hh>
#include <G4Timer.hh>
#include "CexmcRunManager.hh"
#include "CexmcRunManagerMessenger.hh"
#include "CexmcEventInfo.hh"


CexmcRunManager::CexmcRunManager() :
    productionModelType( CexmcUnknownProductionModel ),
    gdmlFileName( "default.gdml" ), saveResults( false ), resultsDir( "." ),
    runId( "trash" ), eventCountPolicy( CexmcCountAllEvents ), messenger( NULL )
{
    messenger = new CexmcRunManagerMessenger( this );
}


CexmcRunManager::~CexmcRunManager()
{
    delete messenger;
}


void  CexmcRunManager::UpdateScoringMy( void )
{
    G4ScoringManager * scManager(
                            G4ScoringManager::GetScoringManagerIfExist() );
    if ( ! scManager )
        return;

    G4int  nPar( scManager->GetNumberOfMesh() );
    if( nPar < 1 )
        return;

    G4HCofThisEvent *  hce( currentEvent->GetHCofThisEvent() );
    if ( ! hce )
        return;

    G4int  nColl( hce->GetCapacity() );
    for( G4int  i( 0 ); i < nColl; ++i )
    {
        G4VHitsCollection *  hc( hce->GetHC( i ) );
        if ( hc )
            scManager->Accumulate( hc );
    }
}


/* mostly adopted from G4RunManager::DoEventLoop() */
void  CexmcRunManager::DoEventLoop( G4int  nEvent, const char *  macroFile,
                                    G4int  nSelect )
{
    if ( verboseLevel > 0 ) 
        timer->Start();

    G4String  msg;
    if ( macroFile != 0 )
    {
        if ( nSelect < 0 )
            nSelect = nEvent;
        msg = "/control/execute ";
        msg += macroFile;
    }
    else
    {
        nSelect = -1;
    }

    G4int  iEvent( 0 );
    G4int  iEventEffective( 0 );

    for( iEvent = 0; iEventEffective < nEvent; ++iEvent )
    {
        currentEvent = GenerateEvent( iEvent );
        eventManager->ProcessOneEvent( currentEvent );
        CexmcEventInfo *  eventInfo( static_cast< CexmcEventInfo * >(
                                        currentEvent->GetUserInformation() ) );
        switch ( eventCountPolicy )
        {
        case CexmcCountAllEvents :
            ++iEventEffective;
            break;
        case CexmcCountEventsWithInteraction :
            if ( eventInfo->TpTriggerIsOk() )
                ++iEventEffective;
            break;
        case CexmcCountEventsWithTrigger :
            if ( eventInfo->EdTriggerIsOk() )
                ++iEventEffective;
            break;
        default :
            ++iEventEffective;
            break;
        }
        AnalyzeEvent( currentEvent );
        /* G4RunManager::UpdateScoring() is private, so we have to use
         * own copy */
        UpdateScoringMy();
        if( iEvent < nSelect )
            G4UImanager::GetUIpointer()->ApplyCommand( msg );
        StackPreviousEvent( currentEvent );
        currentEvent = 0;
        if( runAborted )
            break;
    }

    if( verboseLevel > 0 )
    {
        timer->Stop();
        G4cout << "Run terminated." << G4endl;
        G4cout << "Run Summary" << G4endl;
        if( runAborted )
        {
            G4cout << "  Run Aborted after " << iEvent <<
                      " events processed." << G4endl;
        }
        else
        {
            G4cout << "  Number of events processed : " << iEvent <<
                      ", effectively: " << iEventEffective << G4endl;
        }
        G4cout << "  "  << *timer << G4endl;
    }
}

