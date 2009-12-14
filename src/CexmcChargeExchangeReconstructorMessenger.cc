/*
 * ============================================================================
 *
 *       Filename:  CexmcChargeExchangeReconstructorMessenger.cc
 *
 *    Description:  charge exchange reconstructor messenger
 *
 *        Version:  1.0
 *        Created:  14.12.2009 17:53:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4UIcmdWithABool.hh>
#include "CexmcChargeExchangeReconstructorMessenger.hh"
#include "CexmcChargeExchangeReconstructor.hh"
#include "CexmcMessenger.hh"

CexmcChargeExchangeReconstructorMessenger::
        CexmcChargeExchangeReconstructorMessenger(
                        CexmcChargeExchangeReconstructor *  reconstructor ) :
            reconstructor( reconstructor ), useTableMass( NULL )
{
    useTableMass = new G4UIcmdWithABool(
        ( CexmcMessenger::reconstructorDirName + "useTableMass" ).c_str(),
        this );
    useTableMass->SetGuidance( "\n    If true then reconstructor will use "
        "table mass of output particle\n    when building output particle "
        "full energy,\n    otherwise reconstructed mass will be used" );
    useTableMass->SetParameterName( "UseTableMass", false );
    useTableMass->SetDefaultValue( false );
    useTableMass->AvailableForStates( G4State_PreInit, G4State_Idle );
}


CexmcChargeExchangeReconstructorMessenger::
                                    ~CexmcChargeExchangeReconstructorMessenger()
{
    delete useTableMass;
}


void  CexmcChargeExchangeReconstructorMessenger::SetNewValue(
                                        G4UIcommand *  cmd, G4String  value )
{
    do
    {
        if ( cmd == useTableMass )
        {
            reconstructor->UseTableMass(
                                G4UIcmdWithABool::GetNewBoolValue( value ) );
        }
    } while ( false );
}

