/*
 * =============================================================================
 *
 *       Filename:  CexmcProductionModel.hh
 *
 *    Description:  interface for production model
 *
 *        Version:  1.0
 *        Created:  03.11.2009 16:50:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_PRODUCTION_MODEL_HH
#define CEXMC_PRODUCTION_MODEL_HH

#include <G4Types.hh>

class CexmcProductionModelMessenger;


class  CexmcProductionModel
{
    public:
        explicit CexmcProductionModel( G4bool  fermiMotionIsOn = true );

        virtual ~CexmcProductionModel();

    public:
        void  ApplyFermiMotion( G4bool  on );

    protected:
        G4bool  fermiMotionIsOn;

    private:
        CexmcProductionModelMessenger *  messenger;
};


inline void  CexmcProductionModel::ApplyFermiMotion( G4bool  on )
{
    fermiMotionIsOn = on;
}


#endif

