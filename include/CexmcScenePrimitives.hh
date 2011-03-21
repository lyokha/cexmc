/*
 * =============================================================================
 *
 *       Filename:  CexmcScenePrimitives.hh
 *
 *    Description:  auxiliary scene primitives (radial lines etc.)
 *
 *        Version:  1.0
 *        Created:  03.01.2011 11:27:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_SCENE_PRIMITIVES_HH
#define CEXMC_SCENE_PRIMITIVES_HH

#include <vector>
#include <G4ThreeVector.hh>
#include <G4VModel.hh>
#include <G4VVisManager.hh>

class  G4VGraphicsScene;
class  CexmcSetup;
class  CexmcScenePrimitivesMessenger;


class  CexmcScenePrimitives : public G4VModel
{
    private:
        struct  CexmcRadialLine
        {
            CexmcRadialLine( const G4ThreeVector &  line ) :
                theta( line.x() ), phi( line.y() ), length( line.z() )
            {}

            G4double  theta;

            G4double  phi;

            G4double  length;
        };

        typedef std::vector< CexmcRadialLine >  CexmcRadialLines;

    public:
        explicit CexmcScenePrimitives( CexmcSetup *  setup );

        ~CexmcScenePrimitives();

    public:
        void  DescribeYourselfTo( G4VGraphicsScene &  scene );

    public:
        void  DrawRadialLine( const G4ThreeVector &  line );

        void  MarkTargetCenter( G4bool  on = true );

        void  HighlightInnerCrystals( G4bool = true );

        void  ClearRadialLines( void );

    private:
        void  DrawRadialLine( G4VGraphicsScene &  scene,
                              const CexmcRadialLine *  rLine );

        void  MarkTargetCenter( G4VGraphicsScene &  scene );

        void  HighlightInnerCrystals( G4VGraphicsScene &  scene );

    private:
        void  UpdateScene( void );

    private:
        CexmcSetup *                     setup;

        G4bool                           markTargetCenter;

        G4bool                           highlightInnerCrystals;

        CexmcRadialLines                 radialLines;

    private:
        CexmcScenePrimitivesMessenger *  messenger;
};


inline void  CexmcScenePrimitives::DrawRadialLine( const G4ThreeVector &  line )
{
    radialLines.push_back( line );
    UpdateScene();
}


inline void  CexmcScenePrimitives::MarkTargetCenter( G4bool  on )
{
    markTargetCenter = on;
    UpdateScene();
}


inline void  CexmcScenePrimitives::HighlightInnerCrystals( G4bool  on )
{
    highlightInnerCrystals = on;
    UpdateScene();
}


inline void  CexmcScenePrimitives::ClearRadialLines( void )
{
    radialLines.clear();
    UpdateScene();
}


inline void CexmcScenePrimitives::UpdateScene( void )
{
    G4VVisManager *  visManager( G4VVisManager::GetConcreteInstance() );
    if ( visManager )
        visManager->NotifyHandlers();
}


#endif

