/*
 * =============================================================================
 *
 *       Filename:  CexmcTrackInfo.hh
 *
 *    Description:  track info
 *
 *        Version:  1.0
 *        Created:  22.11.2009 18:42:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_TRACK_INFO_HH
#define CEXMC_TRACK_INFO_HH

#include <G4VUserTrackInformation.hh>
#include "CexmcCommon.hh"


class  CexmcTrackInfo : public G4VUserTrackInformation
{
    public:
        explicit CexmcTrackInfo( CexmcTrackType  trackType =
                                                     CexmcInsipidTrack );

    public:
        void  Print( void ) const;

    public:
        CexmcTrackType  GetTrackType( void ) const;

        void            SetTrackType( CexmcTrackType  value );

        G4double        GetTrackLengthInTarget( void ) const;

        void            AddTrackLengthInTarget( G4double  value );

    private:
        CexmcTrackType  trackType;

        G4double        trackLengthInTarget;
};


inline CexmcTrackType  CexmcTrackInfo::GetTrackType( void ) const
{
    return trackType;
}


inline void  CexmcTrackInfo::SetTrackType( CexmcTrackType  value )
{
    trackType = value;
}


inline G4double  CexmcTrackInfo::GetTrackLengthInTarget( void ) const
{
    return trackLengthInTarget;
}


inline void  CexmcTrackInfo::AddTrackLengthInTarget( G4double  value )
{
    trackLengthInTarget += value;
}


#endif

