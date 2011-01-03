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
#include <G4ThreeVector.hh>
#include <G4VisAttributes.hh>
#include <G4VisManager.hh>
#include <G4Colour.hh>
#include "CexmcScenePrimitives.hh"
#include "CexmcScenePrimitivesMessenger.hh"
#include "CexmcSetup.hh"


namespace
{
    G4Colour  CexmcRadialLineColour( 1.0, 0.8, 0.0 );
    G4double  CexmcRadialLineWidth( 2.0 );
    G4double  CexmcRadialLineCapScreenSize( 4.0 );
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


CexmcScenePrimitives::CexmcScenePrimitives() : visManager( NULL ),
    radialLineLength( 1 * m ), radialLineOrigin( 0, 0, 0 ),
    isInitialized( false ), messenger( NULL )
{
    messenger = new CexmcScenePrimitivesMessenger( this );
}


CexmcScenePrimitives::~CexmcScenePrimitives()
{
    delete messenger;
}


void  CexmcScenePrimitives::Initialize( G4VisManager *  visManager_,
                                        const CexmcSetup *  setup )
{
    visManager = visManager_;
    G4ThreeVector  vec( setup->GetTargetTransform().TransformPoint(
                                                G4ThreeVector( 0, 0, 0 ) ) );
    radialLineOrigin.setX( vec.x() );
    radialLineOrigin.setY( vec.y() );
    radialLineOrigin.setZ( vec.z() );
}


void  CexmcScenePrimitives::DrawRadialLine( G4double  angle )
{
    G4Polyline       line;
    G4Point3D        radialLineEnd( - std::sin( angle ) * radialLineLength, 0,
                                    std::cos( angle ) * radialLineLength );
    radialLineEnd += radialLineOrigin;
    line.push_back( radialLineOrigin );
    line.push_back( radialLineEnd );

    G4VisAttributes  visAttributes( CexmcRadialLineColour );
    visAttributes.SetLineWidth( CexmcRadialLineWidth );
    line.SetVisAttributes( visAttributes );
    visManager->Draw( line );

    G4Circle         circle;
    circle.SetScreenSize( CexmcRadialLineCapScreenSize );
    circle.SetFillStyle( G4Circle::filled );
    circle.SetVisAttributes( visAttributes );
    circle.SetPosition( G4ThreeVector( 0, 0, 0 ) );
    visManager->Draw( circle );
}

