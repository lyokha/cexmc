/*
 * ============================================================================
 *
 *       Filename:  CexmcProductionModelMessenger.cc
 *
 *    Description:  switch charge exchange production model aspects
 *
 *        Version:  1.0
 *        Created:  03.11.2009 16:01:24
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4UIcmdWithABool.hh>
#include "CexmcProductionModel.hh"
#include "CexmcProductionModelMessenger.hh"
#include "CexmcMessenger.hh"


CexmcProductionModelMessenger::CexmcProductionModelMessenger(
                                    CexmcProductionModel *  productionModel ) :
    productionModel( productionModel )
{
    applyFermiMotion = new G4UIcmdWithABool(
        ( CexmcMessenger::physicsDirName + "applyFermiMotionInTarget" ).c_str(),
        this );
    applyFermiMotion->SetGuidance( "Switch on/off fermi motion in target "
                                   "nuclei" );
    applyFermiMotion->SetParameterName( "ApplyFermiMotionInTarget", false );
    applyFermiMotion->AvailableForStates( G4State_PreInit, G4State_Idle );
}


CexmcProductionModelMessenger::~CexmcProductionModelMessenger()
{
    delete applyFermiMotion;
}


void CexmcProductionModelMessenger::SetNewValue( G4UIcommand *  cmd,
                                                 G4String  value )
{
    do
    {
        if ( cmd == applyFermiMotion )
        {
            productionModel->ApplyFermiMotion(
                                G4UIcmdWithABool::GetNewBoolValue( value ) );
            break;
        }
    } while ( false );
}

