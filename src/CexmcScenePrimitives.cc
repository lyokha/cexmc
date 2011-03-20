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
#include <G4VVisManager.hh>
#include <G4Colour.hh>
#include "CexmcScenePrimitives.hh"
#include "CexmcScenePrimitivesMessenger.hh"


namespace
{
    G4double  CexmcDefaultRadialLineLength( 1 * m );
    G4double  CexmcRadialLineWidth( 2.0 );
    G4double  CexmcRadialLineCapScreenSize( 4.0 );
    G4double  CexmcMarkerScreenSize( 2.0 );
    G4Colour  CexmcRadialLineColour( 1.0, 0.8, 0.0 );
    G4Colour  CexmcMarkerColour( CexmcRadialLineColour );
    G4Colour  CexmcICHlAreaColour( 1.0, 0.2, 0.0, 0.2 );
}


CexmcScenePrimitives *  CexmcScenePrimitives::instance( NULL );


CexmcScenePrimitives *  CexmcScenePrimitives::Instance( void )
{
    if ( instance == NULL )
        instance = new CexmcScenePrimitives;

    return instance;
}


void  CexmcScenePrimitives::Destroy( void )
{
    delete instance;
    instance = NULL;
}


CexmcScenePrimitives::CexmcScenePrimitives() :
    radialLineLength( CexmcDefaultRadialLineLength ), targetCenter( 0, 0, 0 ),
    isInitialized( false ), messenger( NULL )
{
    messenger = new CexmcScenePrimitivesMessenger( this );
}


CexmcScenePrimitives::~CexmcScenePrimitives()
{
    delete messenger;
}


void  CexmcScenePrimitives::Initialize( const CexmcSetup *  setup )
{
    targetCenter = setup->GetTargetTransform().TransformPoint(
                                                G4ThreeVector( 0, 0, 0 ) );
    calorimeterLeftTransform = setup->GetCalorimeterLeftTransform();
    calorimeterRightTransform = setup->GetCalorimeterRightTransform();
    calorimeterGeometry = setup->GetCalorimeterGeometry();
}


void  CexmcScenePrimitives::DrawRadialLine( G4double  angle )
{
    G4VVisManager *  visManager( G4VVisManager::GetConcreteInstance() );

    if ( ! visManager  )
        return;

    G4Polyline       line;
    G4Point3D        radialLineEnd( - std::sin( angle ) * radialLineLength, 0,
                                    std::cos( angle ) * radialLineLength );
    radialLineEnd += targetCenter;
    line.push_back( targetCenter );
    line.push_back( radialLineEnd );

    G4VisAttributes  visAttributes( CexmcRadialLineColour );
    visAttributes.SetLineWidth( CexmcRadialLineWidth );
    line.SetVisAttributes( visAttributes );
    visManager->Draw( line );

    G4Circle  circle;
    circle.SetScreenSize( CexmcRadialLineCapScreenSize );
    circle.SetFillStyle( G4Circle::filled );
    circle.SetVisAttributes( CexmcRadialLineColour );
    circle.SetPosition( targetCenter );
    visManager->Draw( circle );
}


void  CexmcScenePrimitives::MarkTargetCenter( void )
{
    G4VVisManager *  visManager( G4VVisManager::GetConcreteInstance() );

    if ( ! visManager  )
        return;

    G4Circle  circle;
    circle.SetScreenSize( CexmcMarkerScreenSize );
    circle.SetFillStyle( G4Circle::filled );
    circle.SetVisAttributes( CexmcMarkerColour );
    circle.SetPosition( targetCenter );
    visManager->Draw( circle );
}


void  CexmcScenePrimitives::HighlightInnerCrystals( void )
{
    G4VVisManager *  visManager( G4VVisManager::GetConcreteInstance() );

    if ( ! visManager  )
        return;

    G4double  icWidth( calorimeterGeometry.crystalWidth *
                       ( calorimeterGeometry.nCrystalsInRow - 2 ) / 2 );
    G4double  icHeight( calorimeterGeometry.crystalHeight *
                       ( calorimeterGeometry.nCrystalsInColumn - 2 ) / 2 );
    G4double  icLength( calorimeterGeometry.crystalLength / 2 );
    icWidth = icWidth < 0 ? 0 : icWidth;
    icHeight = icHeight < 0 ? 0 : icHeight;

    G4PolyhedronBox  innerCrystals( icWidth, icHeight, icLength );
    G4VisAttributes  visAttributes( CexmcICHlAreaColour );
    innerCrystals.SetVisAttributes( visAttributes );
    visManager->Draw( innerCrystals, G4Transform3D(
                            calorimeterLeftTransform.NetRotation().inverse(),
                            calorimeterLeftTransform.NetTranslation() ) );
    visManager->Draw( innerCrystals, G4Transform3D(
                            calorimeterRightTransform.NetRotation().inverse(),
                            calorimeterRightTransform.NetTranslation() ) );
}

