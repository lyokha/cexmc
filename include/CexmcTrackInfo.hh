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

    private:
        CexmcTrackType  trackType;
};


inline CexmcTrackType  CexmcTrackInfo::GetTrackType( void ) const
{
    return trackType;
}


inline void  CexmcTrackInfo::SetTrackType( CexmcTrackType  value )
{
    trackType = value;
}


#endif

