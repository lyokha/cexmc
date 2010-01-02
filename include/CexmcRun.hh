/*
 * =============================================================================
 *
 *       Filename:  CexmcRun.hh
 *
 *    Description:  run data (acceptances etc.)
 *
 *        Version:  1.0
 *        Created:  19.12.2009 23:52:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_RUN_HH
#define CEXMC_RUN_HH

#include <map>
#include <G4Run.hh>


typedef std::map< G4int, G4int >  CexmcNmbOfHitsInRanges;


class  CexmcRun : public G4Run
{
    public:
        CexmcRun();

    public:
        void  IncrementNmbOfHitsSampled( G4int  index );

        void  IncrementNmbOfHitsTriggeredReal( G4int  index );

        void  IncrementNmbOfHitsTriggeredRec( G4int  index );

        void  IncrementNmbOfOrphanHits( G4int  index );

        void  IncrementNmbOfSavedEvents( void );

        void  IncrementNmbOfSavedFastEvents( void );

    public:
        const CexmcNmbOfHitsInRanges &  GetNmbOfHitsSampled( void ) const;

        const CexmcNmbOfHitsInRanges &  GetNmbOfHitsTriggeredReal( void ) const;

        const CexmcNmbOfHitsInRanges &  GetNmbOfHitsTriggeredRec( void ) const;

        const CexmcNmbOfHitsInRanges &  GetNmbOfOrphanHits( void ) const;

        G4int                           GetNmbOfSavedEvents( void ) const;

        G4int                           GetNmbOfSavedFastEvents( void ) const;

    private:
        CexmcNmbOfHitsInRanges  nmbOfHitsSampled;

        CexmcNmbOfHitsInRanges  nmbOfHitsTriggeredReal;

        CexmcNmbOfHitsInRanges  nmbOfHitsTriggeredRec;

        CexmcNmbOfHitsInRanges  nmbOfOrphanHits;

        G4int                   nmbOfSavedEvents;

        G4int                   nmbOfSavedFastEvents;
};


inline const CexmcNmbOfHitsInRanges &
                            CexmcRun::GetNmbOfHitsSampled( void ) const
{
    return nmbOfHitsSampled;
}


inline const CexmcNmbOfHitsInRanges &
                            CexmcRun::GetNmbOfHitsTriggeredReal( void ) const
{
    return nmbOfHitsTriggeredReal;
}


inline const CexmcNmbOfHitsInRanges &
                            CexmcRun::GetNmbOfHitsTriggeredRec( void ) const
{
    return nmbOfHitsTriggeredRec;
}


inline const CexmcNmbOfHitsInRanges &
                            CexmcRun::GetNmbOfOrphanHits( void ) const
{
    return nmbOfOrphanHits;
}


inline G4int  CexmcRun::GetNmbOfSavedEvents( void ) const
{
    return nmbOfSavedEvents;
}


inline G4int  CexmcRun::GetNmbOfSavedFastEvents( void ) const
{
    return nmbOfSavedFastEvents;
}


#endif

