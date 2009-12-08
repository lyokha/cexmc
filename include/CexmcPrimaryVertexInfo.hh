/*
 * =============================================================================
 *
 *       Filename:  CexmcPrimaryVertexInfo.hh
 *
 *    Description:  original position, direction and momentum
 *
 *        Version:  1.0
 *        Created:  07.12.2009 23:51:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_PRIMARY_VERTEX_INFO_HH
#define CEXMC_PRIMARY_VERTEX_INFO_HH

#include <G4VUserPrimaryVertexInformation.hh>
#include <G4ThreeVector.hh>


class  CexmcPrimaryVertexInfo : public G4VUserPrimaryVertexInformation
{
    public:
        CexmcPrimaryVertexInfo( const G4ThreeVector &  position,
                                const G4ThreeVector &  direction,
                                G4double  momentumAmp );

    public:
        void  Print( void ) const;

    public:
        const G4ThreeVector &  GetPosition( void ) const;

        const G4ThreeVector &  GetDirection( void ) const;

        G4double               GetMomentumAmp( void ) const;

    private:
        G4ThreeVector  position;

        G4ThreeVector  direction;

        G4double       momentumAmp;
};


inline const G4ThreeVector &  CexmcPrimaryVertexInfo::GetPosition( void ) const
{
    return position;
}


inline const G4ThreeVector &  CexmcPrimaryVertexInfo::GetDirection( void ) const
{
    return direction;
}


inline G4double  CexmcPrimaryVertexInfo::GetMomentumAmp( void ) const
{
    return momentumAmp;
}


#endif

