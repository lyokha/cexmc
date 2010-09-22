/*
 * =============================================================================
 *
 *       Filename:  CexmcTrackPointsDigitizer.hh
 *
 *    Description:  track points collector
 *
 *        Version:  1.0
 *        Created:  24.11.2009 16:09:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_TRACK_POINTS_DIGITIZER_HH
#define CEXMC_TRACK_POINTS_DIGITIZER_HH

#include <G4VDigitizerModule.hh>
#include "CexmcTrackPointInfo.hh"

class  G4String;


class  CexmcTrackPointsDigitizer : public G4VDigitizerModule
{
    public:
        explicit CexmcTrackPointsDigitizer( const G4String &  name );

    public:
        void  Digitize( void );

    public:
        const CexmcTrackPointInfo &  GetMonitorTP( void ) const;

        const CexmcTrackPointInfo &  GetTargetTPBeamParticle( void ) const;

        const CexmcTrackPointInfo &  GetTargetTPOutputParticle( void ) const;

        const CexmcTrackPointInfo &  GetTargetTPNucleusParticle( void ) const;

        const CexmcTrackPointInfo &
                GetTargetTPOutputParticleDecayProductParticle( 
                                                        G4int  index ) const;

        const CexmcTrackPointInfo &  GetVetoCounterTPLeft( void ) const;

        const CexmcTrackPointInfo &  GetVetoCounterTPRight( void ) const;

        const CexmcTrackPointInfo &  GetCalorimeterTPLeft( void ) const;

        const CexmcTrackPointInfo &  GetCalorimeterTPRight( void ) const;

    public:
        G4bool    HasTriggered( void ) const;

    private:
        void      InitializeData( void );

    private:
        CexmcTrackPointInfo  monitorTP;

        CexmcTrackPointInfo  targetTPBeamParticle;

        CexmcTrackPointInfo  targetTPOutputParticle;

        CexmcTrackPointInfo  targetTPNucleusParticle;

        CexmcTrackPointInfo  targetTPOutputParticleDecayProductParticle[ 2 ];

        CexmcTrackPointInfo  vetoCounterTPLeft;

        CexmcTrackPointInfo  vetoCounterTPRight;

        CexmcTrackPointInfo  calorimeterTPLeft;

        CexmcTrackPointInfo  calorimeterTPRight;

        G4bool               hasTriggered;

    private:
        G4int                nCrystalsInColumn;

        G4int                nCrystalsInRow;

        G4double             crystalWidth;

        G4double             crystalHeight;
};


inline const CexmcTrackPointInfo &
            CexmcTrackPointsDigitizer::GetMonitorTP( void ) const
{
    return monitorTP;
}


inline const CexmcTrackPointInfo &
            CexmcTrackPointsDigitizer::GetTargetTPBeamParticle( void ) const
{
    return targetTPBeamParticle;
}


inline const CexmcTrackPointInfo &
            CexmcTrackPointsDigitizer::GetTargetTPOutputParticle( void ) const
{
    return targetTPOutputParticle;
}


inline const CexmcTrackPointInfo &
            CexmcTrackPointsDigitizer::GetTargetTPNucleusParticle( void ) const
{
    return targetTPNucleusParticle;
}


inline const CexmcTrackPointInfo &
    CexmcTrackPointsDigitizer::GetTargetTPOutputParticleDecayProductParticle(
                                                            G4int  index ) const
{
    if ( index == 1 )
        return targetTPOutputParticleDecayProductParticle[ 1 ];

    return targetTPOutputParticleDecayProductParticle[ 0 ];
}


inline const CexmcTrackPointInfo &
            CexmcTrackPointsDigitizer::GetVetoCounterTPLeft( void ) const
{
    return vetoCounterTPLeft;
}


inline const CexmcTrackPointInfo &
            CexmcTrackPointsDigitizer::GetVetoCounterTPRight( void ) const
{
    return vetoCounterTPRight;
}


inline const CexmcTrackPointInfo &
            CexmcTrackPointsDigitizer::GetCalorimeterTPLeft( void ) const
{
    return calorimeterTPLeft;
}


inline const CexmcTrackPointInfo &
            CexmcTrackPointsDigitizer::GetCalorimeterTPRight( void ) const
{
    return calorimeterTPRight;
}


inline G4bool  CexmcTrackPointsDigitizer::HasTriggered( void ) const
{
    return hasTriggered;
}


#endif

