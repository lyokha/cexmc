/*
 * =============================================================================
 *
 *       Filename:  CexmcScenePrimitives.cc
 *
 *    Description:  auxiliary scene primitives (radial lines etc.)
 *
 *        Version:  1.0
 *        Created:  03.01.2011 11:45:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#include <cmath>
#include <G4Polyline.hh>
#include <G4Circle.hh>
#include <G4Polyhedron.hh>
#include <G4ThreeVector.hh>
#include <G4VisAttributes.hh>
#include <G4VGraphicsScene.hh>
#include <G4AffineTransform.hh>
#include <G4Transform3D.hh>
#include <G4Point3D.hh>
#include "CexmcScenePrimitives.hh"
#include "CexmcScenePrimitivesMessenger.hh"
#include "CexmcSetup.hh"
#include "CexmcCommon.hh"


namespace
{
    G4double  CexmcRadialLineWidth( 2.0 );
    G4double  CexmcRadialLineCapScreenSize( 4.0 );
    G4double  CexmcMarkerScreenSize( 2.0 );
    G4double  CexmcICHlLineLineWidth( 1.0 );
    G4Colour  CexmcDefaultSPColour( 1.0, 1.0, 1.0 );
}


CexmcScenePrimitives::CexmcScenePrimitives( CexmcSetup *  setup ) :
    setup( setup ), markTargetCenter( false ), highlightInnerCrystals( false ),
    messenger( NULL )
{
    messenger = new CexmcScenePrimitivesMessenger( this );
    SetGlobalDescription( CexmcScenePrimitivesDescription );
    spColours[ CexmcTargetCenterMark_SP ] = CexmcDefaultSPColour;
    spColours[ CexmcRadialLine_SP ] = CexmcDefaultSPColour;
    spColours[ CexmcInnerCrystalsHl_SP ] = CexmcDefaultSPColour;
}


CexmcScenePrimitives::~CexmcScenePrimitives()
{
    delete messenger;
}


void  CexmcScenePrimitives::DescribeYourselfTo( G4VGraphicsScene &  scene )
{
    if ( markTargetCenter )
        MarkTargetCenter( scene );
    if ( highlightInnerCrystals )
        HighlightInnerCrystals( scene );
    for ( CexmcRadialLines::const_iterator  k( radialLines.begin() );
                                                k != radialLines.end(); ++k )
    {
        DrawRadialLine( scene, &*k );
    }
}


void  CexmcScenePrimitives::MarkTargetCenter( G4VGraphicsScene &  scene )
{
    G4Circle  circle;
    circle.SetScreenSize( CexmcMarkerScreenSize );
    circle.SetFillStyle( G4Circle::filled );
    circle.SetVisAttributes( spColours[ CexmcTargetCenterMark_SP ] );

    const G4AffineTransform &  transform( setup->GetTargetTransform() );
    G4Transform3D              transform3D( G4RotationMatrix(),
                                            transform.NetTranslation() );

    scene.BeginPrimitives( transform3D );
    scene.AddPrimitive( circle );
    scene.EndPrimitives();
}


void  CexmcScenePrimitives::DrawRadialLine( G4VGraphicsScene &  scene,
                                            const CexmcRadialLine *  rLine )
{
    G4double    theta( rLine->theta * deg );
    G4double    phi( rLine->phi * deg );
    G4double    length( rLine->length * cm );
    G4Point3D   radialLineEnd( - std::sin( theta ) * std::cos( phi ) * length,
                               std::sin( theta ) * std::sin( phi ) * length,
                               std::cos( theta ) * length );

    G4Polyline  line;
    line.push_back( G4ThreeVector() );
    line.push_back( radialLineEnd );

    G4VisAttributes  visAttributes( spColours[ CexmcRadialLine_SP ] );
    visAttributes.SetLineWidth( CexmcRadialLineWidth );
    line.SetVisAttributes( visAttributes );

    G4Circle  circle;
    circle.SetScreenSize( CexmcRadialLineCapScreenSize );
    circle.SetFillStyle( G4Circle::filled );
    circle.SetVisAttributes( spColours[ CexmcRadialLine_SP ] );

    const G4AffineTransform &  transform( setup->GetTargetTransform() );
    G4Transform3D              transform3D( G4RotationMatrix(),
                                            transform.NetTranslation() );

    scene.BeginPrimitives( transform3D );
    scene.AddPrimitive( circle );
    scene.AddPrimitive( line );
    scene.EndPrimitives();
}


void  CexmcScenePrimitives::HighlightInnerCrystals( G4VGraphicsScene &  scene )
{
    const CexmcSetup::CalorimeterGeometryData &  calorimeterGeometry(
                                            setup->GetCalorimeterGeometry() );
    G4double  icWidth( calorimeterGeometry.crystalWidth *
                       ( calorimeterGeometry.nCrystalsInRow - 2 ) / 2 );
    G4double  icHeight( calorimeterGeometry.crystalHeight *
                       ( calorimeterGeometry.nCrystalsInColumn - 2 ) / 2 );
    G4double  icLength( calorimeterGeometry.crystalLength / 2 );
    icWidth = icWidth < 0 ? 0 : icWidth;
    icHeight = icHeight < 0 ? 0 : icHeight;

    G4PolyhedronBox  innerCrystals( icWidth, icHeight, icLength );
    G4VisAttributes  visAttributes( spColours[ CexmcInnerCrystalsHl_SP ] );
    visAttributes.SetLineWidth( CexmcICHlLineLineWidth );
    innerCrystals.SetVisAttributes( visAttributes );

    const G4AffineTransform &  transformLeft(
                                        setup->GetCalorimeterLeftTransform() );
    G4Transform3D              transform3DLeft(
                                        transformLeft.NetRotation().inverse(),
                                        transformLeft.NetTranslation() );
    const G4AffineTransform &  transformRight(
                                        setup->GetCalorimeterRightTransform() );
    G4Transform3D              transform3DRight(
                                        transformRight.NetRotation().inverse(),
                                        transformRight.NetTranslation() );

    scene.BeginPrimitives( transform3DLeft );
    scene.AddPrimitive( innerCrystals );
    scene.EndPrimitives();
    scene.BeginPrimitives( transform3DRight );
    scene.AddPrimitive( innerCrystals );
    scene.EndPrimitives();
}

