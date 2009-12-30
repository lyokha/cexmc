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
#include "CexmcAngularRange.hh"

class  G4Event;
class  CexmcPhysicsManager;
class  CexmcEnergyDepositDigitizer;
class  CexmcEnergyDepositStore;
class  CexmcTrackPointsDigitizer;
class  CexmcTrackPointsStore;
class  CexmcEventActionMessenger;
class  CexmcProductionModelData;
class  CexmcChargeExchangeReconstructor;


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

        void      SetVerboseDrawLevel( G4int  verboseDraw_ );

        void      DrawTrajectoryMarkers( G4bool  on );

        CexmcChargeExchangeReconstructor *  GetReconstructor( void );

    private:
        CexmcEnergyDepositStore *  MakeEnergyDepositStore(
                        const CexmcEnergyDepositDigitizer *  digitizer ) const;

        CexmcTrackPointsStore *  MakeTrackPointsStore(
                        const CexmcTrackPointsDigitizer *  digitizer ) const;

        void  PrintEnergyDeposit( const CexmcEnergyDepositStore *  edStore )
                                                                        const;

        void  PrintTrackPoints( const CexmcTrackPointsStore *  tpStore ) const;

        void  PrintProductionModelData(
                        const CexmcAngularRangeList &  angularRanges,
                        const CexmcProductionModelData &  pmData ) const;

        void  PrintReconstructedData(
                        const CexmcAngularRangeList &  angularRanges,
                        const CexmcAngularRange &  angularGap ) const;

        void  FillEnergyDepositHisto( const CexmcEnergyDepositStore *  edStore )
                                                                        const;

        void  DrawTrajectories( const G4Event *  event );

        void  DrawTrackPoints( const CexmcTrackPointsStore *  tpStore ) const;

        void  DrawReconstructionData( void );

        void  UpdateRunHits( const CexmcAngularRangeList &  aRangesReal,
                             const CexmcAngularRangeList &  aRangesRec,
                             G4bool  tpDigitizerHasTriggered,
                             G4bool  edDigitizerHasTriggered,
                             G4bool  reconstructorHasTriggered,
                             const CexmcAngularRange &  aGap );

        void  SaveEvent( const CexmcEnergyDepositStore *  edStore );

    private:
        CexmcPhysicsManager *               physicsManager;

    private:
        CexmcChargeExchangeReconstructor *  reconstructor;

    private:
        G4int                               verbose;

        G4int                               verboseDraw;

        G4bool                              drawTrajectoryMarkers;

        CexmcEventActionMessenger *         messenger;
};


inline void  CexmcEventAction::SetVerboseOnCexmcLevel( G4int  verbose_ )
{
    verbose = verbose_;
}


inline void  CexmcEventAction::SetVerboseDrawLevel( G4int  verboseDraw_ )
{
    verboseDraw = verboseDraw_;
}


inline void  CexmcEventAction::DrawTrajectoryMarkers( G4bool  on )
{
    drawTrajectoryMarkers = on;
}


inline CexmcChargeExchangeReconstructor *
                                    CexmcEventAction::GetReconstructor( void )
{
    return reconstructor;
}


#endif

