/*
 * =============================================================================
 *
 *       Filename:  CexmcRunAction.hh
 *
 *    Description:  run action
 *
 *        Version:  1.0
 *        Created:  20.12.2009 00:15:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_RUN_ACTION_HH
#define CEXMC_RUN_ACTION_HH

#include <G4UserRunAction.hh>
#include <CexmcRun.hh>
#include <CexmcAngularRange.hh>

class  CexmcPhysicsManager;


class  CexmcRunAction : public G4UserRunAction
{
    public:
        explicit CexmcRunAction( CexmcPhysicsManager *  physicsManager );

    public:
        G4Run *  GenerateRun( void );

        void     EndOfRunAction( const G4Run *  run );

    private:
        void     PrintResults(
                        const CexmcNmbOfHitsInRanges &  nmbOfHitsSampled,
                        const CexmcNmbOfHitsInRanges &  nmbOfHitsTriggeredReal,
                        const CexmcNmbOfHitsInRanges &  nmbOfHitsTriggeredRec,
                        const CexmcAngularRangeList &  angularRanges );

    private:
        CexmcPhysicsManager *  physicsManager;
};


#endif

