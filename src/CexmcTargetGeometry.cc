/*
 * ============================================================================
 *
 *       Filename:  CexmcTargetGeometry.cc
 *
 *    Description:  target geometry (static methods)
 *
 *        Version:  1.0
 *        Created:  03.12.2009 17:28:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4VPhysicalVolume.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4AffineTransform.hh>
#include "CexmcTargetGeometry.hh"
#include "CexmcException.hh"


void  CexmcTargetGeometry::GetTargetTransform( G4AffineTransform &  transform )
{
    /* we suppose that transformation only occurs in 'TargetInnerCover'
     * physical volume! I.e. the target locates in the center of
     * 'TargetInnerCover' */
    const G4PhysicalVolumeStore *  pvs( G4PhysicalVolumeStore::GetInstance() );
    G4VPhysicalVolume *            pVolume( pvs->GetVolume(
                                                        "TargetInnerCover" ) );
    if ( ! pVolume )
        throw CexmcException( CexmcIncompatibleGeometry );

    transform.SetNetTranslation( pVolume->GetTranslation() );
    G4RotationMatrix *  rm( pVolume->GetRotation() );
    if ( rm )
        transform.SetNetRotation( *rm );
}

