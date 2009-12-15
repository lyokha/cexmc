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
#include "CexmcParticleGun.hh"

class  CexmcParticleGun;


class  CexmcPrimaryVertexInfo : public G4VUserPrimaryVertexInformation
{
    public:
        explicit CexmcPrimaryVertexInfo(
                                    const CexmcParticleGun *  particleGun );

    public:
        void  Print( void ) const;

    public:
        const CexmcParticleGun *  GetParticleGun( void ) const;

    private:
        const CexmcParticleGun *  particleGun;
};


inline const CexmcParticleGun *
                            CexmcPrimaryVertexInfo::GetParticleGun( void ) const
{
    return particleGun;
}


#endif

