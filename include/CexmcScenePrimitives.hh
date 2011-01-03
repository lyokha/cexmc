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

class  G4VisManager;
class  CexmcSetup;
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
        void  Initialize( G4VisManager *  visManager_,
                          const CexmcSetup *  setup );

        void  SetRadialLineLength( G4double  value );

        void  DrawRadialLine( G4double  angle );

    private:
        G4VisManager *  visManager;

        G4double        radialLineLength;

        G4Point3D       radialLineOrigin;

        G4bool          isInitialized;

    private:
        CexmcScenePrimitivesMessenger *  messenger;

    private:
        static CexmcScenePrimitives *    instance;
};


inline void  CexmcScenePrimitives::SetRadialLineLength( G4double  value )
{
    radialLineLength = value;
}


#endif

