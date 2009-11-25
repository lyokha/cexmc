/*
 * ============================================================================
 *
 *       Filename:  CexmcEventActionMessenger.cc
 *
 *    Description:  event action messenger (verbose level etc.)
 *
 *        Version:  1.0
 *        Created:  25.11.2009 14:45:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4UIcmdWithAnInteger.hh>
#include "CexmcEventActionMessenger.hh"
#include "CexmcEventAction.hh"
#include "CexmcMessenger.hh"


CexmcEventActionMessenger::CexmcEventActionMessenger(
                                    CexmcEventAction *  eventAction ) :
    eventAction( eventAction )
{
    setVerboseLevel = new G4UIcmdWithAnInteger(
               ( CexmcMessenger::eventDirName + "verbose" ).c_str() , this );
    setVerboseLevel->SetGuidance( "0 - do not print messages,\n"
                        "  1 - print messages if studied process triggered,\n"
                        "  2 - print messages on event trigger,\n"
                        "  3 - print messages on any triggered event,\n"
                        "  4 - print messages on every event" );
    setVerboseLevel->SetParameterName( "Verbose", true );
    setVerboseLevel->SetDefaultValue( 0 );
    setVerboseLevel->SetRange( "Verbose >= 0 && Verbose <= 4" );
    setVerboseLevel->AvailableForStates( G4State_PreInit, G4State_Idle );
}


CexmcEventActionMessenger::~CexmcEventActionMessenger()
{
    delete setVerboseLevel;
}


void  CexmcEventActionMessenger::SetNewValue( G4UIcommand *  cmd,
                                              G4String  value )
{
    do
    {
        if ( cmd == setVerboseLevel )
        {
            eventAction->SetVerboseOnCexmcLevel(
                                G4UIcmdWithAnInteger::GetNewIntValue( value ) );
            break;
        }
    } while ( false );
}

