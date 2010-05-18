/*
 * =============================================================================
 *
 *       Filename:  CexmcIncidentParticleTrackInfo.hh
 *
 *    Description:  incident particle track info
 *
 *        Version:  1.0
 *        Created:  18.05.2010 13:04:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_INCIDENT_PARTICLE_TRACK_INFO_HH
#define CEXMC_INCIDENT_PARTICLE_TRACK_INFO_HH

#include "CexmcTrackInfo.hh"


class  CexmcIncidentParticleTrackInfo : public CexmcTrackInfo
{
    public:
        explicit CexmcIncidentParticleTrackInfo( CexmcTrackType  trackType =
                                                            CexmcInsipidTrack );

    public:
        G4double  GetTrackLengthInTarget( void ) const;

        void      AddTrackLengthInTarget( G4double  value );

        G4double  GetStepSize( void ) const;

        void      SetStepSize( G4double  value );

        void      ResetTrackLengthInTarget( void );

        G4bool    IsStudiedProcessActivated( void ) const;

        void      ActivateStudiedProcess( G4bool  on = true );

    private:
        G4double  trackLengthInTarget;

        G4double  stepSize;

        G4bool    isStudiedProcessActivated;
};


inline G4double  CexmcIncidentParticleTrackInfo::GetTrackLengthInTarget( void )
                                                                        const
{
    return trackLengthInTarget;
}


inline void  CexmcIncidentParticleTrackInfo::AddTrackLengthInTarget(
                                                            G4double  value )
{
    trackLengthInTarget += value;
}


inline G4double  CexmcIncidentParticleTrackInfo::GetStepSize( void ) const
{
    return stepSize;
}


inline void  CexmcIncidentParticleTrackInfo::SetStepSize( G4double  value )
{
    stepSize = value;
}


inline void  CexmcIncidentParticleTrackInfo::ResetTrackLengthInTarget( void )
{
    trackLengthInTarget = 0.;
}


inline G4bool  CexmcIncidentParticleTrackInfo::IsStudiedProcessActivated( void )
                                                                        const
{
    return isStudiedProcessActivated;
}


inline void  CexmcIncidentParticleTrackInfo::ActivateStudiedProcess(
                                                                G4bool  on )
{
    isStudiedProcessActivated = on;
}


#endif

