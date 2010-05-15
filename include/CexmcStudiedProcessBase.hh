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
        G4int  GetNumberOfTriggeredEvents( void ) const;

        void   IncrementNumberOfTriggeredEvents( void );

        void   ResetNumberOfTriggeredEvents( void );

        void   ResetTrackWatchedState( void );

    protected:
        G4bool  trackWatched;

        G4int   numberOfTriggeredEvents;
};


inline G4int  CexmcStudiedProcessBase::GetNumberOfTriggeredEvents( void ) const
{
    return numberOfTriggeredEvents;
}


inline void  CexmcStudiedProcessBase::IncrementNumberOfTriggeredEvents( void )
{
    ++numberOfTriggeredEvents;
}


inline void  CexmcStudiedProcessBase::ResetNumberOfTriggeredEvents( void )
{
    numberOfTriggeredEvents = 0;
}


inline void  CexmcStudiedProcessBase::ResetTrackWatchedState( void )
{
    trackWatched = false;
}


#endif

