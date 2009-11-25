/*
 * =============================================================================
 *
 *       Filename:  CexmcEventAction.hh
 *
 *    Description:  event action
 *
 *        Version:  1.0
 *        Created:  27.10.2009 22:41:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_EVENT_ACTION_HH
#define CEXMC_EVENT_ACTION_HH

#include <G4UserEventAction.hh>

class  G4Event;
class  CexmcPhysicsManager;
class  CexmcEnergyDepositDigitizer;
class  CexmcEnergyDepositStore;
class  CexmcTrackPointsDigitizer;
class  CexmcTrackPointsStore;
class  CexmcEventActionMessenger;


class  CexmcEventAction : public G4UserEventAction
{
    public:
        explicit CexmcEventAction( CexmcPhysicsManager *  physicsManager,
                                   G4int  verbose = 0 );

        virtual ~CexmcEventAction();

    public:
        void      BeginOfEventAction( const G4Event *  event );

        void      EndOfEventAction( const G4Event *  event );

    public:
        void      SetVerboseOnCexmcLevel( G4int  verbose_ );

    private:
        CexmcEnergyDepositStore *  MakeEnergyDepositStore(
                        const CexmcEnergyDepositDigitizer *  digitizer ) const;

        CexmcTrackPointsStore *  MakeTrackPointsStore(
                        const CexmcTrackPointsDigitizer *  digitizer ) const;

        void  PrintEnergyDeposit( const CexmcEnergyDepositStore *  edStore )
                                                                        const;

        void  PrintTrackPoints( const CexmcTrackPointsStore *  tpStore ) const;

    private:
        CexmcPhysicsManager *  physicsManager;

    private:
        G4int                        verbose;

        CexmcEventActionMessenger *  messenger;
};


inline void  CexmcEventAction::SetVerboseOnCexmcLevel( G4int  verbose_ )
{
    verbose = verbose_;
}


#endif

