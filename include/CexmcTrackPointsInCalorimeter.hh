/*
 * =============================================================================
 *
 *       Filename:  CexmcTrackPointsInCalorimeter.hh
 *
 *    Description:  track points in calorimeter
 *
 *        Version:  1.0
 *        Created:  22.11.2009 21:57:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_TRACK_POINTS_IN_CALORIMETER_HH
#define CEXMC_TRACK_POINTS_IN_CALORIMETER_HH

#include "CexmcTrackPointsInLeftRightSet.hh"


class  CexmcTrackPointsInCalorimeter : public CexmcTrackPointsInLeftRightSet
{
    public:
        explicit CexmcTrackPointsInCalorimeter( const G4String &  name );

    public:
        void  PrintAll( void );

    protected:
        G4int  GetIndex( G4Step *  step );

    protected:
        static G4int  copyDepth0BitsOffset;

        static G4int  copyDepth1BitsOffset;
};


#endif

