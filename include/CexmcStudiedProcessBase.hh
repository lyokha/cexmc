/*
 * =============================================================================
 *
 *       Filename:  CexmcStudiedProcessBase.hh
 *
 *    Description:  basic interface for Studied Process
 *
 *        Version:  1.0
 *        Created:  12.05.2010 23:18:08
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_STUDIED_PROCESS_BASE_HH
#define CEXMC_STUDIED_PROCESS_BASE_HH

#include <G4WrapperProcess.hh>


class  CexmcStudiedProcessBase : public G4WrapperProcess
{
    public:
        CexmcStudiedProcessBase();

    public:
        void      ResetTrackWatchedState( void );

    protected:
        G4bool    trackWatched;
};


inline void  CexmcStudiedProcessBase::ResetTrackWatchedState( void )
{
    trackWatched = false;
}


#endif

