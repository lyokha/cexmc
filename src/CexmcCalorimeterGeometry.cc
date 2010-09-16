/*
 * ============================================================================
 *
 *       Filename:  CexmcCalorimeterGeometry.cc
 *
 *    Description:  calorimeter geometry (static methods)
 *
 *        Version:  1.0
 *        Created:  02.12.2009 22:44:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4Box.hh>
#include <G4AffineTransform.hh>
#include "CexmcCalorimeterGeometry.hh"
#include "CexmcException.hh"


void  CexmcCalorimeterGeometry::GetGeometryData( G4int &  nCrystalsInColumn,
                                                 G4int &  nCrystalsInRow,
                                                 G4double &  crystalWidth,
                                                 G4double &  crystalHeight,
                                                 G4double &  crystalLength )
{
    const G4LogicalVolumeStore *  lvs( G4LogicalVolumeStore::GetInstance() );
    EAxis                         axis;
    G4double                      width;
    G4double                      offset;
    G4bool                        consuming;

    G4LogicalVolume *             lVolume( lvs->GetVolume( "vCalorimeter" ) );
    if ( ! lVolume )
        throw CexmcException( CexmcIncompatibleGeometry );

    G4VPhysicalVolume *  pVolume( lVolume->GetDaughter( 0 ) );
    if ( pVolume && pVolume->IsReplicated() )
    {
        pVolume->GetReplicationData( axis, nCrystalsInColumn, width, offset,
                                     consuming );
    }

    lVolume = lvs->GetVolume( "vCrystalRow" );
    if ( ! lVolume )
        throw CexmcException( CexmcIncompatibleGeometry );

    pVolume = lVolume->GetDaughter( 0 );
    if ( pVolume && pVolume->IsReplicated() )
    {
        pVolume->GetReplicationData( axis, nCrystalsInRow, width, offset,
                                     consuming );
    }

    lVolume = lvs->GetVolume( "vCrystal" );
    if ( ! lVolume )
        throw CexmcException( CexmcIncompatibleGeometry );

    G4Box *  crystalBox( static_cast< G4Box * >( lVolume->GetSolid() ) );
    crystalWidth = crystalBox->GetXHalfLength() * 2;
    crystalHeight = crystalBox->GetYHalfLength() * 2;
    crystalLength = crystalBox->GetZHalfLength() * 2;
}


void  CexmcCalorimeterGeometry::ConvertToCrystalGeometry(
                const G4ThreeVector &  src, G4int &  row, G4int &  column,
                G4ThreeVector &  dst )
{
    G4int     nCrystalsInColumn;
    G4int     nCrystalsInRow;
    G4double  crystalWidth;
    G4double  crystalHeight;
    G4double  crystalLength;

    GetGeometryData( nCrystalsInColumn, nCrystalsInRow, crystalWidth,
                     crystalHeight, crystalLength );

    row = G4int( ( src.y() + crystalHeight * nCrystalsInColumn / 2 ) /
                 crystalHeight );
    column = G4int( ( src.x() + crystalWidth * nCrystalsInRow / 2 ) /
                    crystalWidth );
    G4double   xInCalorimeterOffset(
                    ( G4double( column ) - G4double( nCrystalsInRow ) / 2 ) *
                                            crystalWidth + crystalWidth / 2 );
    G4double   yInCalorimeterOffset(
                    ( G4double( row ) - G4double( nCrystalsInColumn ) / 2 ) *
                                            crystalHeight + crystalHeight / 2 );
    dst.setX( src.x() - xInCalorimeterOffset );
    dst.setY( src.y() - yInCalorimeterOffset );
}
                 

void  CexmcCalorimeterGeometry::GetCalorimeterLeftTransform(
                                                G4AffineTransform &  transform )
{
    /* we suppose that transformation only occurs in 'CalorimeterLeft'
     * physical volume! */
    const G4PhysicalVolumeStore *  pvs( G4PhysicalVolumeStore::GetInstance() );
    G4VPhysicalVolume *            pVolume( pvs->GetVolume(
                                                        "CalorimeterLeft" ) );
    if ( ! pVolume )
        throw CexmcException( CexmcIncompatibleGeometry );

    transform.SetNetTranslation( pVolume->GetTranslation() );
    G4RotationMatrix *  rm( pVolume->GetRotation() );
    if ( rm )
        transform.SetNetRotation( *rm );
}


void  CexmcCalorimeterGeometry::GetCalorimeterRightTransform(
                                                G4AffineTransform &  transform )
{
    /* we suppose that transformation only occurs in 'CalorimeterRight'
     * physical volume! */
    const G4PhysicalVolumeStore *  pvs( G4PhysicalVolumeStore::GetInstance() );
    G4VPhysicalVolume *            pVolume( pvs->GetVolume(
                                                        "CalorimeterRight" ) );
    if ( ! pVolume )
        throw CexmcException( CexmcIncompatibleGeometry );

    transform.SetNetTranslation( pVolume->GetTranslation() );
    G4RotationMatrix *  rm( pVolume->GetRotation() );
    if ( rm )
        transform.SetNetRotation( *rm );
}

