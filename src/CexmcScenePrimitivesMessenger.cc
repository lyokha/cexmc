/*
 * =============================================================================
 *
 *       Filename:  CexmcScenePrimitivesMessenger.cc
 *
 *    Description:  draw auxiliary scene primitives
 *
 *        Version:  1.0
 *        Created:  03.01.2011 12:42:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithoutParameter.hh>
#include "CexmcScenePrimitives.hh"
#include "CexmcScenePrimitivesMessenger.hh"
#include "CexmcMessenger.hh"


CexmcScenePrimitivesMessenger::CexmcScenePrimitivesMessenger(
                                    CexmcScenePrimitives *  scenePrimitives ) :
    scenePrimitives( scenePrimitives ), setRadialLineLength( NULL ),
    drawRadialLine( NULL )
{
    setRadialLineLength = new G4UIcmdWithADoubleAndUnit(
        ( CexmcMessenger::visDirName + "radialLineLength" ).c_str(), this );
    setRadialLineLength->SetGuidance( "Radial line length" );
    setRadialLineLength->SetParameterName( "RadialLineLength", false );
    setRadialLineLength->SetRange( "RadialLineLength > 0" );
    setRadialLineLength->SetDefaultUnit( "m" );
    setRadialLineLength->SetUnitCandidates( "cm m" );
    setRadialLineLength->AvailableForStates( G4State_PreInit, G4State_Idle );

    drawRadialLine = new G4UIcmdWithADoubleAndUnit(
        ( CexmcMessenger::visDirName + "drawRadialLine" ).c_str(), this );
    drawRadialLine->SetGuidance( "Draw radial line at specified direction" );
    drawRadialLine->SetParameterName( "DrawRadialLine", false );
    drawRadialLine->SetDefaultUnit( "deg" );
    drawRadialLine->SetUnitCandidates( "deg rad" );
    drawRadialLine->AvailableForStates( G4State_PreInit, G4State_Idle );

    markTargetCenter = new G4UIcmdWithoutParameter(
        ( CexmcMessenger::visDirName + "markTargetCenter" ).c_str(), this );
    markTargetCenter->SetGuidance( "Mark target center with a dot" );
    markTargetCenter->AvailableForStates( G4State_PreInit, G4State_Idle );
}


CexmcScenePrimitivesMessenger::~CexmcScenePrimitivesMessenger()
{
    delete setRadialLineLength;
    delete drawRadialLine;
    delete markTargetCenter;
}


void  CexmcScenePrimitivesMessenger::SetNewValue( G4UIcommand *  cmd,
                                                  G4String  value )
{
    do
    {
        if ( cmd == setRadialLineLength )
        {
            scenePrimitives->SetRadialLineLength(
                    G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
            break;
        }
        if ( cmd == drawRadialLine )
        {
            scenePrimitives->DrawRadialLine(
                    G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( value ) );
            break;
        }
        if ( cmd == markTargetCenter )
        {
            scenePrimitives->MarkTargetCenter();
            break;
        }
    } while ( false );
}

