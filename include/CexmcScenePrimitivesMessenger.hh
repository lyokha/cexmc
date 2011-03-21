/*
 * =============================================================================
 *
 *       Filename:  CexmcScenePrimitivesMessenger.hh
 *
 *    Description:  draw auxiliary scene primitives
 *
 *        Version:  1.0
 *        Created:  03.01.2011 12:34:09
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_SCENE_PRIMITIVES_MESSENGER_HH
#define CEXMC_SCENE_PRIMITIVES_MESSENGER_HH

#include <G4UImessenger.hh>

class  G4UIcommand;
class  G4UIcmdWith3Vector;
class  G4UIcmdWithABool;
class  G4UIcmdWithoutParameter;
class  CexmcScenePrimitives;


class  CexmcScenePrimitivesMessenger : public G4UImessenger
{
    public:
        explicit CexmcScenePrimitivesMessenger(
                                    CexmcScenePrimitives *  scenePrimitives );

        ~CexmcScenePrimitivesMessenger();

    public:
        void  SetNewValue( G4UIcommand *  cmd, G4String  value );

    private:
        CexmcScenePrimitives *     scenePrimitives;

        G4UIcmdWith3Vector *       drawRadialLine;

        G4UIcmdWithoutParameter *  clearRadialLines;

        G4UIcmdWithABool *         markTargetCenter;

        G4UIcmdWithABool *         highlightInnerCrystals;
};


#endif

