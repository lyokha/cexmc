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

#include <G4Point3D.hh>
#include <G4AffineTransform.hh>
#include "CexmcSetup.hh"

class  CexmcScenePrimitivesMessenger;


class  CexmcScenePrimitives
{
    public:
        static CexmcScenePrimitives *  Instance( void );

        static void                    Destroy( void );

    private:
        CexmcScenePrimitives();

        ~CexmcScenePrimitives();

    public:
        void  Initialize( const CexmcSetup *  setup );

        void  SetRadialLineLength( G4double  value );

        void  DrawRadialLine( G4double  angle );

        void  MarkTargetCenter( void );

        void  HighlightInnerCrystals( void );

    private:
        G4double                             radialLineLength;

        G4Point3D                            targetCenter;

        G4AffineTransform                    calorimeterLeftTransform;

        G4AffineTransform                    calorimeterRightTransform;

        CexmcSetup::CalorimeterGeometryData  calorimeterGeometry;

        G4bool                               isInitialized;

    private:
        CexmcScenePrimitivesMessenger *      messenger;

    private:
        static CexmcScenePrimitives *        instance;
};


inline void  CexmcScenePrimitives::SetRadialLineLength( G4double  value )
{
    radialLineLength = value;
}


#endif

