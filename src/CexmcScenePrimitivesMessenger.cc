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

#include <G4UIcmdWith3Vector.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithoutParameter.hh>
#include "CexmcScenePrimitives.hh"
#include "CexmcScenePrimitivesMessenger.hh"
#include "CexmcMessenger.hh"


CexmcScenePrimitivesMessenger::CexmcScenePrimitivesMessenger(
                                    CexmcScenePrimitives *  scenePrimitives ) :
    scenePrimitives( scenePrimitives ), drawRadialLine( NULL ),
    clearRadialLines( NULL ), markTargetCenter( NULL ),
    highlightInnerCrystals( NULL )
{
    drawRadialLine = new G4UIcmdWith3Vector(
        ( CexmcMessenger::visDirName + "drawRadialLine" ).c_str(), this );
    drawRadialLine->SetGuidance( "Draw radial line with specified theta, phi "
                                 "(both in deg!)\n    and length (in cm!)" );
    drawRadialLine->SetParameterName( "RadialLineTheta", "RadialLinePhi",
                                      "RadialLineLength", true );
    drawRadialLine->SetRange( "RadialLineLength >= 0." );
    drawRadialLine->SetDefaultValue( G4ThreeVector( 0., 0., 100. ) );
    drawRadialLine->AvailableForStates( G4State_PreInit, G4State_Idle );

    clearRadialLines = new G4UIcmdWithoutParameter(
        ( CexmcMessenger::visDirName + "clearRadialLines" ).c_str(), this );
    clearRadialLines->SetGuidance( "Clear all existing radial lines" );
    clearRadialLines->AvailableForStates( G4State_PreInit, G4State_Idle );

    markTargetCenter = new G4UIcmdWithABool(
        ( CexmcMessenger::visDirName + "markTargetCenter" ).c_str(), this );
    markTargetCenter->SetGuidance( "Mark/unmark target center" );
    markTargetCenter->SetParameterName( "MarkTargetCenter", true );
    markTargetCenter->SetDefaultValue( true );
    markTargetCenter->AvailableForStates( G4State_PreInit, G4State_Idle );

    highlightInnerCrystals = new G4UIcmdWithABool(
        ( CexmcMessenger::visDirName + "hlIC" ).c_str(), this );
    highlightInnerCrystals->SetGuidance( "Highlight inner crystals in "
                                         "calorimeters on/off" );
    highlightInnerCrystals->SetParameterName( "HighlightInnerCrystals", true );
    highlightInnerCrystals->SetDefaultValue( true );
    highlightInnerCrystals->AvailableForStates( G4State_PreInit, G4State_Idle );
}


CexmcScenePrimitivesMessenger::~CexmcScenePrimitivesMessenger()
{
    delete drawRadialLine;
    delete clearRadialLines;
    delete markTargetCenter;
    delete highlightInnerCrystals;
}


void  CexmcScenePrimitivesMessenger::SetNewValue( G4UIcommand *  cmd,
                                                  G4String  value )
{
    do
    {
        if ( cmd == drawRadialLine )
        {
            G4ThreeVector  line( G4UIcmdWith3Vector::GetNew3VectorValue(
                                                                    value ) );
            scenePrimitives->DrawRadialLine( line );
            break;
        }
        if ( cmd == clearRadialLines )
        {
            scenePrimitives->ClearRadialLines();
            break;
        }
        if ( cmd == markTargetCenter )
        {
            scenePrimitives->MarkTargetCenter(
                                G4UIcmdWithABool::GetNewBoolValue( value ) );
            break;
        }
        if ( cmd == highlightInnerCrystals )
        {
            scenePrimitives->HighlightInnerCrystals(
                                G4UIcmdWithABool::GetNewBoolValue( value ) );
            break;
        }
    } while ( false );
}

