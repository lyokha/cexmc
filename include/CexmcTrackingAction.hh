/*
 * =============================================================================
 *
 *       Filename:  CexmcTrackingAction.hh
 *
 *    Description:  tracking action
 *
 *        Version:  1.0
 *        Created:  22.11.2009 17:08:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_TRACKING_ACTION_HH
#define CEXMC_TRACKING_ACTION_HH

#include <G4UserTrackingAction.hh>

class  G4ParticleDefinition;
class  G4Track;


class  CexmcTrackingAction : public G4UserTrackingAction
{
    public:
        CexmcTrackingAction();

    public:
        void  PreUserTrackingAction( const G4Track *  track );

    private:
        G4int                         outputParticleTrackId;
};


#endif

