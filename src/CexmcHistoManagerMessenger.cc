/*
 * ============================================================================
 *
 *       Filename:  CexmcHistoManagerMessenger.cc
 *
 *    Description:  commands to list and show histograms
 *
 *        Version:  1.0
 *        Created:  17.12.2009 21:39:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithAString.hh>
#include "CexmcHistoManagerMessenger.hh"
#include "CexmcHistoManager.hh"
#include "CexmcMessenger.hh"


CexmcHistoManagerMessenger::CexmcHistoManagerMessenger() :
    listHistos( NULL ), printHisto( NULL ), drawHisto( NULL )
{
    listHistos = new G4UIcmdWithoutParameter(
        ( CexmcMessenger::histoDirName + "list" ).c_str(), this );
    listHistos->SetGuidance( "List available histograms" );
    listHistos->AvailableForStates( G4State_PreInit, G4State_Idle );

    printHisto = new G4UIcmdWithAString(
        ( CexmcMessenger::histoDirName + "print" ).c_str(), this );
    printHisto->SetGuidance( "Print specified histogram" );
    printHisto->SetParameterName( "PrintHisto", false );
    printHisto->AvailableForStates( G4State_Idle );

    drawHisto = new G4UIcmdWithAString(
        ( CexmcMessenger::histoDirName + "draw" ).c_str(), this );
    drawHisto->SetGuidance( "Draw specified histogram. Available only if the "
                            "program\n    was launched in graphical mode" );
    drawHisto->SetParameterName( "DrawHisto", false );
    drawHisto->AvailableForStates( G4State_Idle );
}


CexmcHistoManagerMessenger::~CexmcHistoManagerMessenger()
{
    delete listHistos;
    delete printHisto;
    delete drawHisto;
}


void CexmcHistoManagerMessenger::SetNewValue(
                                        G4UIcommand *  cmd, G4String  value )
{
    CexmcHistoManager *  histoManager( CexmcHistoManager::Instance() );

    do
    {
        if ( cmd == listHistos )
        {
           histoManager->List();
        }
        if ( cmd == printHisto )
        {
           histoManager->Print( value );
        }
        if ( cmd == drawHisto )
        {
           histoManager->Draw( value );
        }
    } while ( false );
}

