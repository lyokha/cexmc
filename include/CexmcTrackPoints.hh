/*
 * =============================================================================
 *
 *       Filename:  CexmcTrackPoints.hh
 *
 *    Description:  track points collection
 *
 *        Version:  1.0
 *        Created:  16.11.2009 12:41:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_TRACK_POINTS_HH
#define CEXMC_TRACK_POINTS_HH

#include <G4THitsMap.hh>
#include "CexmcPrimitiveScorer.hh"
#include "CexmcTrackPointInfo.hh"

class  G4HCofThisEvent;
class  G4Step;


typedef G4THitsMap< CexmcTrackPointInfo >  CexmcTrackPointsCollection;


class  CexmcTrackPoints : public CexmcPrimitiveScorer
{
    public:
        explicit CexmcTrackPoints( const G4String &  name );

    public:
        void   Initialize( G4HCofThisEvent *  hcOfThisEvent );

        void   EndOfEvent( G4HCofThisEvent *  hcOfThisEvent );

        void   DrawAll( void );

        void   PrintAll( void );

        void   clear( void );

    protected:
        G4int   GetTrackId( G4Step *  step );

        G4int   GetIndex( G4Step *  step );

        G4bool  ProcessHits( G4Step *  step, G4TouchableHistory *  tHistory );

    protected:
        CexmcTrackPointsCollection *  eventMap;

    private:
        G4int                         hcId;
};


#endif

